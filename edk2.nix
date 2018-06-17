{ stdenv, fetchFromGitHub, fetchpatch, libuuid, python2, iasl, nasm }:

let
  pythonEnv = python2.withPackages(ps: [ps.tkinter]);

targetArch =  "X64";

edk2 = stdenv.mkDerivation {
  name = "edk2-UDK2018";

  src = fetchFromGitHub {
    owner = "tianocore";
    repo = "edk2";
    rev = "vUDK2018";
    sha256 = "12d79g48fdpbdb1jh9k5blajvg4r63b0m2nsiiml3dlnf3jiakw0";
    #sha256 = "1ll4xik9xw5spzcwnm5h34i8y6yag1nng8j55ji8jqiwrf94x23f"; no submodules
    fetchSubmodules = true;
  };

  buildInputs = [ libuuid pythonEnv ];

  makeFlags = "-C BaseTools";

  hardeningDisable = [ "format" "fortify" ];

  installPhase = ''
    mkdir -vp $out
    mv -v BaseTools $out
    mv -v EdkCompatibilityPkg $out
    mv -v edksetup.sh $out
  '';

  enableParallelBuilding = true;

  meta = {
    description = "Tianocore EFI development kit";
    homepage = https://github.com/tianocore/edk2;
    license = stdenv.lib.licenses.bsd2;
    platforms = ["x86_64-linux" "i686-linux" "aarch64-linux"];
  };

  passthru = {
    setup = projectDscPath: attrs: {
      buildInputs = [ pythonEnv ] ++
        stdenv.lib.optionals (attrs ? buildInputs) attrs.buildInputs;

      configurePhase = ''
        mkdir -v Conf

        cp ${edk2}/BaseTools/Conf/target.template Conf/target.txt
        sed -i Conf/target.txt \
          -e 's|Nt32Pkg/Nt32Pkg.dsc|${projectDscPath}|' \
          -e 's|MYTOOLS|GCC49|' \
          -e 's|IA32|${targetArch}|' \
          -e 's|RELEASE|DEBUG|'\

        cp ${edk2}/BaseTools/Conf/tools_def.template Conf/tools_def.txt
        sed -i Conf/tools_def.txt \
          -e 's|DEFINE GCC48_IA32_PREFIX       = /usr/bin/|DEFINE GCC48_IA32_PREFIX       = ""|' \
          -e 's|DEFINE GCC48_X64_PREFIX        = /usr/bin/|DEFINE GCC48_X64_PREFIX        = ""|' \
          -e 's|DEFINE UNIX_IASL_BIN           = /usr/bin/iasl|DEFINE UNIX_IASL_BIN           = ${iasl}/bin/iasl|'

        export WORKSPACE="$PWD"
        export EFI_SOURCE="$PWD/EdkCompatibilityPkg"
        ln -sv ${edk2}/BaseTools BaseTools
        ln -sv ${edk2}/EdkCompatibilityPkg EdkCompatibilityPkg
        . ${edk2}/edksetup.sh BaseTools
      '';

      installPhase = ''
        mv -v Build/*/* $out
      '';

    } // (removeAttrs attrs [ "buildInputs" ] );
  };
};

in

edk2
