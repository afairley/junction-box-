#include "JB_Util.h"

//private function prototypes
static DSetNode *DSetForest_FindSet(DSetForest *this, DSetNode *x);
static void DSetForest_Link(DSetForest *this, DSetNode *x, DSetNode *y);

DSetNode *DSetNode_Create(void *contents){
  DSetNode *ret = (DSetNode *)jballoc(sizeof(DSetNode));
  ret->parent = ret;
  ret->rank = 0;
  ret->contents = contents;
  return ret;
}
/* An object oriented implementation of disjoint set forests as
   outlined in CLR sec 21.3 */
DSetForest *DSetForest_Create(){
  DSetForest *ret = (DSetForest *)jballoc(sizeof(DSetForest));
  ret->lookup = VoidpMap_Create();
  return ret;
}

DSetNode *DSetForest_MakeSet(DSetForest *this, void *element){
  VoidpMap_Element *already = VoidpMap_Get(this->lookup, element);
  DSetNode *set ;
  if(already != NULL){
    set = (DSetNode*)(already->contents);
  }
  else{
    set = DSetNode_Create(element);
    VoidpMap_Add(this->lookup, element, set);
  }
  return set;
}
void DSetForest_Union(DSetForest *this, void *element1, void *element2){
  //TODO null checkage, error reporting
  DSetForest_Link(this, 
		  DSetForest_GetRepresentative(this,element1),
		  DSetForest_GetRepresentative(this,element2)
		  );
  return;
}
static void DSetForest_Link(DSetForest *this, DSetNode *x, DSetNode *y){
  if(x->rank > y->rank){
    y->parent = x;
  }
  else{
    x->parent = y;
    if(x->rank == y->rank){
      y->rank++;
    }
  }
}

DSetNode *DSetForest_GetRepresentative(DSetForest *this, void *element){
  DSetNode *x = (DSetNode*)(VoidpMap_Get(this->lookup, element)->contents);
  if(x != x->parent){
    x->parent = DSetForest_FindSet(this, x->parent);
  }
  return x->parent;
}

static DSetNode *DSetForest_FindSet(DSetForest *this, DSetNode *x){
  if(x != x->parent){
    x->parent = DSetForest_FindSet(this, x->parent);
  }
  return x->parent;
}
