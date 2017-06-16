#!/bin/bash
export DEST="./.exvim.tank"
export TOOLS="/home/leave/exVim/main//vimfiles/tools/"
export TMP="${DEST}/_ID"
export TARGET="${DEST}/ID"
sh ${TOOLS}/shell/bash/update-idutils.sh
