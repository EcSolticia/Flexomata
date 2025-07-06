{ pkgs }:

pkgs.writeShellScriptBin "full-build-test" ''
  set -eu

  rm -rf build
  
  cmake -S . -B build
  cmake --build build
  
  echo "Built Library"

  for example in $(ls ./examples)
  do
    example_dir="./examples/''${example}"
    example_build_dir="''${example_dir}/build"

    rm -rf "''${example_build_dir}"

    cmake -S "''${example_dir}" -B "''${example_build_dir}"
    cmake --build "''${example_build_dir}"
  done

  echo "Built all executables"
''
