#include"JB_Inference.h"

/*Compare vertices first on total degree, then break ties *
 * by comparing cardinalities of associated variable      */
int vtx_SSW_cmp( const void *v1, const void *v2){
  Vertex *vtx1 = *(Vertex **)v1;
  Vertex *vtx2 = *(Vertex **)v2;
  //vtx2 - vtx1 because I followed Cowell et. al. 's strange backwards
  //elimination algorithm
  int ret = Vertex_Totdegree(vtx2) - Vertex_Totdegree(vtx1);
  if(ret == 0)
    ret = ( (Variable*)vtx2->variable )->cardinality - 
      ( (Variable*)vtx1->variable )->cardinality;
  return ret;
}

VertexOrdering *BNet_StateSpaceWeightedElimOrdering(BNet *this){
  VertexOrdering *ret = (VertexOrdering*)jballoc(sizeof(VertexOrdering));

  Graph *g = this->initialGraph;
  int i, numVertices = Graph_GetNumVertices(g);
  ret->size = numVertices;
						
  Vertex **vertices = jballoc(sizeof(Vertex*) * numVertices);
  //TODO Iterating through vertices of a graph is inefficent as impl.
  //TODO(cont.) for now, I just go into the guts to do things quickly
  //TODO(cont.) This should be made unecessary
  VertexMap *vmap = g->vertices;
  Map_Element *itr = vmap->first;
  for(i=0; itr  != NULL; itr = itr->next, i++){
    vertices[i] = (Vertex*)itr->contents;
    //Store a reference to the apropos vbl in the vertex for sorting
    int vblRank = 
      VariableOrdering_GetVariableRankByLabel(this->vbls, 
					      Vertex_GetLabel(vertices[i])
					      );
    vertices[i]->variable = (this->vbls->vbls)[vblRank];
  }
  qsort(vertices, numVertices,
	sizeof(Vertex*),  vtx_SSW_cmp);
  ret->vertices = vertices; 
  return ret;
}

void BNet_BuildElimTree(BNet *this, VertexOrdering *o){
  //TODO detangle memory management, references to vertices
  Graph *moral = MoralGraph(this->initialGraph);
  ElimSetCollection *esc = GatherElimSets(moral,o,this->vbls);
  ElimSetCollection_RemoveNonCliques(esc,o);
  this->moral = moral;//TODO don't keep this around
  this->elimCliques = esc;
  this->elimTree = ElimTree_Create(esc);
  this->numCliques = Graph_GetNumVertices(this->elimTree);
  this->numSeps = Graph_GetNumEdges(this->elimTree);
  //Allocate space for pointers that will allow Potential manipulation
  //without walking the graph, for normalization etc...
  this->seperatorPots = jballoc(sizeof(Pot**) * this->numSeps);
  this->cliquePots = jballoc(sizeof(Pot**) * this->numCliques);
}

/** Generate a variable set containing the variables in variables.
    Add all clique potentials completely containing these variables to a list
    free the variable set.
    return the list*/
List *BNet_AllContainingCliquePotentials(BNet *this, List *vbls){
  VariableSet *vs = VariableSet_Create(this->vbls);
  ListNode *cur = vbls->first;
  while(cur != NULL){
    VariableSet_Add(vs,(Variable*)cur->contents);
    cur = cur->next;
  }
  List *ret = List_Create();
  int i;
  for(i =0; i < this->numCliques; i++){
    Pot *curPot = *(this->cliquePots[i]);
    if(VariableSet_ContainedIn(vs, curPot->vbls))
      List_Append(ret,curPot );
  }
  VariableSet_Destroy(vs);
  return ret;
}
List *BNet_AllContClqPot(BNet *this, Variable *vbl){
  VariableSet *vs = VariableSet_Create(this->vbls);
  VariableSet_Add(vs,vbl);
  List *ret = List_Create();
  int i;
  for(i =0; i < this->numCliques; i++){
    Pot *curPot = *(this->cliquePots[i]);
    if(VariableSet_ContainedIn(vs, curPot->vbls))
      List_Append(ret,curPot );
  }
  VariableSet_Destroy(vs);
  return ret;
}
void BNet_InitializePotentials(BNet *this){
  //initialize seperator potentials
  EdgeItr *eitr = Graph_GetEdgeItr(this->elimTree);
  Edge *curEdge;
  int i = 0,j;
  for(curEdge = EdgeItr_GetCurrent(eitr);
      curEdge != NULL;
      curEdge = EdgeItr_Advance(eitr)){
    VariableSet *vbls = VariableSet_CreateFromBv(this->vbls,
	   				 ((ElimSet*)curEdge->elim_set)->bv);
    curEdge->pot = Pot_CreateTabularUnity(Edge_GetSrcSink(curEdge),vbls);
    ((Pot*)curEdge->pot)->owner = curEdge;
    curEdge->mulMemo = MulMemo_Create();
    this->seperatorPots[i++] = (Pot**)&(curEdge->pot);
  }

  //initialize clique potentials
  VertexItr *vitr = Graph_GetVertexItr(this->elimTree);
  Vertex *curVertex;
  i = 0;
  for(curVertex = VertexItr_GetCurrent(vitr);
      curVertex != NULL;
      curVertex = VertexItr_Advance(vitr)){
    VariableSet *vbls = VariableSet_CreateFromBv(this->vbls,
	   				 ((ElimSet*)curVertex->elim_set)->bv);
    curVertex->pot = Pot_CreateTabularUnity(Vertex_GetLabel(curVertex),vbls);
    ((Pot*)curVertex->pot)->owner = curVertex;
    curVertex->margMemo = MulMemo_Create();
    this->cliquePots[i++] = (Pot**) &(curVertex->pot);
  }

  //multiply CPTs into clique potentials
  for(i =0; i < this->numTables; i++){
    Pot *curTable = (Pot*)this->tables[i]->pot;
    VariableSet *curVbls = curTable->vbls;
    for(j = 0; j < this->numCliques; j++){
      Pot *curClique = *(this->cliquePots[j]);
      if(VariableSet_ContainedIn(curVbls,curClique->vbls)){
	curVertex = (Vertex*)curClique->owner;
	MulInd *mi = MulMemo_GetMargInd(curVertex->margMemo,
					curClique->vbls,
					curVbls);
	Pot_ProductTabular(curTable,curClique,mi);
	break;
      }
    }
  }
  //multiply obs(if any exist into clique potentials
  for(i =0; i < this->numObs; i++){
    Pot *curTable = (Pot*)this->obs[i]->pot;
    VariableSet *curVbls = curTable->vbls;
    for(j = 0; j < this->numCliques; j++){
      Pot *curClique = *(this->cliquePots[j]);
      if(VariableSet_ContainedIn(curVbls,curClique->vbls)){
	curVertex = (Vertex*)curClique->owner;
	MulInd *mi = MulMemo_GetMargInd(curVertex->margMemo,
					curClique->vbls,
					curVbls);
	Pot_ProductTabular(curTable,curClique,mi);
	break;
      }
    }
  }
}

void BNet_InitializePotentialsLog(BNet *this){
  //initialize seperator potentials
  EdgeItr *eitr = Graph_GetEdgeItr(this->elimTree);
  Edge *curEdge;
  int i = 0,j;
  for(curEdge = EdgeItr_GetCurrent(eitr);
      curEdge != NULL;
      curEdge = EdgeItr_Advance(eitr)){
    VariableSet *vbls = VariableSet_CreateFromBv(this->vbls,
	   				 ((ElimSet*)curEdge->elim_set)->bv);
    curEdge->pot = Pot_CreateTabularUnityLog(Edge_GetSrcSink(curEdge),vbls);
    ((Pot*)curEdge->pot)->owner = curEdge;
    curEdge->mulMemo = MulMemo_Create();
    this->seperatorPots[i++] = (Pot**)&(curEdge->pot);
  }

  //initialize clique potentials
  VertexItr *vitr = Graph_GetVertexItr(this->elimTree);
  Vertex *curVertex;
  i = 0;
  for(curVertex = VertexItr_GetCurrent(vitr);
      curVertex != NULL;
      curVertex = VertexItr_Advance(vitr)){
    VariableSet *vbls = VariableSet_CreateFromBv(this->vbls,
	   				 ((ElimSet*)curVertex->elim_set)->bv);
    curVertex->pot = Pot_CreateTabularUnityLog(Vertex_GetLabel(curVertex),vbls);
    ((Pot*)curVertex->pot)->owner = curVertex;
    curVertex->margMemo = MulMemo_Create();
    this->cliquePots[i++] = (Pot**) &(curVertex->pot);
  }

  //multiply CPTs into clique potentials
  for(i =0; i < this->numTables; i++){
    Pot *curTable = (Pot*)this->tables[i]->pot;
    VariableSet *curVbls = curTable->vbls;
    for(j = 0; j < this->numCliques; j++){
      Pot *curClique = *(this->cliquePots[j]);
      if(VariableSet_ContainedIn(curVbls,curClique->vbls)){
	curVertex = (Vertex*)curClique->owner;
	MulInd *mi = MulMemo_GetMargInd(curVertex->margMemo,
					curClique->vbls,
					curVbls);
	Pot_ProductTabularLog(curTable,curClique,mi);
	break;
      }
    }
  }
  //multiply obs(if any exist into clique potentials
  for(i =0; i < this->numObs; i++){
    Pot *curTable = (Pot*)this->obs[i]->pot;
    VariableSet *curVbls = curTable->vbls;
    for(j = 0; j < this->numCliques; j++){
      Pot *curClique = *(this->cliquePots[j]);
      if(VariableSet_ContainedIn(curVbls,curClique->vbls)){
	curVertex = (Vertex*)curClique->owner;
	MulInd *mi = MulMemo_GetMargInd(curVertex->margMemo,
					curClique->vbls,
					curVbls);
	//Observations either 0 things out, or leave them unchanged.
	//So even when in log mode, we use the regular product
	Pot_ProductTabularLog(curTable,curClique,mi);
	break;
      }
    }
  }
}



void BNet_EnterEvidence(BNet *this, Evidence *e){
}
void BNet_ComputeSumMarginals(BNet *this){
}

/* Since the index structure may be in either the receiver,
 * We look in both.
 * Actually, the way this is going to work, all memos will be in the edges
 * Should have just done it that way by design.
 * TODO refactor this */
MulInd *find_marg_indices(Vertex *caller, Edge *sep){
  VariableSet *vbls = ((Pot*)sep->pot)->vbls;
  VariableSet *target = ((Pot*)caller->pot)->vbls;
  MulInd *ret = MulMemo_GetMulInd((MulMemo*)sep->mulMemo, vbls, target);
  return ret;
}

MulInd *find_mul_indices(Vertex *receiver, Edge *sep){
  VariableSet *vbls = ((Pot*)sep->pot)->vbls;
  VariableSet *target = ((Pot*)receiver->pot)->vbls;
  MulInd *ret = MulMemo_GetMulInd((MulMemo*)sep->mulMemo, vbls, target);
  return ret;
}

void MessagePass(Vertex *receiver, Vertex *caller, Edge *sep){
  //log
  /*printf("\nMessage passed from %s to %s .\n" ,
	 Vertex_GetLabel(caller),
	 Vertex_GetLabel(receiver) );*/
  MulInd *margInd = find_marg_indices(caller, sep);
  //log
  /*  printf("\n%s marginalized via %s\n", 
	 Vertex_GetLabel(caller),
	 MulInd_ToString(margInd));*/
  Pot *marg = Pot_SumMarginTabular((Pot*)caller->pot, margInd);
  /*  printf("\nMarginalized to \n%s\n", Pot_ToString(marg));*/
  Pot *updateRatio = Pot_UpdateRatioTabular(marg, (Pot*) sep->pot);
  /*  printf("\n UpdateRatio is \n%s\n", Pot_ToString(updateRatio));*/
  MulInd *mulInd = find_mul_indices(receiver, sep);
/*   printf("\nThis will be mul'ed into %s via \n  %s\n",  */
/* 	 Vertex_GetLabel(receiver), */
/* 	 MulInd_ToString(mulInd)); */
  Pot_ProductTabular(updateRatio, (Pot*)receiver->pot, mulInd);
  Pot_DestroyTabular((Pot*)sep->pot);
  sep->pot = marg;
}

void MessagePassLog(Vertex *receiver, Vertex *caller, Edge *sep){

/*   printf("\nMessage passed from %s to %s .\n" , */
/* 	 Vertex_GetLabel(caller), */
/* 	 Vertex_GetLabel(receiver) ); */
  MulInd *margInd = find_marg_indices(caller, sep);

/*    printf("\n%s marginalized via %s\n",  */
/* 	 Vertex_GetLabel(caller), */
/* 	 MulInd_ToString(margInd)); */
  Pot *marg = Pot_SumMarginTabularLog((Pot*)caller->pot, margInd);
  // printf("\nMarginalized to \n%s\n", Pot_ToString(marg));
  Pot *updateRatio = Pot_UpdateRatioTabularLog(marg, (Pot*) sep->pot);
  // printf("\n UpdateRatio is \n%s\n", Pot_ToString(updateRatio));
  MulInd *mulInd = find_mul_indices(receiver, sep);
/*   printf("\nThis will be mul'ed into %s via \n  %s\n", */
/* 	 Vertex_GetLabel(receiver), */
/* 	 MulInd_ToString(mulInd)); */
  Pot_ProductTabularLog(updateRatio, (Pot*)receiver->pot, mulInd);
  Pot_DestroyTabular((Pot*)sep->pot);
  sep->pot = marg;
}

/*Mark this in marks.
 *Call Vertex_CollectEvidence on each of this's unmarked nghbrs
 *Call Message_Pass(caller, this)*/
void Vertex_CollectEvidence(Vertex *this,  Vertex *caller,
			    Edge *backsep, VertexSet *marks){
  //mark
  VertexSet_Add(marks, this);

  //recurse
  NeighborItr *itr = Graph_GetNeighborItr(this->owner, this);
  Vertex *nghbr;
  Edge *fwdsep;
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr)){
      fwdsep = NeighborItr_GetCurrentEdge(itr);
      Vertex_CollectEvidence(nghbr,this, fwdsep, marks);
    }
  }

  //MessagePass
  if(caller != NULL)
    MessagePass(caller,this, backsep);
}//END Vertex_CollectEvidence

/*Mark this in marks.
 *Call Vertex_CollectEvidenceLog on each of this's unmarked nghbrs
 *Call Message_PassLog(caller, this)*/
void Vertex_CollectEvidenceLog(Vertex *this,  Vertex *caller,
			    Edge *backsep, VertexSet *marks){
  //mark
  VertexSet_Add(marks, this);

  //recurse
  NeighborItr *itr = Graph_GetNeighborItr(this->owner, this);
  Vertex *nghbr;
  Edge *fwdsep;
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr)){
      fwdsep = NeighborItr_GetCurrentEdge(itr);
      Vertex_CollectEvidenceLog(nghbr,this, fwdsep, marks);
    }
  }

  //MessagePass
  if(caller != NULL)
    MessagePassLog(caller,this, backsep);
}//END Vertex_CollectEvidenceLog


/*Generate a VertexSet to pass around as a marking structure
 *Call Vertex_CollectEvidence(fstVertex, NULL, marks) */ 
void BNet_CollectEvidence(BNet *this){
  VertexOrdering *o = defaultVertexOrdering(this->elimTree);
  VertexSet *marks = VertexSet_Create(o);
  Vertex *fstVertex = o->vertices[0];
  Vertex_CollectEvidence(fstVertex, NULL, NULL, marks);
  VertexSet_Destroy(marks);
}

void BNet_CollectEvidenceLog(BNet *this){
  VertexOrdering *o = defaultVertexOrdering(this->elimTree);
  VertexSet *marks = VertexSet_Create(o);
  Vertex *fstVertex = o->vertices[0];
  Vertex_CollectEvidenceLog(fstVertex, NULL, NULL, marks);
  VertexSet_Destroy(marks);
}

/*Mark this in marks
 *Call MessagePass(nghbr, this) for each unmarked nghbr
 *Call Vertex_DistributeEvidence on each of this's unmarked neighbors*/ 
void Vertex_DistributeEvidence(Vertex *this, VertexSet *marks){
  //mark
  VertexSet_Add(marks, this);

  //MessagePass
  NeighborItr *itr = Graph_GetNeighborItr(this->owner, this);
  Vertex *nghbr;
  Edge *fwdsep;
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr)){
      fwdsep = NeighborItr_GetCurrentEdge(itr);
      MessagePass(nghbr,this,fwdsep);
    }
  }

  //recurse
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr))
      Vertex_DistributeEvidence(nghbr, marks);
  }
}

/*Mark this in marks
 *Call MessagePassLog(nghbr, this) for each unmarked nghbr
 *Call Vertex_DistributeEvidenceLog on each of this's unmarked neighbors*/ 
void Vertex_DistributeEvidenceLog(Vertex *this, VertexSet *marks){
  //mark
  VertexSet_Add(marks, this);

  //MessagePass
  NeighborItr *itr = Graph_GetNeighborItr(this->owner, this);
  Vertex *nghbr;
  Edge *fwdsep;
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr)){
      fwdsep = NeighborItr_GetCurrentEdge(itr);
      MessagePassLog(nghbr,this,fwdsep);
    }
  }

  //recurse
  for( nghbr= NeighborItr_GetCurrent(itr);
       nghbr != NULL;
       nghbr= NeighborItr_Advance(itr)){
    if(!VertexSet_ContainsVertex(marks, nghbr))
      Vertex_DistributeEvidenceLog(nghbr, marks);
  }
}


/*Generate a VertexSet to pass around as a marking structure
 *Call Vertex_DistribueEvidence(fstVertex, marks) */ 
void BNet_DistributeEvidence(BNet *this){
  VertexOrdering *o = defaultVertexOrdering(this->elimTree);
  VertexSet *marks = VertexSet_Create(o);
  Vertex *fstVertex = o->vertices[0];
  Vertex_DistributeEvidence(fstVertex, marks);
  VertexSet_Destroy(marks);
}

/*Generate a VertexSet to pass around as a marking structure
 *Call Vertex_DistribueEvidence(fstVertex, marks) */ 
void BNet_DistributeEvidenceLog(BNet *this){
  VertexOrdering *o = defaultVertexOrdering(this->elimTree);
  VertexSet *marks = VertexSet_Create(o);
  Vertex *fstVertex = o->vertices[0];
  Vertex_DistributeEvidenceLog(fstVertex, marks);
  VertexSet_Destroy(marks);
}

/**
 * methods for SWIG/integration testing convenience. RELOCATE
 * BuildSimpleBNetBase 
 * BuildSimpleBNetDObs(int val)
 * BuildSimpleBNetAObs(int val)
 * BuildSimpleBNetBObs(int val)
 * BuildSimpleBNetCObs(int val)
 * and the log analogues
 */
void BuildSimpleBNetBase(Variable **A,Variable **B,
			  Variable **C, Variable **D,
			  VariableOrdering **o,
			  CPT ***tables){
  //Make variables and  variable ordering
  List *l = List_Create();
  *A = Variable_CreateDiscrete(String_Create("A"),2,NULL);
  *B = Variable_CreateDiscrete(String_Create("B"),2,NULL);
  *C = Variable_CreateDiscrete(String_Create("C"),2,NULL);
  *D = Variable_CreateDiscrete(String_Create("D"),2,NULL);
  List_Append(l,*D);
  List_Append(l,*A);
  List_Append(l,*B);
  List_Append(l,*C);
  *o = VariableOrdering_Create(l);

  //Make CPTs
  *tables = (CPT**)jballoc(sizeof(CPT*)*4);

  //Create a table for P(C|A,B)
  VariableSet *vs1 = VariableSet_Create(*o);
  VariableSet_Add(vs1,*A);
  VariableSet_Add(vs1,*B);
  VariableSet_Add(vs1,*C);
  double *valueData1 = (double*)jballoc(sizeof(double) *8);
  valueData1[0] = 1.0/2.0;/* A=0, B=0, C=0 */
  valueData1[1] = 1.0/2.0;/* A=0, B=0, C=1 */
  valueData1[2] = 3.0/4.0;/* A=0, B=1, C=0 */
  valueData1[3] = 1.0/4.0;/* A=0, B=1, C=1 */
  valueData1[4] = 1.0/2.0;/* A=1, B=0, C=0 */
  valueData1[5] = 1.0/2.0;/* A=1, B=0, C=1 */
  valueData1[6] = 3.0/4.0;/* A=1, B=1, C=0 */
  valueData1[7] = 1.0/4.0;/* A=1, B=1, C=1 */
  Pot *p1 = Pot_CreateTabular("cptPot",vs1,valueData1);
  Variable **parents1 = jballoc(sizeof(Variable*)*2);
  parents1[0]=*A;
  parents1[1]=*B;
  (*tables)[0] = CPT_Create(parents1, 2, *C, p1);

  //Create a table for P(A|D)
  VariableSet *vs2 = VariableSet_Create(*o);
  VariableSet_Add(vs2,*A);
  double *valueData2 = (double*)jballoc(sizeof(double) *4);
  valueData2[0] = 1.0/2.0;/* D=0, A=0,*/
  valueData2[1] = 1.0/2.0;/* D=0, A=1,*/
  valueData2[2] = 1.0/4.0;/* D=1, A=0,*/
  valueData2[3] = 3.0/4.0;/* D=1, A=1,*/
  Pot *p2 = Pot_CreateTabular("cptPot",vs2,valueData2);
  Variable **parents2 = jballoc(sizeof(Variable*));
  parents2[0]=*D;
  (*tables)[1] = CPT_Create(parents2, 1, *A, p2);

  //Create a table for P(B|D)
  VariableSet *vs3 = VariableSet_Create(*o);
  VariableSet_Add(vs3,*B);
  double *valueData3 = (double*)jballoc(sizeof(double) *4);
  valueData3[0] = 1.0/4.0;/* D=0, B=0,*/
  valueData3[1] = 3.0/4.0;/* D=0, B=1,*/
  valueData3[2] = 2.0/4.0;/* D=1, B=0,*/
  valueData3[3] = 2.0/4.0;/* D=1, B=1,*/
  Pot *p3 = Pot_CreateTabular("cptPot",vs3,valueData3);
  Variable **parents3 = jballoc(sizeof(Variable*));
  parents3[0]=*D;
  (*tables)[2] = CPT_Create(parents3, 1, *B, p3);

  //Create a table for P(D)
  VariableSet *vs4 = VariableSet_Create(*o);
  VariableSet_Add(vs4,*D);
  double *valueData4 = (double*)jballoc(sizeof(double) *2);
  valueData4[0] = 1.0/4.0;/* D=0*/
  valueData4[1] = 3.0/4.0;/* D=1*/
  Pot *p4 = Pot_CreateTabular("cptPot",vs4,valueData4);
  (*tables)[3]= CPT_Create(NULL, 0, *D, p4);  
  return;
}
void BuildSimpleBNetBaseLog(Variable **A,Variable **B,
			  Variable **C, Variable **D,
			  VariableOrdering **o,
			  CPT ***tables){
  //Make variables and  variable ordering
  List *l = List_Create();
  *A = Variable_CreateDiscrete(String_Create("A"),2,NULL);
  *B = Variable_CreateDiscrete(String_Create("B"),2,NULL);
  *C = Variable_CreateDiscrete(String_Create("C"),2,NULL);
  *D = Variable_CreateDiscrete(String_Create("D"),2,NULL);
  List_Append(l,*D);
  List_Append(l,*A);
  List_Append(l,*B);
  List_Append(l,*C);
  *o = VariableOrdering_Create(l);

  //Make CPTs
  *tables = (CPT**)jballoc(sizeof(CPT*)*4);

  //Create a table for P(C|A,B)
  VariableSet *vs1 = VariableSet_Create(*o);
  VariableSet_Add(vs1,*A);
  VariableSet_Add(vs1,*B);
  VariableSet_Add(vs1,*C);
  double *valueData1 = (double*)jballoc(sizeof(double) *8);
  valueData1[0] = log(1.0/2.0);/* A=0, B=0, C=0 */
  valueData1[1] = log(1.0/2.0);/* A=0, B=0, C=1 */
  valueData1[2] = log(3.0/4.0);/* A=0, B=1, C=0 */
  valueData1[3] = log(1.0/4.0);/* A=0, B=1, C=1 */
  valueData1[4] = log(1.0/2.0);/* A=1, B=0, C=0 */
  valueData1[5] = log(1.0/2.0);/* A=1, B=0, C=1 */
  valueData1[6] = log(3.0/4.0);/* A=1, B=1, C=0 */
  valueData1[7] = log(1.0/4.0);/* A=1, B=1, C=1 */
  Pot *p1 = Pot_CreateTabular("cptPot",vs1,valueData1);
  Variable **parents1 = jballoc(sizeof(Variable*)*2);
  parents1[0]=*A;
  parents1[1]=*B;
  (*tables)[0] = CPT_Create(parents1, 2, *C, p1);

  //Create a table for P(A|D)
  VariableSet *vs2 = VariableSet_Create(*o);
  VariableSet_Add(vs2,*A);
  double *valueData2 = (double*)jballoc(sizeof(double) *4);
  valueData2[0] = log(1.0/2.0);/* D=0, A=0,*/
  valueData2[1] = log(1.0/2.0);/* D=0, A=1,*/
  valueData2[2] = log(1.0/4.0);/* D=1, A=0,*/
  valueData2[3] = log(3.0/4.0);/* D=1, A=1,*/
  Pot *p2 = Pot_CreateTabular("cptPot",vs2,valueData2);
  Variable **parents2 = jballoc(sizeof(Variable*));
  parents2[0]=*D;
  (*tables)[1] = CPT_Create(parents2, 1, *A, p2);

  //Create a table for P(B|D)
  VariableSet *vs3 = VariableSet_Create(*o);
  VariableSet_Add(vs3,*B);
  double *valueData3 = (double*)jballoc(sizeof(double) *4);
  valueData3[0] = log(1.0/4.0);/* D=0, B=0,*/
  valueData3[1] = log(3.0/4.0);/* D=0, B=1,*/
  valueData3[2] = log(2.0/4.0);/* D=1, B=0,*/
  valueData3[3] = log(2.0/4.0);/* D=1, B=1,*/
  Pot *p3 = Pot_CreateTabular("cptPot",vs3,valueData3);
  Variable **parents3 = jballoc(sizeof(Variable*));
  parents3[0]=*D;
  (*tables)[2] = CPT_Create(parents3, 1, *B, p3);

  //Create a table for P(D)
  VariableSet *vs4 = VariableSet_Create(*o);
  VariableSet_Add(vs4,*D);
  double *valueData4 = (double*)jballoc(sizeof(double) *2);
  valueData4[0] = log(1.0/4.0);/* D=0*/
  valueData4[1] = log(3.0/4.0);/* D=1*/
  Pot *p4 = Pot_CreateTabular("cptPot",vs4,valueData4);
  (*tables)[3]= CPT_Create(NULL, 0, *D, p4);  
  return;
}

BNet *BuildSimpleBNet(){
  Variable *A,*B, *C, *D;
  VariableOrdering *o;
  CPT **tables;
  BuildSimpleBNetBase(&A,&B,&C,&D,&o,&tables);
  BNet *bn = BNet_Create(o,tables, 4);
  return bn;
}

BNet *BuildSimpleBNetDObs(int val){
  Variable *A,*B, *C, *D;
  VariableOrdering *o;
  CPT **tables;
  BuildSimpleBNetBase(&A,&B,&C,&D,&o,&tables);
  VariableSet *dset = VariableSet_Create(o);
  VariableSet_Add(dset,D);
  Observation *obs = Observation_Create(dset, val);
  List *tlist, *olist;
  tlist = List_Create();
  int i;
  for( i = 0; i < 4; i++){
    List_Append(tlist,tables[i]);
  }
  olist = List_Create();
  List_Append(olist,obs);
  BNet *bn = BNet_Create_List2(o,tlist, olist);
  return bn;
}
BNet *BuildSimpleBNetDObsLog(int val){
  Variable *A,*B, *C, *D;
  VariableOrdering *o;
  CPT **tables;
  BuildSimpleBNetBaseLog(&A,&B,&C,&D,&o,&tables);
  VariableSet *dset = VariableSet_Create(o);
  VariableSet_Add(dset,D);
  Observation *obs = Observation_CreateLog(dset, val);
  List *tlist, *olist;
  tlist = List_Create();
  int i;
  for( i = 0; i < 4; i++){
    List_Append(tlist,tables[i]);
  }
  olist = List_Create();
  List_Append(olist,obs);
  BNet *bn = BNet_Create_List2(o,tlist, olist);
  return bn;
}

BNet *BuildSimpleBNetCObs(int val){
  Variable *A, *B, *C, *D;
  VariableOrdering *o;
  CPT **tables;
  BuildSimpleBNetBase(&A,&B,&C,&D,&o,&tables);
  VariableSet *cset = VariableSet_Create(o);
  VariableSet_Add(cset,C);
  Observation *obs = Observation_Create(cset, val);
  List *tlist, *olist;
  tlist = List_Create();
  int i;
  for( i = 0; i < 4; i++){
    List_Append(tlist,tables[i]);
  }
  olist = List_Create();
  List_Append(olist,obs);
  BNet *bn = BNet_Create_List2(o,tlist, olist);
  return bn;
}
BNet *BuildSimpleBNetAObs(int val){
  Variable *A, *B, *C, *D;
  VariableOrdering *o;
  CPT **tables;
  BuildSimpleBNetBase(&A,&B,&C,&D,&o,&tables);
  VariableSet *aset = VariableSet_Create(o);
  VariableSet_Add(aset,A);
  Observation *obs = Observation_Create(aset, val);
  List *tlist, *olist;
  tlist = List_Create();
  int i;
  for( i = 0; i < 4; i++){
    List_Append(tlist,tables[i]);
  }
  olist = List_Create();
  List_Append(olist,obs);
  BNet *bn = BNet_Create_List2(o,tlist, olist);
  return bn;
}

//Helper Functions
//TODO remove use of strcatLK
char *MulInd_ToString(MulInd *this){
  int i, j;
  int **qindices = this->secondIndices;
  char *msg = "";
  char buf[10];
  for(i =0; i < this->firstIndices; i++){
    sprintf(buf, "%i :", i);
    msg = strcatLK(msg, buf, "\n");
    for(j =0; j < this->mulSize; j++){
      int qindex = qindices[i][j];
      sprintf(buf, "%i", qindex);
      msg = strcatLK(msg, buf," ");
    }
  }
  return msg;
}


char *Pot_ToString(Pot *this){
  int i;
  char *msg = jballoc( strlen(this->label) + 1 + (32 * this->tableSize));
  msg[0] = '\0';
  strcat(msg, this->label);
  strcat(msg, " ");
  char buf[32];
  for(i = 0; i < this->tableSize; i++){
    sprintf(buf ,"%f ", this->valueData.tabData[i]);
    strcat(msg, buf);
  }
  return msg;
}

char *Pot_ToStringLog(Pot *this){
  int i;
  char *msg = jballoc( strlen(this->label) + 1 + (32 * this->tableSize));
  msg[0] = '\0';
  strcat(msg, this->label);
  strcat(msg, " ");
  char buf[32];
  for(i = 0; i < this->tableSize; i++){
    sprintf(buf ,"%f ", exp(this->valueData.tabData[i]));
    strcat(msg, buf);
  }
  return msg;
}

void Print_BNet(BNet *bn){
  int i;
  printf("\n************************************\n");
  printf("NumTables : %i NumVertices:%i NumEdges:%i NumVbls :%i\n", bn->numTables,
	 Graph_GetNumVertices( bn->initialGraph),
	 Graph_GetNumEdges(   bn->initialGraph),
	 bn->vbls->size);
  printf("************************************\n");
  printf("\n************************************\n");
  printf("Clique Potentials : %i\n", bn->numCliques);
  printf("************************************\n");
    for(i = 0; i < bn->numCliques; i++){
    printf("\n%s\n",Pot_ToString(*(bn->cliquePots[i])));
  }
  printf("\n************************************\n");
  printf("Seperator Potentials %i\n ", bn->numSeps);
  printf("************************************\n");
  for(i = 0; i < bn->numSeps; i++){
    printf("\n %s \n",Pot_ToString(*(bn->seperatorPots[i])));
  }
}

void Print_BNetLog(BNet *bn){
  int i;
  printf("\n************************************\n");
  printf("NumTables : %i NumVertices:%i NumEdges:%i NumVbls :%i\n", bn->numTables,
	 Graph_GetNumVertices( bn->initialGraph),
	 Graph_GetNumEdges( bn->initialGraph),
	 bn->vbls->size);
  printf("************************************\n");
  printf("\n************************************\n");
  printf("Clique Potentials : %i\n", bn->numCliques);
  printf("************************************\n");
    for(i = 0; i < bn->numCliques; i++){
    printf("\n%s\n",Pot_ToStringLog(*(bn->cliquePots[i])));
  }
  printf("\n************************************\n");
  printf("Seperator Potentials %i\n ", bn->numSeps);
  printf("************************************\n");
  for(i = 0; i < bn->numSeps; i++){
    printf("\n %s \n",Pot_ToStringLog(*(bn->seperatorPots[i])));
  }
}
