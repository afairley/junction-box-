               :#include "JB_Util.h"
               :/******************************************************************************
               : ***** Maps:
               : ***** Provides a  mapping from strings to pointers,
               : ******************************************************************************/
               :Map_Element *Map_Element_Create(char *id, void *contents){
               :  Map_Element *ret =   (Map_Element *)jballoc(sizeof(Map_Element));
               :  ret->id = id;
               :  ret->contents = contents;
               :  ret->next = NULL;
               :  return ret;
               :}
               :
               :Map *Map_Create(){
               :  Map *ret = malloc(sizeof(Map));
               :  ret->first = NULL;
               :  ret->size = 0;
               :  return ret;
               :}
     1  0.0824 :Map_Element *Map_Get(Map *this, char *id ){ /* Map_Get total:     14  1.1532 */
               :  Map_Element *me =NULL;
     1  0.0824 :  for(me = this->first;
     4  0.3295 :      me != NULL;
     1  0.0824 :      me = me->next){
     7  0.5766 :    if(strcmp(me->id, id) == 0)
               :      break;
               :  }
               :  return me;//if not found me = NULL
               :}
     1  0.0824 :void Map_Add(Map *this, Map_Element *me){ /* Map_Add total:      1  0.0824 */
               :  Map_Element *already = Map_Get(this, me->id);
               :  if(already != NULL)
               :    return;
               :  else{
               :    me->next = this->first;
               :    this->first = me;
               :    this->size++;
               :    return;
               :  }
               :}
               :
               :int Map_Size( Map *this){ /* Map_Size total:      1  0.0824 */
               :  return this->size;
     1  0.0824 :}
               :
               :char *Map_ToString(Map *this, char *(cntntPrinter)(Map_Element *content)){
               :  char *ret;
               :  Map_Element *curME;
               :  ListNode *curLN;
               :  List *strings = List_Create();
               :  int stringLength = 0;
               :
               :  for(curME = this->first;
               :      curME != NULL;
               :      curME = (Map_Element*)curME->next){
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
               :/*void Map_Delete(Map *this , Map_Element *me){
               :  HASH_DEL( this->hash, me);
               :}
               :
               :void Map_SortBy(Map *this,int (*cmp)()){
               :  HASH_SORT( this->hash, cmp);
               :  } */
               :
               :Map_Element *Map_GetFirst_Element(Map *this){
               :  return this->first;
               :}
               :
               :Map_Element *Map_Element_GetNext_Element(Map_Element *me){
               :  return me->next;
               :}
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_Util/Map.c"
 * 
 *     16  1.3180
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
