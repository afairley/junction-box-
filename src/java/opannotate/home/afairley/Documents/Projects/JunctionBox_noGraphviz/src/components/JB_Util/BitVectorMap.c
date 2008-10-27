               :#include "JB_Util.h"
               :/******************************************************************************
               : ***** BitVectorMaps:
               : ***** A simple mapping from BitVectors to void pointers
               : ******************************************************************************/
               :
               :BitVectorMap_Element *BitVectorMap_Element_Create(void  *id, void *contents){
               :  BitVectorMap_Element *ret =   (BitVectorMap_Element *)jballoc(sizeof(BitVectorMap_Element));
               :  ret->id = id;
               :  ret->contents = contents;
               :  ret->next = NULL;
               :  return ret;
               :}
               :
               :BitVectorMap *BitVectorMap_Create(){
               :  BitVectorMap *ret = malloc(sizeof(BitVectorMap));
               :  ret->first = NULL;
               :  ret->size = 0;
               :  return ret;
               :}
               :BitVectorMap_Element *BitVectorMap_Get(BitVectorMap *this, BitVector *id ){ /* BitVectorMap_Get total:      1  0.0824 */
               :  BitVectorMap_Element *me =NULL;
     1  0.0824 :  for(me = this->first;
               :      me != NULL;
               :      me = me->next){
               :    if(BitVector_Equals(me->id,id,0))
               :      break;
               :  }
               :  return me;//if not found me = NULL
               :}
               :void BitVectorMap_Add(BitVectorMap *this, BitVector *id, void *contents){
               :  BitVectorMap_Element *already = BitVectorMap_Get(this, id);
               :  if(already != NULL)
               :    return;
               :  else{
               :    already = BitVectorMap_Element_Create(id,contents);
               :    already->next = this->first;
               :    this->first =already;
               :    this->size++;
               :    return;
               :  }
               :}
               :
               :int BitVectorMap_Size( BitVectorMap *this){
               :  return this->size;
               :}
               :
               :char *BitVectorMap_ToString(BitVectorMap *this, char *(cntntPrinter)(BitVectorMap_Element *content)){
               :  char *ret;
               :  BitVectorMap_Element *curME;
               :  ListNode *curLN;
               :  List *strings = List_Create();
               :  int stringLength = 0;
               :
               :  for(curME = this->first;
               :      curME != NULL;
               :      curME = (BitVectorMap_Element*)curME->next){
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
               :BitVectorMap_Element *BitVectorMap_GetFirst_Element(BitVectorMap *this){
               :  return this->first;
               :}
               :
               :BitVectorMap_Element *BitVectorMap_Element_GetNext_Element(BitVectorMap_Element *me){
               :  return me->next;
               :}
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_Util/BitVectorMap.c"
 * 
 *      1  0.0824
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
