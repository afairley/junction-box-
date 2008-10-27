export JBHOME=`pwd`
export LD_LIBRARY_PATH=$JBHOME/lib/bin/graphviz/lib:$LD_LIBRARY_PATH
#echo `/sbin/ldconfig -v -n $JBHOME/lib/bin/graphviz/lib`
#echo $LD_LIBRARY_PATH
#echo `ldd randomDag`
$*