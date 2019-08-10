{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  inputsFrom = with pkgs; [
    OVMF edk2
  ];
}
