{
  description = "Nexilis library for game server development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          name = "nexilis-devshell";
          nativeBuildInputs = [
            pkgs.cmake
            pkgs.gcc
            pkgs.boost
            pkgs.gtest
            (pkgs.python3.withPackages (ps: with ps; [
              python-dotenv
            ]))
          ];

          shellHook = ''
            export NEXILIS_PREFIX=${self.packages.${system}.lib}
            export LD_LIBRARY_PATH=$NEXILIS_PREFIX/lib:$LD_LIBRARY_PATH

            echo "Welcome to the Nexilis Dev Shell"
            echo "NEXILIS_PREFIX set to $NEXILIS_PREFIX"
            python ../scripts/src/env.py

            test-runner() {
                python ../scripts/src/test_runner.py "$@"
            }
            echo "Available test-runner commands:"
            echo "  test-runner --all      # Run all tests"
            echo "  test-runner --cpp      # Run C++ tests"
            echo "  nexilis_tests          # Run C++ tests directly"
          '';
        };

        packages = {
          lib = pkgs.stdenv.mkDerivation {
            pname = "nexilis-library";
            version = "0.0.1";
            src = ../nexilis;

            nativeBuildInputs = [ pkgs.cmake ];
            buildInputs = [ pkgs.boost ];

            configurePhase = ''
              mkdir -p build
              cd build
              cmake ..
            '';

            buildPhase = ''
              make
            '';

            installPhase = ''
              mkdir -p $out/lib $out/include
              cp libnexilis.so $out/lib/
              cp -r $src/include/* $out/include/
            '';
          };

          cpp-tests = pkgs.stdenv.mkDerivation {
            pname = "nexilis-tests";
            version = "0.0.1";
            src = ../tests/nexilis;

            nativeBuildInputs = [
              pkgs.cmake
              pkgs.patchelf
            ];

            buildInputs = [
              pkgs.boost
              pkgs.gtest
              self.packages.${system}.lib
            ];

            cmakeFlags = [
              "-DCMAKE_PREFIX_PATH=${self.packages.${system}.lib}"
            ];

            configurePhase = ''
              mkdir -p build
              cd build
              cmake ..
            '';

            buildPhase = "make";

            installPhase = ''
              mkdir -p $out/bin
              cp nexilis_tests $out/bin/
              patchelf --set-rpath ${self.packages.${system}.lib}/lib ./nexilis_tests
            '';
          };
      };
  });
}
