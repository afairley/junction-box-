#include "JB_Util.h"
/******************************************************************************
 ***** Linked
 ***** Lists
 ******************************************************************************/

ListNode *ListNode_Create(void *contents){
  ListNode *ret = (ListNode*)jballoc(sizeof(ListNode));
  ret->contents = contents;
  ret->next = NULL;
  return ret;
}

void ListNode_Destroy(ListNode *ln){
  free(ln);
}


List *List_Create(){
  List *ret = jballoc(sizeof(List));
  ret->first = NULL;
  ret->last = NULL;
  ret->size = 0;
  if(ret ==NULL){
    reportError( ERRSTRING(JB_Util.h, List_Create, ENOMEM) );
  }
  return ret;
}

void List_Destroy(List *this){
  ListNode *cur = this->first;
  while(cur !=NULL){
    ListNode *prev = cur;
    cur= cur->next;
    ListNode_Destroy(prev);
  }
  free(this);
}

/*Inserts c at the end of l.*/
void List_Append(List *l, void *c){
  ListNode *new = ListNode_Create(c);
  if(new == NULL)
    reportError( ERRSTRING(JB_Util.h, List_Append, ENOMEM) );
  if(l->size == 0)
    l->first = l->last = new;
  else{
    l->last->next = new;
    l->last = new;
  }
  (l->size)++;
}

/*Inserts c at the beginning of l.*/
void List_Prepend(List *l, void *c){
  ListNode *new = ListNode_Create(c);
  if(new == NULL)
    reportError( ERRSTRING(JB_Util.h, List_Prepend, ENOMEM) );
  if(l->size == 0)
    l->first = l->last = new;
  else{
    new->next = l->first;
    l->first = new;
  }
  (l->size)++;
}

char *List_ToString(List *this, char *(cntntPrinter)(ListNode *content)){
  char *ret;
  ListNode *stringItr, *listItr;;
  List *strings = List_Create();
  int stringLength = 0;

  for(listItr = this->first;
      listItr != NULL;
      listItr = listItr->next){
    List_Append(strings, cntntPrinter(listItr));
  }

  for(stringItr = strings->first;
      stringItr != NULL;
      stringItr = stringItr->next){
    stringLength += strlen( (char *)stringItr->contents);
  }

  stringLength += 1;//For null
  ret = jballoc(stringLength);
  ret[0] = (char)0;

  for(stringItr = strings->first;
      stringItr != NULL;
      stringItr = stringItr->next){
    char *curstr =(char *)stringItr->contents;
    strcat(ret,curstr);
  }

  return ret;
}
