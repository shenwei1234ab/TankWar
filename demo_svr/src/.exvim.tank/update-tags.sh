#!/bin/bash
export DEST="./.exvim.tank"
export TOOLS="/home/leave/exVim/main//vimfiles/tools/"
export CTAGS_CMD="ctags"
export OPTIONS="--fields=+iaS --extra=+q"
export TMP="${DEST}/_tags"
export TARGET="${DEST}/tags"
sh ${TOOLS}/shell/bash/update-tags.sh
