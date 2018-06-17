# UEFI App Playground
I created this repository to play around with UEFI apps.
It shows how easy it is to write UEFI apps (using the C standard library),
and build them using [EDKII](https://github.com/tianocore/edk2).

## Demo Apps
### FileIo.efi
Prints a hello world message if provided no arguments.
Reads content of provided file into memory and prints them.

### Verify.efi
Prints the sha512 hash of the first argument using openssl.

## Build
1. Install [Nix]() package manager
2. Run `nix-build`

## Run
1. Copy executable from `result/X64/FileIo.efi` to your ESP
2. Boot into an EFI-Shell and type the name of the executable

## License
Same license as EDKII: BSD 2-Clause
