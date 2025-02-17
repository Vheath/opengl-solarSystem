{
	description = "A basic flake with a shell";
	inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
	inputs.systems.url = "github:nix-systems/default";
	inputs.flake-utils = {
		url = "github:numtide/flake-utils";
		inputs.systems.follows = "systems";
	};
	
	outputs = { nixpkgs, flake-utils, ... }:
		flake-utils.lib.eachDefaultSystem (
		system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
		in
		{
			devShells.default = pkgs.mkShell {
				packages = [ pkgs.bashInteractive ];

				# in native - things that run on a machine in build-time
				nativeBuildInputs = with pkgs; [
					cmake 
					gcc
				];

				# in buildInputs - things that links with binary
				buildInputs = with pkgs; [
					mesa
					libGL
					glfw 
					glm
					libglvnd
				];
			
				LD_LIBRARY_PATH = ''
				${pkgs.lib.makeLibraryPath [
					pkgs.libglvnd
					pkgs.libGL
				]}:$LD_LIBRARY_PATH
				'';
			};
		}
	);
}

