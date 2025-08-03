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
        cmake = pkgs.cmake;
        stdenv = pkgs.stdenv;
        gcc = pkgs.gcc;
        boost = pkgs.boost;
        gtest = pkgs.gtest;
        lib = pkgs.lib;
        patchelf = pkgs.patchelf;
        python = pkgs.python3;
        pythonEnv = pkgs.python3.withPackages (ps: with ps; [
          python-dotenv
        ]);
      in
      {
        devShells.default = pkgs.mkShell {
          name = "nexilis-devshell";
          nativeBuildInputs = [
            cmake
            gcc
            boost
            gtest
            python
            pythonEnv
          ];

          shellHook = ''
            export NEXILIS_PREFIX=${self.packages.${system}.lib}
            export LD_LIBRARY_PATH=$NEXILIS_PREFIX/lib:$LD_LIBRARY_PATH
            echo "Welcome to the Nexilis Dev Shell"
            echo "NEXILIS_PREFIX set to $NEXILIS_PREFIX"
          '';
        };

        packages = {
          lib = pkgs.stdenv.mkDerivation {
            pname = "nexilis-library";
            version = "0.0.1";
            src = ../nexilis;

            nativeBuildInputs = [ cmake ];
            buildInputs = [ boost ];

            configurePhase = ''
              mkdir -p build
              cd build
              cmake .. -DBUILD_SHARED_LIBS=ON
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

            nativeBuildInputs = [ cmake patchelf ];
            buildInputs = [
              boost
              gtest
              self.packages.${system}.lib
            ];

            CMAKE_PREFIX_PATH = self.packages.${system}.lib;

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
