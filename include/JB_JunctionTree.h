#ifndef H_JB_JT
#define H_JB_JT
#include "JB_Util.h"
#include "BitVector.h"
#include "JB_Graph.h"


//TODO take advantage of the fact that A subset B can be expressed as
//TODO A == A & B for A,B bitvector reps of sets
typedef struct elim_set ElimSet;
typedef struct elimset_collection ElimSetCollection;
typedef Graph ElimTree;
typedef struct vertex_ordering VertexOrdering;



struct vertex_ordering{
  int size;
  Vertex **vertices;
};

struct elim_set{
  BitVector *bv;
  VariableOrdering *o;
};

struct elimset_collection{
  int size;
  ElimSet **elim_sets;
  IntMap *remap;
};

VertexOrdering *VertexOrdering_Create(List *vIn);
void VertexOrdering_Destroy(VertexOrdering *this);
VertexOrdering *defaultVertexOrdering(Graph *g);
int VertexOrdering_GetVertexRank(VertexOrdering *this, Vertex *v);
int VertexOrdering_GetVertexRankByLabel(VertexOrdering *this, char *c);
char *VertexOrdering_ToString(VertexOrdering *this);

ElimSet *ElimSet_Create(VariableOrdering *o);
void ElimSet_Destroy(ElimSet *this);
void ElimSet_Add(ElimSet *this, Vertex *v);
int ElimSet_Contains(ElimSet *this, ElimSet *other);
int ElimSet_HighestVertex(ElimSet *this, VertexOrdering *o);
char *ElimSet_ToString(ElimSet *this);
Graph *MoralGraph(Graph *immoral);
ElimSetCollection *ElimSetCollection_Create(int size);
void ElimSetCollection_Add(ElimSetCollection *this,ElimSet *eset, int index);
char *ElimSetCollection_ToString(ElimSetCollection *this);
ElimSetCollection *GatherElimSets(Graph *moral, 
				  VertexOrdering *o,
				  VariableOrdering *o2);
void ElimSetCollection_RemoveNonCliques(ElimSetCollection *this, VertexOrdering *o);
ElimTree *ElimTree_Create(ElimSetCollection *esc);

#endif
