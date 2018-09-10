# UEFI App Playground
I created this repository to play around with UEFI apps.
It shows how easy it is to write UEFI apps (using the C standard library),
and build them using [EDKII](https://github.com/tianocore/edk2).

## Demo Apps
### FileIo.efi (Regular C-style)
Prints a hello world message if provided no arguments.
Reads content of provided file into memory and prints them.

### Verify.efi (EDKII C-style)
Prints the SHA-512 hash of the first argument using OpenSSL.

## Build
1. Install [Nix](https://nixos.org/nix) package manager
2. Run `nix-build`

or

1. Install [Nix](https://nixos.org/nix) package manager
2. Launch nix-shell with deps: `nix-shell '<nixpkgs> -A OVMF`
3. `git clone --recursive https://github.com/tianocore/edk2`
4. `ln -s ../FileIoPkg edk2/`
5. `source edksetup.sh BaseTools`
6. `build -a X64 -p FileIoPkg/FileIoPkg.dsc -n$(nproc) -b DEBUG -t GCC5`

Step 5 sources environment variables and add the tools to your PATH.
Step 6 builds the app.

## Run
### On hardware
1. Copy executable from `result/X64/FileIo.efi`/`Build/FileIoPkg/DEBUG_GCC5/X64/FileIo.efi` to your ESP
2. Boot into an EFI-Shell and type the name of the executable

### In QEMU

Start up QEMU without network (to prevent PXE boot), custom firmware to force UEFI boot and directory with built executable as FAT partition which is accessible from the UEFI commandline.
```
qemu-system-x86_64 -net none \
    -pflash Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd  \
    -hda fat:Build/FileIoPkg/DEBUG_GCC5/X64/
```

## License
Same license as EDKII: BSD 2-Clause
