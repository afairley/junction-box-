               :#include "JB_JunctionTree.h"
               :#include <stdlib.h>
               :/* Pass in a list ordering vertices from lowest to highest, 
               : * get back an array backed  collection with the same ordering*/
               :VertexOrdering *VertexOrdering_Create(List *vIn){
               :  int i;
               :  ListNode *vInItr;
               :  VertexOrdering *ret = (VertexOrdering*)jballoc(sizeof(VertexOrdering));
               :  ret->vertices =(Vertex**)jballoc( sizeof(Vertex*) * vIn->size);
               :  ret->size = vIn->size;
               :  
               :  for(i =0, vInItr = vIn->first ;
               :      i < ret->size && vInItr != NULL;
               :      i++, vInItr = vInItr->next){
               :    ret->vertices[i] = vInItr->contents;
               :  }
               :  if(i < ret->size)
               :    reportError(" Lists aren't tracking their size properly.");
               :  return ret;
               :}
               :
               :void VertexOrdering_Destroy(VertexOrdering *this){
               :  jbfree(this->vertices);
               :  jbfree(this);
               :}
               :
               :
               :/*simply iterates through the vertices of g using Graph_GetFstVertex,
               :  Graph_GetNxtVertex and produces an identically ordered Vertex Ordering*/
               :VertexOrdering *defaultVertexOrdering(Graph *g){
               :  Vertex * cur;
               :  List *l = List_Create();
               :  for( cur = Graph_GetFstVertex(g);
               :       cur != NULL;
               :       cur = Graph_GetNxtVertex(g,cur)){
               :    List_Append(l, cur);
               :  }
               :  VertexOrdering *ret = VertexOrdering_Create(l);
               :  //TODO List_Destroy
               :  return ret;
               :}
               :
               :char *VertexOrdering_ToString(VertexOrdering *this){
               :  int length =0;
               :  int i;
               :  char *ret;
               :  for (i =0 ; i < this->size; i++){
               :    length += strlen(Vertex_GetLabel(this->vertices[i])) +1/*following
               :							     space*/;
               :  }
               :  ret = jballoc(length*sizeof(char) +1);
               :  ret[0]=(char)0;
               :  for (i =0 ; i < this->size; i++){
               :    strcat(ret,Vertex_GetLabel(this->vertices[i]));
               :    strcat(ret, " ");
               :  }
               :  return ret;
               :}
               :
               :/*linear search, with pointer comparison. Probably want to replace this
               :  with a hash table lookup*/
               :int VertexOrdering_GetVertexRank(VertexOrdering *this, Vertex *v){
               :  int ret;
               :  for(ret = 0; ret < this->size; ret++){
               :    if(this->vertices[ret] == v)
               :       return ret;
               :  }
               :  return -1;
               :}
               :/*linear search, with string comparison. Probably want to replace this
               :  with a hash table lookup*/
               :int VertexOrdering_GetVertexRankByLabel(VertexOrdering *this, char *c){ /* VertexOrdering_GetVertexRankByLabel total:      1  0.0824 */
               :  int ret;
               :  for(ret = 0; ret < this->size; ret++){
     1  0.0824 :    if(strcmp( Vertex_GetLabel(this->vertices[ret]),c) == 0)
               :       return ret;
               :  }
               :  return -1;
               :}
               :
               :ElimSet *ElimSet_Create(VariableOrdering *o){
               :  ElimSet *ret = (ElimSet*)jballoc(sizeof(ElimSet));
               :  ret->bv = BitVector_Create(o->size, jballoc,jbfree, 0);
               :  ret->o = o;
               :  return ret;
               :}
               :
               :char *ElimSet_GetLabel(ElimSet *this){ /* ElimSet_GetLabel total:      1  0.0824 */
               :  int i, length = 0;
               :  char *ret;
               :  for(i = 0; i < BitVector_GetSize(this->bv,0); i++){
               :    if(BitVector_GetBit(this->bv, i))
               :      length += strlen( Variable_GetLabel(this->o->vbls[i])  ) +1;
               :    //+1 for space following or NULL in the last case
               :  }
               :  ret = jballoc(length * sizeof(char) +1);//+1 for a trailing \n 
               :  //which will be put there by ElimSetCollection_ToString
               :  ret[0] = (char)0;
               :  for(i = 0; i < BitVector_GetSize(this->bv,0); i++){
     1  0.0824 :    if(BitVector_GetBit(this->bv, i)){
               :      strcat(ret, Variable_GetLabel(this->o->vbls[i]) );
               :      strcat(ret, " ");
               :    }
               :  }
               :  return ret;
               :}
               ://TODO unify nomenclature. ToString for all data structure
               :char *ElimSet_ToString(ElimSet *this){
               : return ElimSet_GetLabel(this);
               :}
               :
               ://Add a vertex to an elimination set
               ://Should have no effect if the vertex is already in the set
               :void ElimSet_Add(ElimSet *this, Vertex *v){
               :  int indx = VariableOrdering_GetVariableRankByLabel(
               :						     this->o,
               :						     Vertex_GetLabel(v)
               :						     );
               :  if(indx >= 0){
               :    BitVector_SetBitOn(this->bv,indx,0); 
               :  }
               :}
               ://TODO This is inefficient because of the way that 
               ://Graph_GetVertex is implemented. Doing this necessitates using a hashmap.
               :List *ElimSet_GetVertexList(ElimSet *this, Graph *g){ /* ElimSet_GetVertexList total:      1  0.0824 */
               :  int i;
               :  List *ret = List_Create();
     1  0.0824 :  for(i = 0; i > BitVector_GetSize(this->bv,0); i++){
               :    if(BitVector_GetBit(this->bv, i)){
               :      char *label = Variable_GetLabel(this->o->vbls[i]);
               :      List_Append(ret,Graph_GetVertex(g, label));
               :    }
               :  }
               :  return ret;
               :}
               :/*Returns true if this contains other*/
               :int ElimSet_Contains(ElimSet *this, ElimSet *other){ /* ElimSet_Contains total:      1  0.0824 */
               :  BitVector *test = BitVector_And(this->bv,other->bv,0);
     1  0.0824 :  int ret = BitVector_Equals(other->bv,test,0);
               :  BitVector_Destroy(test);
               :  return ret;
               :}
               :
               :ElimSet *ElimSet_Intersect(ElimSet *this, ElimSet *other){ /* ElimSet_Intersect total:      3  0.2471 */
               :  ElimSet *ret = (ElimSet*)jballoc(sizeof(ElimSet));
     1  0.0824 :  ret->bv = BitVector_And(this->bv, other->bv, 0);
     1  0.0824 :  ret->o = this->o;
               :  return ret;
     1  0.0824 :}
               :
               :typedef Edge EdgeCandidate;
               :
               :EdgeCandidate *EdgeCandidate_Create(Vertex *src, Vertex *sink){
               :  EdgeCandidate *ret = (EdgeCandidate*)jballoc(sizeof(EdgeCandidate));
               :/*   ret->edge = NULL; */
               :  ret->src_sink = NULL;
               :  ret->owner = NULL;
               :  ret->src = src;
               :  ret->sink = sink;
               :  ret->elim_set = ElimSet_Intersect(Vertex_GetElimSet(src),
               :				   Vertex_GetElimSet(sink));
               :  return ret;
               :}
               :
               :typedef struct ecc_collect ECCollection;
               :
               :struct ecc_collect{
               :  int size;
               :  VertexOrdering *o;
               :  EdgeCandidate **edgeCs;
               :};
               :
     1  0.0824 :int ECCollection_DefEdgeComp(const void *ec1, const void *ec2){ /* ECCollection_DefEdgeComp total:      3  0.2471 */
     2  0.1647 :  EdgeCandidate *edgec1 = *(EdgeCandidate**)ec1, *edgec2 =*(EdgeCandidate**)ec2;
               :  int onBits1 = BitVector_GetNumOnBits(
               :				       ((ElimSet*)Edge_GetElimSet(edgec1))->bv
               :				       ,0);
               :  int onBits2 = BitVector_GetNumOnBits(
               :				       ((ElimSet*)Edge_GetElimSet(edgec2))->bv
               :				       ,0);
               :  return onBits2 - onBits1;
               :}
               :
               :ECCollection *ECCollection_Create(VertexOrdering *o, int (*cmp)(const void *, const void*) ){ /* ECCollection_Create total:      1  0.0824 */
               :  ECCollection *ret = (ECCollection *)jballoc(sizeof(ECCollection));
               :  int i,j, index;
               :  Vertex *src, *sink;
               :  if(cmp == NULL)
               :    cmp = ECCollection_DefEdgeComp;
               :  ret->size = (o->size *(o->size -1) ) /2;
               :  ret->o = o;
               :  ret->edgeCs = jballoc(ret->size * sizeof(EdgeCandidate));
               :  for(i = 0, index = 0; i < o->size; i++){
               :    src = o->vertices[i];
               :    for(j = i + 1; j < o->size; j++){
               :      sink = o->vertices[j];
     1  0.0824 :      ret->edgeCs[index++] = EdgeCandidate_Create(src,sink);
               :    }
               :  }
               :  qsort(ret->edgeCs,ret->size ,sizeof(EdgeCandidate*), cmp);
               :  return ret;
               :}
               :
               :EdgeCandidate **ECCollection_GetEdgeCs(ECCollection *this){
               :  return this->edgeCs;
               :}
               :
               :
               :ElimSetCollection *ElimSetCollection_Create(int size){
               :  ElimSetCollection *ret = (ElimSetCollection*)jballoc(sizeof(ElimSetCollection));
               :  ret->elim_sets =(ElimSet**)jballoc( sizeof(ElimSet*) * size);
               :  ret->size = size;
               :  ret->remap = IntMap_Create();
               :  return ret;
               :}
               :
               :
               :ElimSet *ElimSetCollection_Get(ElimSetCollection *this, int indx){
               :  return this->elim_sets[indx];
               :}
               :
               :ElimSet *ElimSetCollection_Get_W_ReMap(ElimSetCollection *this, int indx){
               :  ElimSet *ret = this->elim_sets[indx];
               :  int newIndx = 0;
               :  if(ret == NULL){
               :    newIndx = IntMap_Get(this->remap, indx)->contents;
               :    if(newIndx < 0){
               :      return NULL;
               :    }
               :    ret =  this->elim_sets[newIndx];
               :    if(ret == NULL)
               :      ret = ElimSetCollection_Get_W_ReMap(this, newIndx);
               :  }
               :  return ret;
               :}
               :void ElimSetCollection_ReMap(ElimSetCollection *this,
               :			     int oldIndx, int newIndx,
               :			     VertexOrdering *o){
               :  //TODO Remove remaping intmap from ESC
               :  this->elim_sets[newIndx] = this->elim_sets[oldIndx];
               :  this->elim_sets[oldIndx] = NULL;
               :
               :}
               :
               :void ElimSetCollection_Add(ElimSetCollection *this, ElimSet *eset, int indx){
               :  this->elim_sets[indx]=eset;
               :}
               :
               :char  *ElimSetCollection_ToString(ElimSetCollection *this){
               :  int i;
               :  char *ret;
               :  ListNode *stringItr;
               :  int stringLength = 0;
               :  List *strings = List_Create();
               :  for (i =0 ; i < this->size; i++){
               :    ElimSet *es = this->elim_sets[i];
               :    if(es != NULL)
               :      List_Append(strings, ElimSet_ToString(es));
               :  }
               :  for(stringItr = strings->first;
               :      stringItr != NULL;
               :      stringItr = stringItr->next){
               :    stringLength += strlen( (char *)stringItr->contents)  +1;//for \n
               :  }
               :
               :  stringLength += 1;//For null
               :  ret = jballoc(stringLength);
               :  ret[0] = (char)0;
               :  
               :  for(stringItr = strings->first;
               :      stringItr != NULL;
               :      stringItr = stringItr->next){
               :    char *curstr =(char *)stringItr->contents;
               :    strcat(ret,curstr);
               :    strcat(ret, "\n");
               :  }
               :  
               :  return ret;
               :}
               :
               :Graph *MoralGraph(Graph *imm){
               :  //TODO mangle the name of imm to produce ret's name
               :  Graph *ret = Graph_Copy(imm);
               :  Graph_SetDirected(ret, FALSE);
               :  Vertex *current, *currentMoral;
               :  
               :  for(current = Graph_GetFstVertex(imm);
               :      current != NULL;
               :      current = Graph_GetNxtVertex(imm, current))
               :    {
               :      currentMoral =Graph_GetVertex(ret, Vertex_GetLabel(current));
               :      List *parents = List_Create();
               :      Vertex *curParent1, *curParent2, *moralParent1, *moralParent2;
               :      ListNode *cursor1, *cursor2;
               :      curParent1 = Graph_GetFstParent(imm, current);
               :      if(curParent1 == NULL){
               :	continue;//Node has no parents
               :      }
               :      for(;
               :	  curParent1 != NULL;
               :	  curParent1 = Graph_GetNxtParent(imm, current, curParent1))
               :	{    
               :	  List_Append(parents,curParent1);
               :	  moralParent1 = Graph_GetVertex(ret,Vertex_GetLabel(curParent1));
               :	}
               :      for(cursor1 = parents->first;
               :	  cursor1 != NULL;
               :	  cursor1 = cursor1->next)
               :	{
               :	  curParent1 = (Vertex*)cursor1->contents;
               :	  moralParent1 = Graph_GetVertex(ret,Vertex_GetLabel(curParent1));
               :	  for(cursor2 = cursor1->next;
               :	      cursor2 != NULL;
               :	      cursor2 = cursor2->next)
               :	    {
               :	      curParent2 = (Vertex*)cursor2->contents;
               :	      moralParent2 = Graph_GetVertex(ret,Vertex_GetLabel(curParent2));
               :	      Graph_AddEdge(ret, moralParent1, moralParent2);
               :	    }
               :	}
               :      
               :    }
               :  return ret;
               :}
               ://Gathers the elimination sets of of moral, and triangulates it
               ://in place according to the ordering specified by o
               :ElimSetCollection *GatherElimSets(Graph *moral, 
               :				  VertexOrdering *o,
               :				  VariableOrdering *o2){ /* GatherElimSets total:      1  0.0824 */
               :  int i;
               :  ElimSetCollection *ret = ElimSetCollection_Create(o->size);
               :  for(i = o->size -1; i > -1; i--){
               :    Vertex *current = Graph_GetVertex(moral,
               :				      Vertex_GetLabel(o->vertices[i])
     1  0.0824 :				      );//Vertices in o are not from moral!
               :    Vertex *nghbr;
               :    ElimSet *eset = ElimSet_Create(o2);
               :    ElimSet_Add(eset, current);
               :    NeighborItr * itr =  Graph_GetNeighborItr(moral,current);
               :    for( nghbr= NeighborItr_GetCurrent(itr);
               :	 nghbr != NULL;
               :	 nghbr= NeighborItr_Advance(itr)){
               :      if(VertexOrdering_GetVertexRankByLabel(o,Vertex_GetLabel(nghbr)) < i){
               :	ElimSet_Add(eset, nghbr);
               :      }      
               :    }
               :    NeighborItr_Destroy(itr);
               :    //now connect all members of eset
               :    Graph_ConnectAll(moral,ElimSet_GetVertexList(eset, moral));
               :    ElimSetCollection_Add(ret, eset, i);
               :  }
               :  return ret;
               :}
               :
               :
               :void ElimSetCollection_RemoveNonCliques(ElimSetCollection *this,
               :					VertexOrdering *o       ) {
               :  int i,j;
               :  for(i = 0; i < this->size; i++){
               :    ElimSet *possiblyContained = ElimSetCollection_Get(this, i);
               :    if(possiblyContained == NULL)
               :      continue;
               :    for(j = i+1; j < this->size; j++){
               :      ElimSet *possiblyContains = ElimSetCollection_Get(this, j);
               :      if(i == j)
               :	continue;
               :      if(possiblyContains == NULL)
               :	continue;//j has been swapped for something else already
               :      if(ElimSet_Contains(possiblyContains, possiblyContained)){
               :	ElimSetCollection_ReMap(this, j,i,o);
               :	j = -1;
               :	break;
               :      }
               :    }
               :    if(j == -1)
               :      //Since we swapped in j, we must now see if it is contained elsewhere
               :      i = i-1;
               :  }
               :}
               :
               :/*As outlined in Jordan's preprint, we will employ Kruskal's Algorithm
               :  to generate a spanning tree over the cliques which maximizes
               :  seperator cardinality.  The correctness of the suggestions in
               :  Cowell et. al. evades me, and they don't supply proof outside of a
               :  reference to a difficult to obtain technical report. I tried a
               :  number of interpretations, but they didn't appear to work. */
               :/*MST-Kruskal (V, w)
               : * A <- EMPTY_SET
               : * for v in V
               : *  DSetForest_MakeSet(v)
               : * Enumerate Possible Edges of G into E
               : * Sort E by w
               : * for e=(u,v) in E
               : *    if( DSetForest_FindSet(u) != DSetForest_FindSet(v) )
               : *       A <- A \cup {e}
               : *       DSetForest_Union(u,v)
               : */
               :/*TODO Once Variables/Potentials are implemented, I'm going to want to include
               :  tie breaking by minimum SepSet cost, as discussed 
               :  in Darwiche's open secrets paper*/
               :ElimTree *ElimTree_Create(ElimSetCollection *esc){
               :  ElimTree *ret = Graph_Create("Elim_Tree", FALSE);
               :  Vertex *u,*v;
               :  int i;
               :  DSetForest *dsf = DSetForest_Create();
               :  //Create a vertex for every clique
               :  //carry out the "Make Set portion of kruskal
               :  for(i = 0;  i < esc->size; i++){
               :    ElimSet *es = ElimSetCollection_Get(esc, i);
               :    if(es == NULL){
               :      continue;
               :    }
               :    char *label = ElimSet_GetLabel(es);
               :    v =Graph_GetVertex(ret,label);
               :    if(v == NULL){
               :      v = Graph_AddVertex(ret,label);
               :      Vertex_SetElimSet(v,es);
               :      DSetForest_MakeSet(dsf, v);
               :      //Note that the vertices in here are from the original triangulated graph
               :    }
               :  }
               :
               :  ECCollection *ecc = ECCollection_Create(defaultVertexOrdering(ret),NULL);
               :  int numVertices = Graph_GetNumVertices(ret);
               :  int numEdgeCs = ecc->size;
               :  int numEdges = 0;
               :  EdgeCandidate **edgeCs = ECCollection_GetEdgeCs(ecc);
               :  for(i =0; i < numEdgeCs; i++){
               :    EdgeCandidate *ec =edgeCs[i];
               :    u = Edge_GetHead(ec);
               :    v = Edge_GetTail(ec);
               :    if(DSetForest_GetRepresentative(dsf,u) != DSetForest_GetRepresentative(dsf,v)){
               :      numEdges += 1;
               :      Edge *e = Graph_AddEdge(ret,u,v);
               :      Edge_SetElimSet(e, Edge_GetElimSet(ec));
               :      DSetForest_Union(dsf, u,v);
               :      if(numEdges == numVertices -1)
               :	break;//We've finished the tree, so let's stop checking for edges
               :    }
               :  }
               :  return ret;
               :}
               :  
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_JunctionTree/JB_JunctionTree.c"
 * 
 *     12  0.9885
 */


/* 
 * Command line: opannotate --source -o opannotate/ libjunctionbox.so 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: Core 2, speed 2200 MHz (estimated)
 * Counted CPU_CLK_UNHALTED events (Clock cycles when not halted) with a unit mask of 0x00 (Unhalted core cycles) count 100000
 */
