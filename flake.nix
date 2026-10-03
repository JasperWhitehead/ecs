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
            make build/libsds.a
          '';
          installPhase =
          ''
            mkdir -p $out/bin
            mkdir -p $lib/lib
            cp build/libsds.a $lib/lib

            mkdir -p $dev/include
            cp include/*.h $dev/include/
          '';
        };
      }
    );
  };
}
