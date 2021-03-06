#include"JB_Model.h"
CPT *CPT_Create(Variable **parents, int numParents, 
		Variable *tableFor, Pot *pot){
  CPT *ret = jballoc(sizeof(CPT));
  ret->parents = parents;
  ret->numParents = numParents;
  ret->tableFor = tableFor;
  ret->allvbls = pot->vbls;
  ret->pot = pot;
  
  return ret;
}

CPT *CPT_Create_ParentList(List *parentList, 
		Variable *tableFor, Pot *pot){
  CPT *ret = jballoc(sizeof(CPT));
  if(parentList != NULL){
    ret->parents =jballoc(sizeof(Variable*)*parentList->size);
    ret->numParents = parentList->size;
    ListNode *cur = parentList->first;
    int i = 0;
    while(cur != NULL){
      ret->parents[i++] = (Variable*)cur->contents;
      cur = cur->next;
    }
  }/*TODO: ELSE?*/
  
  ret->tableFor = tableFor;
  ret->allvbls = pot->vbls;
  ret->pot = pot;
  return ret;
}

Variable *CPT_GetIthParent(CPT *this, int i){
  //TODO could add a bounds check here...
  return this->parents[i];
}
void CPT_Destroy(CPT *this){
  //TODO Revisit memory management of CPT when end-to-end is done
  jbfree(this);
}

Observation *Observation_Create(VariableSet *vset, int value){
  
  Variable *v = VariableSet_GetFstVbl(vset);
  int vblSize = Variable_GetCardinality(v);
  if(value > vblSize || value < 0){
    //handle illegal values
    reportError("Observation provided with value exceeding variable's cardinality");
    return NULL;
  }

  //initialize potential values
  double *potVals = (double*)jballoc(sizeof(double)*vblSize);
  int i;
  for(i = 0; i < vblSize; i++){
    if(i == value)
      potVals[i] = 1.0;
    else
      potVals[i] = 0;
  }
  Pot *pot = Pot_CreateTabular(String_Create(Variable_GetLabel(v)),
				     vset,
				     potVals);
  //fill in returned struct
  Observation *ret = (Observation*)jballoc(sizeof(Observation));
  ret->v    = v;
  ret->pot  = pot;
  ret->value= value; 
  return ret;
}
Observation *Observation_CreateLog(VariableSet *vset, int value){
  
  Variable *v = VariableSet_GetFstVbl(vset);
  int vblSize = Variable_GetCardinality(v);
  if(value > vblSize || value < 0){
    //handle illegal values
    reportError("Observation provided with value exceeding variable's cardinality");
    return NULL;
  }

  //initialize potential values
  double *potVals = (double*)jballoc(sizeof(double)*vblSize);
  int i;
  for(i = 0; i < vblSize; i++){
    if(i == value)
      potVals[i] = log(1.0);
    else
      potVals[i] = log(0);
  }
  Pot *pot = Pot_CreateTabular(String_Create(Variable_GetLabel(v)),
				     vset,
				     potVals);
  //fill in returned struct
  Observation *ret = (Observation*)jballoc(sizeof(Observation));
  ret->v    = v;
  ret->pot  = pot;
  ret->value= value; 
  return ret;
}

void Observation_Destroy(Observation *this){
  Pot_DestroyTabular(this->pot); 
  jbfree(this);
}

BNet *BNet_Create(VariableOrdering *vbls, 
		  CPT **tables, 
		  int numTables){
  int i,j;
  BNet *ret = jballoc(sizeof(BNet));
  ret->vbls = vbls;
  ret->tables = tables;
  ret->numTables =numTables;
  /*Set up the BNet Graph*/
  ret->initialGraph = Graph_Create("BNet",TRUE);
  Graph *g = ret->initialGraph;
  for(i = 0; i < vbls->size; i++){
    Variable *v = (vbls->vbls)[i];
    Graph_AddVertex(g, Variable_GetLabel(v) );
  }
  for(i = 0; i < numTables; i++){
    CPT *curTab = tables[i];
    Variable *child = curTab->tableFor;
    Vertex *childV = Graph_GetVertex(g, Variable_GetLabel(child));
    for(j =0; j < curTab->numParents; j++){
      Variable *parent = CPT_GetIthParent(curTab,j);
      Vertex *parentV = Graph_GetVertex(g, Variable_GetLabel(parent));
      Graph_AddEdge(g,parentV,childV);
    }
  }
  return ret;
}
BNet *BNet_Create_List(VariableOrdering *vbls, 
		  List *tables){
  int i,j;
  BNet *ret      = jballoc(sizeof(BNet));
  ret->vbls      = vbls;

  //initialize tables;
  ret->tables    = (CPT**)jballoc(sizeof(CPT*)*tables->size);
  ret->numTables = tables->size;
  ret->numObs = 0;
  ListNode *cur = tables->first;
  i = 0;
  while(cur != NULL){
    ret->tables[i++] = (CPT*)cur->contents;
    cur = cur->next;
  }

  /*Set up the BNet Graph*/
  ret->initialGraph = Graph_Create("BNet",TRUE);
  Graph *g = ret->initialGraph;
  //add vertices
  for(i = 0; i < vbls->size; i++){
    Variable *v = (vbls->vbls)[i];
    Graph_AddVertex(g, Variable_GetLabel(v) );
  }

  //add edges
  for(i = 0; i < ret->numTables; i++){
    CPT *curTab = ret->tables[i];
    Variable *child = curTab->tableFor;
    Vertex *childV = Graph_GetVertex(g, Variable_GetLabel(child));
    for(j =0; j < curTab->numParents; j++){
      Variable *parent = CPT_GetIthParent(curTab,j);
      Vertex *parentV = Graph_GetVertex(g, Variable_GetLabel(parent));
      Graph_AddEdge(g,parentV,childV);
    }
  }
  return ret;
}

BNet *BNet_Create_List2(VariableOrdering *vbls, 
			List *tables,
			List *obs){
  int i,j;
  BNet *ret      = jballoc(sizeof(BNet));
  ret->vbls      = vbls;

  //initialize tables;
  ret->tables    = (CPT**)jballoc(sizeof(CPT*)*tables->size);
  ret->numTables = tables->size;
  ListNode *cur = tables->first;
  i = 0;
  while(cur != NULL){
    ret->tables[i++] = (CPT*)cur->contents;
    cur = cur->next;
  }
  //initialize observations
  ret->obs       = (Observation**)jballoc(
			 sizeof(Observation*)*obs->size);
  ret->numObs    = obs->size;

  i = 0;
  cur = obs->first;
  while(cur != NULL){
    ret->obs[i++] = (Observation*)cur->contents;
    cur = cur->next;
  }
  /*Set up the BNet Graph*/
  ret->initialGraph = Graph_Create("BNet",TRUE);
  Graph *g = ret->initialGraph;
  //add vertices
  for(i = 0; i < vbls->size; i++){
    Variable *v = (vbls->vbls)[i];
    Graph_AddVertex(g, Variable_GetLabel(v) );
  }

  //add edges
  for(i = 0; i < ret->numTables; i++){
    CPT *curTab = ret->tables[i];
    Variable *child = curTab->tableFor;
    Vertex *childV = Graph_GetVertex(g, Variable_GetLabel(child));
    for(j =0; j < curTab->numParents; j++){
      Variable *parent = CPT_GetIthParent(curTab,j);
      Vertex *parentV = Graph_GetVertex(g, Variable_GetLabel(parent));
      Graph_AddEdge(g,parentV,childV);
    }
  }
  return ret;
}

void BNet_Destory(BNet *this){
  int i;
  Graph_Destroy(this->initialGraph);
  //TODO Free ElimTree, cliques, seperators, 
  for(i = 0; i < this->numTables; i++)
    CPT_Destroy(this->tables[i]);
  jbfree(this);
}
