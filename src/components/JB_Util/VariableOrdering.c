#include <JB_Util.h>
/******************************************************************************
 ***** VariableOrdering
 ******************************************************************************/

/* Pass in a list ordering vertices from lowest to highest, 
 * get back an array backed  collection with the same ordering*/
VariableOrdering *VariableOrdering_Create(List *vIn){
  int i;
  ListNode *vInItr;
  VariableOrdering *ret = (VariableOrdering*)jballoc(sizeof(VariableOrdering));
  ret->vbls =(Variable**)jballoc( sizeof(Variable*) * vIn->size);
  ret->size = vIn->size;
  
  for(i =0, vInItr = vIn->first ;
      i < ret->size && vInItr != NULL;
      i++, vInItr = vInItr->next){
    ret->vbls[i] = vInItr->contents;
  }
  if(i < ret->size)
    reportError(" Lists aren't tracking their size properly.");
  return ret;
}

char *VariableOrdering_ToString(VariableOrdering *this){
  int length =0;
  int i;
  char *ret;
  for (i =0 ; i < this->size; i++){
    length += strlen(Variable_GetLabel(this->vbls[i])) +1/*following
							     space*/;
  }
  ret = jballoc(length*sizeof(char) +1);
  ret[0]=(char)0;
  for (i =0 ; i < this->size; i++){
    strcat(ret,Variable_GetLabel(this->vbls[i]));
    strcat(ret, " ");
  }
  return ret;
}

//Very slowly(linear search) returns the rank of a variable in an ordering.
//Can't binary search, as the ordering is specified arbitrarily
//could maintain a lookup table
int VariableOrdering_GetVariableRank(VariableOrdering *this, Variable *v){
  int ret;
  for(ret = 0; ret < this->size; ret++){
    if(this->vbls[ret] == v)
       return ret;
  }
  return -1;
}

//Very slowly(linear search) returns the rank of a variable in an ordering.
//Can't binary search, as the ordering is specified arbitrarily
//could maintain a lookup table
int VariableOrdering_GetVariableRankByLabel(VariableOrdering *this,
					    char *label){
  int ret;
  for(ret = 0; ret < this->size; ret++){
    if( strcmp(Variable_GetLabel(this->vbls[ret]),label) == 0 )
       return ret;
  }
  return -1;
}
