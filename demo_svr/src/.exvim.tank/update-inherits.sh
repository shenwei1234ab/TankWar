#!/bin/bash
export DEST="./.exvim.tank"
export TOOLS="/home/leave/exVim/main//vimfiles/tools/"
export TMP="${DEST}/_inherits"
export TARGET="${DEST}/inherits"
sh ${TOOLS}/shell/bash/update-inherits.sh
