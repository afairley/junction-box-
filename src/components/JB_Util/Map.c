#include "JB_Util.h"
/******************************************************************************
 ***** Maps:
 ***** Provides a  mapping from strings to pointers,
 ******************************************************************************/
Map_Element *Map_Element_Create(char *id, void *contents){
  Map_Element *ret =   (Map_Element *)jballoc(sizeof(Map_Element));
  ret->id = id;
  ret->contents = contents;
  ret->next = NULL;
  return ret;
}

Map *Map_Create(){
  Map *ret = malloc(sizeof(Map));
  ret->first = NULL;
  ret->size = 0;
  return ret;
}
Map_Element *Map_Get(Map *this, char *id ){
  Map_Element *me =NULL;
  for(me = this->first;
      me != NULL;
      me = me->next){
    if(strcmp(me->id, id) == 0)
      break;
  }
  return me;//if not found me = NULL
}
void Map_Add(Map *this, Map_Element *me){
  Map_Element *already = Map_Get(this, me->id);
  if(already != NULL)
    return;
  else{
    me->next = this->first;
    this->first = me;
    this->size++;
    return;
  }
}

int Map_Size( Map *this){
  return this->size;
}

char *Map_ToString(Map *this, char *(cntntPrinter)(Map_Element *content)){
  char *ret;
  Map_Element *curME;
  ListNode *curLN;
  List *strings = List_Create();
  int stringLength = 0;

  for(curME = this->first;
      curME != NULL;
      curME = (Map_Element*)curME->next){
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

/*void Map_Delete(Map *this , Map_Element *me){
  HASH_DEL( this->hash, me);
}

void Map_SortBy(Map *this,int (*cmp)()){
  HASH_SORT( this->hash, cmp);
  } */

Map_Element *Map_GetFirst_Element(Map *this){
  return this->first;
}

Map_Element *Map_Element_GetNext_Element(Map_Element *me){
  return me->next;
}
