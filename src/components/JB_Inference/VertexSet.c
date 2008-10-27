#include "JB_Inference.h"
VertexSet *VertexSet_Create(VertexOrdering *o){
  VertexSet *ret = (VertexSet*)jballoc(sizeof(VertexSet));
  ret->bv = BitVector_Create(o->size, jballoc,jbfree, 0);
  ret->o = o;
  return ret;
}

void VertexSet_Destroy(VertexSet *this){
  BitVector_Destroy(this->bv);
  VertexOrdering_Destroy(this->o);
  jbfree(this);
}

char *VertexSet_GetLabel(VertexSet *this){
  int i, length = 0;
  char *ret;
  for(i = 0; i < BitVector_GetSize(this->bv,0); i++){
    if(BitVector_GetBit(this->bv, i))
      length += strlen( Vertex_GetLabel(this->o->vertices[i])  ) +1;
    //+1 for space following or NULL in the last case
  }
  ret = jballoc(length * sizeof(char) +1);//+1 for a trailing \n 
  //which will be put there by VertexSetCollection_ToString
  ret[0] = (char)0;
  for(i = 0; i < BitVector_GetSize(this->bv,0); i++){
    if(BitVector_GetBit(this->bv, i)){
      strcat(ret, Vertex_GetLabel(this->o->vertices[i]) );
      strcat(ret, " ");
    }
  }
  return ret;
}
//TODO unify nomenclature. ToString for all data structure
char *VertexSet_ToString(VertexSet *this){
 return VertexSet_GetLabel(this);
}

//Add a vertex to an elimination set
//Should have no effect if the vertex is already in the set
void VertexSet_Add(VertexSet *this, Vertex *v){
  int indx = VertexOrdering_GetVertexRank(this->o,v);
  if(indx >= 0){
    BitVector_SetBitOn(this->bv,indx,0); 
  }
}
//Returns nonzero if this contains v
int VertexSet_ContainsVertex(VertexSet *this, Vertex *v){
  int indx = VertexOrdering_GetVertexRank(this->o,v);
  char ret = 0;
  if(indx >= 0){
    ret = BitVector_GetBit(this->bv,indx); 
  }
  return ret;
  
}
/*Returns true if this contains other*/
int VertexSet_Contains(VertexSet *this, VertexSet *other){
  BitVector *test = BitVector_And(this->bv,other->bv,0);
  int ret = BitVector_Equals(other->bv,test,0);
  BitVector_Destroy(test);
  return ret;
}

VertexSet *VertexSet_Intersect(VertexSet *this, VertexSet *other){
  VertexSet *ret = (VertexSet*)jballoc(sizeof(VertexSet));
  ret->bv = BitVector_And(this->bv, other->bv, 0);
  ret->o = this->o;
  return ret;
}
