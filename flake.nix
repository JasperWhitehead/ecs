{
  description = "test flake";
  inputs =
  {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  };
  outputs = {self, nixpkgs, ...}@inputs: 
  let
    supported_systems = ["x86_64-linux"];
    for_supported_systems = nixpkgs.lib.genAttrs supported_systems;
  in
  {
    devShells = for_supported_systems
    (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        default = pkgs.mkShell
        {
          packages =
          [
            pkgs.gcc
          ];
        };
      }   
    );
    packages = for_supported_systems
    (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        default = pkgs.stdenv.mkDerivation
        {
          pname = "sds";
          version = "1.0";
          src = ./.;
          outputs =
          [
            "out"
            "lib"
            "dev"
          ];
          buildPhase =
          ''
            mkdir -p build
            gcc -Iinclude -c src/dynamic_array.c -o build/dynamic_array.o -Wno-free-nonheap-object
            gcc -Iinclude -c src/object_manager.c -o build/object_manager.o -Wno-free-nonheap-object
            ar rcs build/libsds.a build/dynamic_array.o build/object_manager.o
          '';
          installPhase =
          ''
            mkdir -p $out/bin
            mkdir -p $lib/lib
            cp build/libsds.a $lib/lib/

            mkdir -p $dev/include
            cp include/*.h $dev/include/
          '';
        };
      }
    );
  };
}
