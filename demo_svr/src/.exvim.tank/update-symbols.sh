#!/bin/bash
export DEST="./.exvim.tank"
export TOOLS="/home/leave/exVim/main//vimfiles/tools/"
export TMP="${DEST}/_symbols"
export TARGET="${DEST}/symbols"
sh ${TOOLS}/shell/bash/update-symbols.sh
