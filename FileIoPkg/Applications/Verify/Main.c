/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseCryptLib.h>

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
  if (Argc > 1) {
    //UINT8    *Data = NULL;
    CHAR16 *Data;
    UINTN    DataSize;
    UINT8    *HashValue;
    BOOLEAN  Status;
    VOID     *HashCtx;
    UINTN    CtxSize;

    Data = Argv[1];
    Status  = FALSE;
    HashCtx = NULL;

    DataSize = StrLen (Data);
    Print(L"DataSize: %d\n", DataSize);

    HashValue = AllocatePool (SHA512_DIGEST_SIZE);
    Print(L"Allocated %d bytes for HashValue\n", SHA512_DIGEST_SIZE);

    CtxSize = Sha512GetContextSize ();
    HashCtx = AllocatePool (CtxSize);
    if (HashCtx != NULL)
      Print(L"%d bytes for HashCtx allocated\n", CtxSize);
    Status = Sha512Init   (HashCtx);
    if (Status == TRUE)
      Print(L"Sha512 inited\n");
    Status = Sha512Update (HashCtx, Data, DataSize);
    if (Status == TRUE)
      Print(L"Sha512 updated\n");
    Status = Sha512Final  (HashCtx, HashValue);
    if (Status == TRUE)
      Print(L"Sha512 finaled\n");

    Print(L"Hash of %s is ", Data);
    int i;
    for(i = 0; i < SHA512_DIGEST_SIZE; i++) {
      Print(L"%02x", HashValue[i]);
    }
    Print(L"\n");

    if (HashCtx != NULL) {
      FreePool (HashCtx);
    }

    Status = Status;  // TODO handle Status properly
  } else {
    Print(L"Usage Verify.efi [STRING]\n");
  }

  return(0);
}
