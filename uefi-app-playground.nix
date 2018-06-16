{ stdenv, lib, overrideCC, edk2, nasm, iasl, openssl, gcc49 }:

let

  projectDscPath = "FileIoPkg/FileIoPkg.dsc";

  version = "0.1.0";
  uefiAppPlayground = ./.;
  srcs = [ edk2.src uefiAppPlayground ];
  newStdenv = overrideCC stdenv gcc49;
in

newStdenv.mkDerivation (edk2.setup projectDscPath {
  name = "uefi-app-playground-${version}";

  inherit srcs;

  buildInputs = [ nasm iasl openssl ];

  hardeningDisable = [ "stackprotector" "pic" "fortify" ];

  # BaseTools,EdkCompatibilityPkg, cannot be symlinked for some reason
  unpackPhase = ''
    for file in \
      "${edk2.src}"/{AppPkg,ArmPkg,ArmPlatformPkg,ArmVirtPkg,BeagleBoardPkg,CorebootModulePkg,CorebootPayloadPkg,CryptoPkg,DuetPkg,EdkShellBinPkg,EdkShellPkg,EmbeddedPkg,EmulatorPkg,FatBinPkg,FatPkg,IntelFrameworkModulePkg,IntelFrameworkPkg,IntelFsp2Pkg,IntelFsp2WrapperPkg,IntelFspPkg,IntelFspWrapperPkg,IntelSiliconPkg,MdeModulePkg,MdePkg,NetworkPkg,Nt32Pkg,Omap35xxPkg,OptionRomPkg,OvmfPkg,PcAtChipsetPkg,QuarkPlatformPkg,QuarkSocPkg,SecurityPkg,ShellBinPkg,ShellPkg,SignedCapsulePkg,SourceLevelDebugPkg,StandaloneMmPkg,StdLib,StdLibPrivateInternalFiles,UefiCpuPkg,UnixPkg,Vlv2DeviceRefCodePkg,Vlv2TbltDevicePkg} \
      "${uefiAppPlayground}"/FileIoPkg;
    do
      ln -sv "$file" .
    done
  '';

  buildPhase = ''
      build -n $NIX_BUILD_CORES
      #build -a X64 -p FileIoPkg/FileIoPkg.dsc -n $NIX_BUILD_CORES -b DEBUG
  '';

  dontPatchELF = true;
  dontStrip = true;

  meta = {
    description = "Playground for testing/writing UEFI Apps using EDK2";
    license = lib.licenses.bsd2;
    platforms = [ "x86_64-linux" ];
    maintainer = lib.maintainers.johnazoidberg;
  };
})
