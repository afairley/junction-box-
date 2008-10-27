               :#include "JB_Util.h"
               :/******************************************************************************
               : ***** Memory Management
               : ******************************************************************************/
               :/*Wrap malloc so as to avoid tedious null checks everywhere, and provide
               :  a central point of failure*/
     2  0.1647 :void *jballoc(size_t amt){ /* jballoc total:      7  0.5766 */
     3  0.2471 :  void *ret = malloc(amt);
     2  0.1647 :  if(ret == NULL){
               :    reportError("Out of memory");
               :  }
               :  return ret;
               :}
               :
               :/*Wrap free. Maybe add ref counting? */
     1  0.0824 :void jbfree(void *p){ /* jbfree total:      4  0.3295 */
     1  0.0824 :  free(p);
     2  0.1647 :}
               :
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_Util/JB_Mem.c"
 * 
 *     11  0.9061
 */


/* 
 * Command line: opannotate --source -o opannotate/ libjunctionbox.so 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: Core 2, speed 2200 MHz (estimated)
 * Counted CPU_CLK_UNHALTED events (Clock cycles when not halted) with a unit mask of 0x00 (Unhalted core cycles) count 100000
 */
