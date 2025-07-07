{ pkgs }:

pkgs.writeShellScriptBin "build-docs" ''
  set -eu

  cat ./docs/docs_prepend.txt ./docs/DOCS.md ./docs/docs_append.txt > ./docs/tmp_docs.md
	  
  pandoc --from markdown --to html5 --standalone --css=docs_style.css --toc --number-sections --highlight-style=zenburn -o ./docs/output_docs.html ./docs/tmp_docs.md
  rm ./docs/tmp_docs.md

  echo 'Done building docs. See docs/output_docs.html'
''
