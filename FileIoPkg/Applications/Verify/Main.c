/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    Copyright (c) 2018 Daniel Schaefer
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/ShellLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseCryptLib.h>

BOOLEAN Hash(UINTN DataSize, IN VOID *Data, OUT UINT8 *HashValue)
{
  BOOLEAN  Status;
  VOID    *HashCtx = NULL;
  UINTN    CtxSize = Sha512GetContextSize ();

  HashCtx = AllocatePool (CtxSize);
  if (HashCtx != NULL)
    Print(L"%d bytes for HashCtx allocated\n", CtxSize);
  Status = Sha512Init (HashCtx);
  if (Status == 1)
    Print(L"Sha512 inited\n");
  Status = Sha512Update (HashCtx, Data, DataSize);
  if (Status == 1)
    Print(L"Sha512 updated\n");
  Status = Sha512Final (HashCtx, HashValue);
  if (Status == 1)
    Print(L"Sha512 finaled\n");

  if (HashCtx != NULL) {
    FreePool (HashCtx);
  }
  return Status;
}

VOID PrintHex(
  IN UINT8 *HashValue
  )
{
    UINT8 I;
    for(I = 0; I < SHA512_DIGEST_SIZE; I++) {
      Print(L"%02x", HashValue[I]);
    }
    Print(L"\n");
}

VOID PrintCertSubject(
  IN UINT8  *Cert,
  IN UINTN CertLen
  )
{
  UINT8 *Subject;
  UINTN  SubjectSize = 0;

  // Get size of subject
  BOOLEAN Err = X509GetSubjectName (Cert, CertLen, NULL, &SubjectSize);
  Print(L"SubjectSize: %d\n", SubjectSize);
  if (!Err) {
    Print(L"Couldn't read subjectlen of cert.\n");
  }

  Subject = AllocateZeroPool (SubjectSize);
  Err = X509GetSubjectName (Cert, CertLen, Subject, &SubjectSize);
  Print(L"SubjectSize: %d\n", SubjectSize);
  if (!Err) {
    Print(L"Couldn't read subject of cert.\n");
  }

  Print(L"Subject: %s\n", Subject);
  FreePool (Subject);
}

EFI_STATUS
ReadFile (
  IN  CHAR16 *FileName,
  OUT UINT8 **Content,
  OUT UINTN  *ContentLen
  )
{
    SHELL_FILE_HANDLE FileHandle;
    EFI_STATUS        Status;

    Status = ShellOpenFileByName(
      FileName, &FileHandle, EFI_FILE_MODE_READ, 0
    );
    Status = ShellGetFileSize (FileHandle, ContentLen);
    // TODO reallocate if we actually need the last byte
    *Content = AllocateZeroPool (*ContentLen + 1);
    Status = ShellReadFile (FileHandle, ContentLen, *Content);
    ShellCloseFile (&FileHandle);

    return Status;
}

/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  if (Argc == 2) {
    BOOLEAN Status;
    UINT8   *HashValue;
    UINTN    DataSize;

    DataSize = StrLen (Argv[1]);
    Print(L"DataSize: %d\n", DataSize);
    HashValue = AllocatePool (SHA512_DIGEST_SIZE);
    Print(L"Allocated %d bytes for HashValue\n", SHA512_DIGEST_SIZE);

    Status = Hash (DataSize, Argv[1], HashValue);
    PrintHex (HashValue);
    FreePool (HashValue);
    Status = Status;  // TODO handle Status properly
  } else if (Argc == 3) {
    EFI_STATUS        Status;

    UINT8            *RawCert;
    UINTN             RawCertLen;
    UINT8            *RawChain;
    UINTN             RawChainLen;

    // Read root cert
    Status = ReadFile (Argv[1], &RawCert, &RawCertLen);
    //Print(L"RawCert: %s (%d)\n", RawCert, RawCertLen);
    PrintCertSubject (RawCert, RawCertLen);

    // Read cert chain
    Status = ReadFile (Argv[2], &RawChain, &RawChainLen);
    //Print(L"RawChain: %s (%d)\n", RawChain, RawChainLen);

    // Validate full chain
    if (NULL == RawChain) Print(L"RawChain is NULL\n");
    Print(L"RawChainLen: %d\n", RawChainLen);
    if (NULL == RawCert) Print(L"RawCert is NULL\n");
    Print(L"RawCertLen: %d\n", RawCertLen);
    BOOLEAN Result = X509VerifyCert (
      RawChain, RawChainLen,
      RawCert, RawCertLen
    );
    Print(L"Chain validated: %d\n", Result);

    UINT8   *HashValue = AllocatePool (SHA512_DIGEST_SIZE);
    Print(L"Allocated %d bytes for HashValue\n", SHA512_DIGEST_SIZE);
    Status = Hash (RawCertLen, RawCert, HashValue);
    PrintHex (HashValue);
    FreePool (HashValue);

    FreePool (RawCert);
    FreePool (RawChain);
    Status = Status;
  } else {
    Print(L"Usage Verify.efi [String to hash]\n");
    Print(L"Usage Verify.efi [Root-Cert] [Cert-Chain]\n");
  }

  return(0);
}
