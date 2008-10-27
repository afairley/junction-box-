#ifndef H_JB_MODEL
#define H_JB_MODEL
#include "JB_Util.h"
#include "JB_Graph.h"
#include "JB_Pot.h"

typedef struct conditional_probability_table CPT;
typedef struct observation Observation;
typedef struct bayes_net BNet;

struct bayes_net{
  VariableOrdering *vbls;
  CPT **tables;
  int numTables;
  Observation **obs;
  int numObs;
  Graph *initialGraph;
  Graph *moral;
  ElimTree *elimTree;
  ElimSetCollection *elimCliques;
  Pot ***cliquePots;
  int numCliques;
  Pot ***seperatorPots;
  int numSeps;
};

struct conditional_probability_table{
  Variable **parents;
  int numParents;
  Variable *tableFor;
  VariableSet *allvbls;
  Pot *pot;
};

struct observation{
  Variable *v;
  Pot *pot;
  int value;
};

CPT *CPT_Create(Variable **parents, int numParents,
		Variable *tableFor, Pot *pot);
CPT *CPT_Create_ParentList(List *parentList, 
		Variable *tableFor, Pot *pot);
void CPT_Destroy(CPT *this);

Observation *Observation_Create(VariableSet *v, int value);
Observation *Observation_CreateLog(VariableSet *v, int value);
void Observation_Destroy(Observation *this);

BNet *BNet_Create(VariableOrdering *vbls, 
		  CPT **tables, 
		  int numTables);
BNet *BNet_Create_List(VariableOrdering *vbls, 
		       List *tables);
BNet *BNet_Create_List2(VariableOrdering *vbls, 
			List *tables,
			List *obs);
void BNet_Destory(BNet *this);
#endif
