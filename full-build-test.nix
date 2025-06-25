{ pkgs }:

pkgs.writeShellScriptBin "full-build-test" ''
  rm -rf build
  
  cmake -S . -B build
  cmake --build build
  
  echo "Built Library"

  for example in $(ls ./examples)
  do
    example_dir="./examples/''${example}"
    example_build_dir="''${example_dir}/build"

    cmake -S "''${example_dir}" -B "''${example_build_dir}"
    cmake --build "''${example_build_dir}"
  done

  echo "Built all executables"
''