{
  description = "Rollercoaster Revolution 99 - dev environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { nixpkgs, ... }: let
    forAllSystems = nixpkgs.lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
      "x86_64-darwin"
      "aarch64-darwin"
    ];
  in {
    devShells = forAllSystems (system: let
      pkgs = import nixpkgs { inherit system; };
    in {
      default = pkgs.mkShell {
        packages = with pkgs; [
          cmake
          gcc
          SDL2
          SDL2_image
          SDL2_mixer
          SDL2_ttf
          sqlite
          clang-tools
        ];

        shellHook = ''
          echo "🎢 Rollercoaster Revolution 99 dev shell"
          echo ""
          echo "Build:"
          echo "  cmake -B build -DCMAKE_BUILD_TYPE=Release"
          echo "  cmake --build build"
          echo ""
          echo "Run:"
          echo "  ./build/RollercoasterRevolution99"
        '';
      };
    });
  };
}
