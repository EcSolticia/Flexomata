{
  description = "EcSolticia's development environment for Flexomata";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
  };

  outputs = { self , nixpkgs ,... }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };

    fullBuildTestScript = import ./full-build-test.nix { inherit pkgs; };

  in {
    packages.${system}.full-build-test = fullBuildTestScript;

    devShells."${system}".default = pkgs.mkShell {
      
      packages = with pkgs; [
        gcc14
        cmake
        gdb

        fullBuildTestScript
      ];

      shellHook = ''
        echo 'Flexomata Dev Environment'
      '';

    };

    };

}
