{
  description = "exec and spawn utils for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = rec {
          default = exspcpp;

          exspcpp = pkgs.stdenv.mkDerivation {
            pname = "exspcpp";
            version = "0.0.0";
            src = nixpkgs.lib.cleanSource ./.;
            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];
            buildInputs = [
              tinycmmc.packages.${system}.default
            ];
            propagatedBuildInputs = with pkgs; [
              gtest
            ];
           };
        };
      }
    );
}
