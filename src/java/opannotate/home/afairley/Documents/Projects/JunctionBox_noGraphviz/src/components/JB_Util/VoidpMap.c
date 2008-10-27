               :#include "JB_Util.h"
               :/******************************************************************************
               : ***** VoidpMaps:
               : ***** A simple mapping from voidps to voidps
               : ******************************************************************************/
               :VoidpMap_Element *VoidpMap_Element_Create(void  *id, void *contents){
               :  VoidpMap_Element *ret =   (VoidpMap_Element *)jballoc(sizeof(VoidpMap_Element));
               :  ret->id = id;
               :  ret->contents = contents;
               :  ret->next = NULL;
               :  return ret;
               :}
               :
               :VoidpMap *VoidpMap_Create(){
               :  VoidpMap *ret = malloc(sizeof(VoidpMap));
               :  ret->first = NULL;
               :  ret->size = 0;
               :  return ret;
               :}
               :VoidpMap_Element *VoidpMap_Get(VoidpMap *this, void *id ){ /* VoidpMap_Get total:      2  0.1647 */
               :  VoidpMap_Element *me =NULL;
               :  for(me = this->first;
     1  0.0824 :      me != NULL;
               :      me = me->next){
     1  0.0824 :    if(me->id == id)
               :      break;
               :  }
               :  return me;//if not found me = NULL
               :}
               :void VoidpMap_Add(VoidpMap *this, void *id, void *contents){
               :  VoidpMap_Element *already = VoidpMap_Get(this, id);
               :  if(already != NULL)
               :    return;
               :  else{
               :    already = VoidpMap_Element_Create(id,contents);
               :    already->next = this->first;
               :    this->first =already;
               :    this->size++;
               :    return;
               :  }
               :}
               :
               :int VoidpMap_Size( VoidpMap *this){
               :  return this->size;
               :}
               :
               :char *VoidpMap_ToString(VoidpMap *this, char *(cntntPrinter)(VoidpMap_Element *content)){
               :  char *ret;
               :  VoidpMap_Element *curME;
               :  ListNode *curLN;
               :  List *strings = List_Create();
               :  int stringLength = 0;
               :
               :  for(curME = this->first;
               :      curME != NULL;
               :      curME = (VoidpMap_Element*)curME->next){
               :    List_Append(strings, cntntPrinter(curME));
               :  }
               :
               :  for(curLN = strings->first;
               :      curLN != NULL;
               :      curLN = curLN->next){
               :    stringLength += strlen( (char *)curLN->contents);
               :  }
               :  stringLength += 1;//For null
               :  ret = jballoc(stringLength);
               :  for(curLN = strings->first;
               :      curLN != NULL;
               :      curLN = curLN->next){
               :    char *curstr =(char *)curLN->contents;
               :    strcat(ret,curstr);
               :  }
               :  return ret;
               :}
               :
               :VoidpMap_Element *VoidpMap_GetFirst_Element(VoidpMap *this){
               :  return this->first;
               :}
               :
               :VoidpMap_Element *VoidpMap_Element_GetNext_Element(VoidpMap_Element *me){
               :  return me->next;
               :}
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_Util/VoidpMap.c"
 * 
 *      2  0.1647
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
