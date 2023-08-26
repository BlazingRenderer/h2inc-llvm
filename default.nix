{ stdenv
, version
, cmake
, llvmPackages_16
}:
stdenv.mkDerivation {
  inherit version;

  pname = "h2inc-llvm";

  src = ./.;

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    llvmPackages_16.llvm
    llvmPackages_16.libclang
  ];

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    ls -laR
    cp h2inc-llvm $out/bin

    runHook postInstall
  '';
}

