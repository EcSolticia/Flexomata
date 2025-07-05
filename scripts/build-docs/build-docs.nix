{ pkgs }:

pkgs.writeShellScriptBin "build-docs" ''
  set -eu

  cat ./docs/docs_metadata.txt ./DOCS.md > ./tmp_docs.md
	  
  pandoc --from markdown --to html5 --standalone --toc --number-sections --no-highlight -o ./docs/output_docs.html ./tmp_docs.md
  rm ./tmp_docs.md
''
