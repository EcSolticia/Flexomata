{
  description = "EcSolticia's development environment for Flexomata";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
  };

  outputs = { self , nixpkgs ,... }: let
    system = "x86_64-linux";
  in {
    devShells."${system}".default = let
      pkgs = import nixpkgs {
        inherit system;
      };
    in pkgs.mkShell {
      # create an environment with nodejs_18, pnpm, and yarn
      packages = with pkgs; [
	      gcc14
        cmake
        gdb
      ];

      shellHook = ''
	      echo 'Flexomata Dev Environment'
      '';
    };
  };
}
