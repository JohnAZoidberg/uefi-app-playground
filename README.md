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
Prerequisites: Install [Nix](https://nixos.org/nix) package manager

```
# Launch nix-shell with dependencies
nix-shell

# Initialize EDK2 submodule and go inside
git submodule update --init
cd edk2

# Symlink package into EDK2
ln -s ../FileIoPkg

# Build EDK2 base tools
make -C BaseTools

# Source EDK2 build system's environment variables
source edksetup.sh BaseTools

# TODO: Don't we need to build the base tools?

# Build our package
build -a X64 -p FileIoPkg/FileIoPkg.dsc -n$(nproc) -b DEBUG -t GCC5
```

## Run
### On hardware
1. Copy executable from `result/X64/FileIo.efi`/`Build/FileIoPkg/DEBUG_GCC5/X64/FileIo.efi` to your ESP
2. Boot into an EFI-Shell and type the name of the executable

### In QEMU

Start up QEMU without network (to prevent PXE boot), custom firmware to force UEFI boot and directory with built executable as FAT partition which is accessible from the UEFI commandline.

```
qemu-system-x86_64 -net none \
    -pflash Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd \
    -hda fat:Build/FileIoPkg/DEBUG_GCC5/X64/
```

## License
Same license as EDKII: BSD 2-Clause
