{ pkgs }:

pkgs.writeShellScriptBin "build-docs" ''
  set -eu

  cat ./docs/docs_metadata.txt ./docs/DOCS.md > ./docs/tmp_docs.md
	  
  pandoc --from markdown --to html5 --standalone --toc --number-sections --no-highlight -o ./docs/output_docs.html ./docs/tmp_docs.md
  rm ./docs/tmp_docs.md

  echo 'Done building docs. See docs/output_docs.html'
''
