with import <nixpkgs> {};
callPackage ./uefi-app-playground.nix {
  edk2 = callPackage ./edk2.nix {};
}
