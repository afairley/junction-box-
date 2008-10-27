#include "JB_Util.h"
/******************************************************************************
 ***** IntMaps:
 ***** A simple mapping from ints to ints
 ******************************************************************************/
IntMap_Element *IntMap_Element_Create(int id, int contents){
  IntMap_Element *ret =   (IntMap_Element *)jballoc(sizeof(IntMap_Element));
  ret->id = id;
  ret->contents = contents;
  ret->next = NULL;
  return ret;
}

IntMap *IntMap_Create(){
  IntMap *ret = malloc(sizeof(IntMap));
  ret->first = NULL;
  ret->size = 0;
  return ret;
}
IntMap_Element *IntMap_Get(IntMap *this, int id ){
  IntMap_Element *me =NULL;
  for(me = this->first;
      me != NULL;
      me = me->next){
    if(me->id == id)
      break;
  }
  return me;//if not found me = NULL
}
void IntMap_Add(IntMap *this, int id, int contents){
  IntMap_Element *already = IntMap_Get(this, id);
  if(already != NULL)
    return;
  else{
    already = IntMap_Element_Create(id,contents);
    already->next = this->first;
    this->first =already;
    this->size++;
    return;
  }
}

int IntMap_Size( IntMap *this){
  return this->size;
}

char *IntMap_ToString(IntMap *this, char *(cntntPrinter)(IntMap_Element *content)){
  char *ret;
  IntMap_Element *curME;
  ListNode *curLN;
  List *strings = List_Create();
  int stringLength = 0;

  for(curME = this->first;
      curME != NULL;
      curME = (IntMap_Element*)curME->next){
    List_Append(strings, cntntPrinter(curME));
  }

  for(curLN = strings->first;
      curLN != NULL;
      curLN = curLN->next){
    stringLength += strlen( (char *)curLN->contents);
  }
  stringLength += 1;//For null
  ret = jballoc(stringLength);
  for(curLN = strings->first;
      curLN != NULL;
      curLN = curLN->next){
    char *curstr =(char *)curLN->contents;
    strcat(ret,curstr);
  }
  return ret;
}

IntMap_Element *IntMap_GetFirst_Element(IntMap *this){
  return this->first;
}

IntMap_Element *IntMap_Element_GetNext_Element(IntMap_Element *me){
  return me->next;
}
