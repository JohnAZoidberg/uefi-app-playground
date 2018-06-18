/** @file
    A simple, basic, application showing how the "Standard C Libraries" from
    StdLib can be used to interact with the file system.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    Copyright (c) 2018 Daniel Schaefer
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>

#include  <stdlib.h>
#include  <stdio.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <unistd.h>
#include  <string.h>

/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int
main (
  IN int Argc,
  IN char **Argv
  )
{

  puts("Hello there fellow Programmer.");
  puts("Welcome to the world of EDK II.");

  if (Argc > 1) {
    int Fildes = open(Argv[1], O_RDONLY, 777);
    if (Fildes < 0) {
        printf("Ooops Filedes was %d", Fildes);
	return 1;  // exit(1);
    }
    printf("Opened file %s\n", Argv[1]);

    char *Buffer = malloc(128);
    printf("Allocated Buffer\n");

    memset(Buffer, '\0', 128);
    printf("Zeroed Buffer\n");

    ssize_t bytes = read(Fildes, Buffer, 127);
    printf("Read %d bytes from Fildes\n", bytes);

    printf("Read bytes: '%s'\n", Buffer);

    close(Fildes);
    printf("Closed Fildes\n");
  } else {
    printf("You didn't provide any arguments :/\n");
  }

  return 0;
}
