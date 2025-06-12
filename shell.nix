{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  name = "opencv-cpp-env";
  buildInputs = with pkgs; [
    pkg-config
    stdenv.cc
    opencv4
  ];
  shellHook = ''
    export PKG_CONFIG_PATH=${pkgs.opencv4}/lib/pkgconfig:$PKG_CONFIG_PATH
    export CPLUS_INCLUDE_PATH=${pkgs.opencv4}/include/opencv4:$CPLUS_INCLUDE_PATH
    zsh
  '';
}
