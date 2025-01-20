{
  description = "A devShell example";
  inputs = {
    nixpkgs.url      = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url  = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    # calling a function from `flake-utils` that takes a lambda
    # that takes the system we're targetting
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
                # get current working directory
        nativeBuildInputs = with pkgs; [
          libGL
          cmake
          pkg-config
          libglvnd
          lcov
        ];
        buildInputs = with pkgs;[
          pcre2
          libffi
          qt6.qtbase
          qt6.qttools
          qt6.qtdeclarative
          qt6.qtwebsockets
          kdePackages.poppler
        ];

      in
      with pkgs;
      {
        devShells.default = mkShell {
          inherit buildInputs;
          inherit nativeBuildInputs;
        };

      }
    );
}
