{
  description = "H2INC-reimplementation based on LLVM";

  outputs = { self, nixpkgs, ... }: {
    packages.x86_64-linux = let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in {
      default = pkgs.callPackage ./default.nix {
        version = self.lastModifiedDate;
      };
    };

    devShells.x86_64-linux.default = self.outputs.packages.x86_64-linux.default.overrideAttrs(old: {
      hardeningDisable = (old.hardeningDisable or []) ++ [ "all" ];
    });
  };
}
