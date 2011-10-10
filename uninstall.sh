#!/bin/bash

LOGFILE="install_log"

if [ ! -f $LOGFILE ]; then
  echo "logfile does not exist. cannot uninstall"
  exit 1
fi

echo -n "ready to remove installation? press ENTER to continue"
read

for f in `cat $LOGFILE`; do
  if [ ! -d $f ]; then # only remove if not a directory
    rm -f $f
  fi
done
rmdir $f # last entry is directory

echo "done"
