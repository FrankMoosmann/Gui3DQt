Compilation
===========
1) auto-create Makefile by "qmake-qt4 example1.pro"
2) compile with "Make"

Run example
===========
shared libs of Gui must be within LD_LIBRARY_PATH
temporarily add it by 
# export LD_LIBRARY_PATH="../:$LD_LIBRARY_PATH"
from within the example directory
then it should be possible to run the example with
# ./example

