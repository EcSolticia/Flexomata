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

    fullBuildTestScript = import ./scripts/full-build-test.nix { inherit pkgs; };
    buildDocsScript = import ./scripts/build-docs.nix { inherit pkgs; };

  in {
    devShells."${system}".default = pkgs.mkShell {
      
      packages = with pkgs; [
        gcc14
        cmake
        gdb

	      buildDocsScript
        fullBuildTestScript
      ];

      shellHook = ''
        echo 'Flexomata Dev Environment'
      '';

    };

  };

}
