#!/bin/bash
function usage {
  echo "this script installs this package into some library folder (headers and shared objects)"
  echo "please make sure that the specified path exists and you have write permissions"
  echo "usage: $1 <install-location>"
  echo "e.g. : $1 $HOME/lib"
  echo "e.g. : sudo $1 /usr/local"
}

# check parameters
if [[ $# -ne 1  ||  "$1" == "-h"  ||  "$1" == "-help"  ||  "$1" == "--help" ]]; then
  usage $0
  exit 0
fi

PREFIX="$1"

if [ ! -d $PREFIX ]; then
  echo "ERROR: $PREFIX does not exist"
  exit 1
fi

if [ ! -d $PREFIX/include ]; then
  echo "ERROR: $PREFIX/include does not exist"
  exit 2
fi

if [ ! -d $PREFIX/lib ]; then
  echo "ERROR: $PREFIX/lib does not exist"
  exit 3
fi

# at this point everything seems ok --> start installing

echo -n "start installing into $PREFIX. press ENTER to continue"
read

#HEADERS="graphics.hpp  Gui.hpp  MainWindow.hpp  MNavWidget.hpp  spline.hpp  VisualizerCamControl.hpp  VisualizerGrid.hpp  Visualizer.hpp"
HEADERS=`ls *.hpp`
LIBS=`ls lib*.so*`
CWD=`pwd`
INCSUBDIR="Gui3DQt"
LOGFILE="install_log"

rm -f $LOGFILE # remove existing logfile

for lib in $LIBS; do
  echo "installing $lib"
  ln -sf $CWD/$lib $PREFIX/lib/
  echo $PREFIX/lib/$lib >> $LOGFILE
done

mkdir -p $PREFIX/include/$INCSUBDIR
for header in $HEADERS; do
  echo "installing $header"
  ln -sf $CWD/$header $PREFIX/include/$INCSUBDIR/
  echo $PREFIX/include/$INCSUBDIR/$header >> $LOGFILE
done
echo "$PREFIX/include/$INCSUBDIR" >> $LOGFILE

echo "done"
