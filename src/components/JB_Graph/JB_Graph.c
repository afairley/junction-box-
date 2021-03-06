/*JB_Graph.c 
 * Documentation in JB_Graph.h */
#include "JB_Graph.h"

struct neighbor_itr{
  Vertex *home;
  Map_Element *curOut, *curIn;
};
//TODO refactor map_itr into JB_Util, use it in more places.
struct map_itr{
  Map_Element *current;
};


Vertex *VertexMap_Get(VertexMap *this, char *name){
  Map_Element *me = Map_Get(this,name);
  if(me)
    return (Vertex *)me->contents;
  else 
    return NULL;
}
//TODO make this an iterator to avoid quadratic searching
Vertex *VertexMap_GetFirst(VertexMap *this){
  if(this->first != NULL)
    return (Vertex*)this->first->contents;
  return NULL;
}
Vertex *VertexMap_GetNext(VertexMap *this, Vertex *e){
  Map_Element *me = Map_Get(this, Vertex_GetLabel(e));
  if(me == NULL || me->next == NULL)
    return NULL;
  return (Vertex*) me->next->contents;
}

void VertexMap_Add(VertexMap *this, Vertex *v){
  Map_Element *me = jballoc(sizeof(Map_Element));
  me->id = Vertex_GetLabel(v);
  me->contents =v;
  Map_Add(this, me);
}

char *VertexMap_ContentPrinter(Map_Element *me){
  char *ret, *id, *vertexName;
  id = me->id;
  vertexName = Vertex_GetLabel(((Vertex*)me->contents));
  ret = jballoc(strlen(id) + strlen(vertexName) + 4);
  sprintf(ret, "%s \t %s\n", id, vertexName);
  return ret;
}

Edge *EdgeMap_Get(EdgeMap *this, char *src_sink){
  Map_Element *me = Map_Get(this,src_sink);
  if(me)
    return (Edge *)me->contents;
  else
    return NULL;
}
//TODO make this an iterator to avoid quadratic searching
Edge *EdgeMap_GetFirst(EdgeMap *this){
  if(this->first != NULL)
    return (Edge*)this->first->contents;
  return NULL;
}

Edge *EdgeMap_GetNext(EdgeMap *this, Edge *e){
  Map_Element *me = Map_Get(this, Edge_GetSrcSink(e));
  if( me == NULL || me->next == NULL)
    return NULL;
  return (Edge*) me->next->contents;
}

void EdgeMap_Add(EdgeMap *this, Edge *e){
  Map_Element *me = jballoc(sizeof(Map_Element));
  me->id = Edge_GetSrcSink(e);
  me->contents =e;
  Map_Add(this, me);
}

char *EdgeMap_ContentPrinter(Map_Element *me){
  char *ret, *id,*srcsink;
  id = me->id;
  srcsink = Edge_GetSrcSink(((Edge*)me->contents));
  ret = jballoc(strlen(id) + strlen(srcsink) + 4);
  sprintf(ret, "%s \t %s\n", id, srcsink);
  return ret;
}

Vertex *Vertex_Create(Graph *owner, char *name){
  Vertex *ret =  jballoc(sizeof(Vertex)); 
  ret->owner = owner;
/*   ret->node = NULL; */
  ret->elim_set = NULL;
  ret->label = String_Create(name);
  ret->outedges = Map_Create();
  ret->inedges = Map_Create();
  return ret;
}

void Vertex_SetElimSet(Vertex *this, void *es){
  this->elim_set = es;
}
void *Vertex_GetElimSet(Vertex *this){
  return this->elim_set;
}

/* Vertex *Vertex_Wrap(Graph *owner, char *name, Agnode_t *node){ */
/*   Vertex *ret =  jballoc(sizeof(Vertex));  */
/*   ret->node = node; */
/*   ret->outedges = Map_Create(); */
/*   ret->inedges = Map_Create(); */
/*   ret->owner = owner; */
/*   ret->label = String_Create(name); */
/*   return ret; */
/* } */

char *Vertex_GetLabel(Vertex *v){
  return v->label;
}

void Vertex_Destroy(Vertex *v){
  jbfree(v);
}

Edge *Edge_Create(Graph *owner, Vertex *src, Vertex *sink){
  char *srcname, *sinkname;
  Edge *ret =  jballoc(sizeof(Edge));
  ret->owner = owner;
  ret->src = src;
  ret->sink = sink;
/*   ret->edge = NULL; */
  srcname =Vertex_GetLabel(src);
  sinkname =Vertex_GetLabel(sink);
  ret->src_sink = jballoc(strlen(srcname)
			 +strlen(sinkname)
			 +2);//one for space, one for term NULL
  ret->src_sink[0] = (char)0;
  strcat(ret->src_sink,srcname);
  strcat(ret->src_sink," ");
  strcat(ret->src_sink,sinkname);
  return ret;
}

void Edge_SetElimSet(Edge *this, void *es){
  this->elim_set = es;
}
void *Edge_GetElimSet(Edge *this){
  return this->elim_set;
}

/* Edge *Edge_Wrap(Graph *owner, Vertex *src, Vertex *sink, Agedge_t *edge){ */
/*   char *srcname, *sinkname; */
/*   Edge *ret = jballoc(sizeof(Edge)); */
/*   ret->edge = edge; */
/*   ret->owner = owner; */
/*   srcname =Vertex_GetLabel(src); */
/*   sinkname =Vertex_GetLabel(sink); */
/*   ret->src_sink = jballoc(strlen(srcname) */
/* 			  +strlen(sinkname) */
/* 			  +2);//one for space, one for term NULL */
/*   strcat(ret->src_sink,srcname); */
/*   strcat(ret->src_sink," "); */
/*   strcat(ret->src_sink,sinkname); */
/*   return ret; */
  
/* } */

void Edge_Destroy(Edge *this){
  jbfree(this->src_sink);
  jbfree(this);
}

//note that the graph object DOES NOT use this for the key into it's hash
/* char* Edge_GetLabel(Edge *this){ */
/*   if(this->edge) */
/*     return agnameof(this->edge); */
/*   else  */
/*     return NULL; */
/* } */

char* Edge_GetSrcSink(Edge *this){
  return this->src_sink;
}
Vertex* Edge_GetTail(Edge *this){
  return this->src;
}
Vertex* Edge_GetHead(Edge *this){
  return this->sink;
} 

Graph *Graph_common_init(){
  Graph *ret = jballoc(sizeof(Graph));
  ret->vertices=Map_Create();
  ret->edges=Map_Create();
  ret->graph_dirty = TRUE;
  return ret;
}
/* Graph *Graph_Wrap(Agraph_t *graph ){ */
/*   Graph* ret = Graph_common_init(); */
/*   ret->graph = graph; */
/*   ret->directed = agisdirected(graph); */

/*   //Iterate through the vertices of the agraph_t, adding each in turn */
/*   //While at each vertex, add all each of its parents and the filial bonds */
/*   //There's some redundancy, but I find it hard to believe it'll bottleneck */
/*   Agnode_t *cur, *curParent; */
/*   Vertex *curV, *curParentV; */
/*   Agedge_t *filialBond; */
/*   for( cur = agfstnode(graph); */
/*        cur != NULL; */
/*        cur = agnxtnode(graph, cur) ) { */
/*     curV = Graph_AddVertex(ret, agnameof(cur)); */
/*     for ( filialBond = agfstin(graph, cur); */
/* 	  filialBond != NULL; */
/* 	  filialBond = agnxtin(graph,filialBond)){ */
/*       curParent = agtail(filialBond); */
/*       curParentV = Graph_AddVertex(ret, agnameof(curParent) ); */
/*       Graph_AddEdge(ret,curParentV,curV); */
/*     } */
/*   } */
/*   ret->graph_dirty = FALSE; */
/*   return ret; */
/* } */

/* // points g->graph at an equivalent agraph_t */
/* void Graph_Unwrap(Graph *g){ */
/*   if(g->graph_dirty){ */
/*     Agdesc_t desct = (Graph_IsDirected(g)) ? Agstrictdirected */
/*       :Agstrictundirected; */
/*     Agraph_t *graph =  agopen(g->name,desct,NULL); */
/*     Agnode_t *curN, *curParentN; */
/*     g->graph =graph; */
/*     Vertex *cur, *curParent; */
/*     for( cur = Graph_GetFstVertex(g); */
/* 	 cur != NULL; */
/* 	 cur = Graph_GetNxtVertex(g,cur)){ */
/*       curN = agnode(graph,Vertex_GetLabel(cur), TRUE); */
/*       for(curParent = Graph_GetFstParent(g,cur); */
/* 	  curParent != NULL; */
/* 	  curParent = Graph_GetNxtParent(g,cur, curParent)){ */
/* 	  curParentN = agnode(graph,Vertex_GetLabel(curParent), TRUE); */
/* 	  agedge(graph,curParentN,curN, NULL, TRUE); */
/*       } */
/*     } */
/*     g->graph_dirty = FALSE; */
/*     return; */
/*   } */
/*   else */
/*     return;//Graph already has a cgraph version  */
/* } */

Graph *Graph_Create(char *name, bool directed){
  Graph* ret = Graph_common_init();
  ret->name = name;
  ret->directed = directed;
  return ret;
}

Graph *Graph_Copy(Graph *this){
  Graph *fresh = Graph_Create(this->name, Graph_IsDirected(this));
  Vertex *cur, *curParent;
  Vertex *curF, *curParentF;
  for( cur = Graph_GetFstVertex(this);
       cur != NULL;
       cur = Graph_GetNxtVertex(this,cur)){
    curF = Graph_AddVertex(fresh,Vertex_GetLabel(cur));
    for(curParent = Graph_GetFstParent(this,cur);
	curParent != NULL;
	curParent = Graph_GetNxtParent(this,cur, curParent)){
      curParentF = Graph_AddVertex(fresh,Vertex_GetLabel(curParent) );
      Graph_AddEdge(fresh,curParentF,curF);
    }
  }

  fresh->graph_dirty = TRUE;
  return fresh;
}

char* Graph_GetName(Graph *this){
  return this->name;
}

int Graph_GetNumVertices(Graph *this){
  return Map_Size(this->vertices);
}
int Graph_GetNumEdges(Graph *this){
  return Map_Size(this->edges);
}

void Graph_SetDirected(Graph *this, bool setting){
  this->directed = setting;
}

/* /\*Reads in a graph in .dot format from filename*\/ */
/* Graph *Graph_Read(char* filename){ */
/*   FILE *ifp = fopen(filename, "r"); */
/*   Agraph_t *reddit = agread(ifp, NULL); */
/*   Graph *ret = Graph_Wrap(reddit); */
/*   fclose(ifp); */
/*   return ret; */
/* } */

/*TODO: Look at what the return from this means, it's probably just an errno*/
/* int Graph_Write(Graph *g, char *filename){ */
/*   FILE *ofp = fopen(filename, "w"); */
/*   Graph_Unwrap(g); */
/*   Agraph_t *agraph_g = g->graph; */
/*   int ret =agwrite(agraph_g, ofp); */
/*   fclose(ofp); */
/*   return ret; */
/* } */

int Graph_Destroy(Graph *g){
  int ret = 0;
/*   if(g->graph) */
/*     ret = agclose(g->graph); */
  //Memory leak city!
  jbfree(g);
  return ret;
}

/*We do a tmpfile shuffle here because  graphviz implements
  text output to files rather than buffers... grr*/
//OS-DEPENDENT: stat
/* char *Graph_ToGraphViz(Graph *g){ */
/*   int i; */
/*   struct stat file_status; */
/*   FILE *temp; */
/*   Graph_Write(g, ".jbtmpfile"); */
/*   fclose(temp); */
/*   temp = fopen(".jbtmpfile","w"); */
/*   if(stat(".jbtmpfile", &file_status) != 0){ */
/*     reportError( ERRSTRING(JB_Graph.c, Graph_ToGraphViz, STATFAILED) ); */
/*   } */
/*   char *ret = jballoc(file_status.st_size +1); */
/*   for(i = 0; i < file_status.st_size; i++){ */
/*     ret[i] = getc(temp); */
/*   } */
/*   ret[i] = 0; */
/*   return ret; */
/* } */
void Graph_ConnectAll(Graph *g, List *l){
  if(l->size > 0){
    Vertex *nb1, *nb2;
    ListNode *itr1,*itr2;
    for(itr1 = l->first; itr1->next != NULL; itr1 = itr1->next){
      nb1 = (Vertex*)itr1->contents;  
      for(itr2 = itr1->next; itr2 != NULL; itr2 = itr2->next){
	nb2 = (Vertex*)itr2->contents;
	Graph_AddEdge(g, nb1, nb2);
	Graph_AddEdge(g, nb2, nb1);
      }
    }
  }
}

int Vertex_Totdegree(Vertex *v){
  return Map_Size(v->outedges)+Map_Size(v->inedges);
}

int Graph_VertexIndegree(Graph *g, Vertex *v){
  return Map_Size(v->outedges);
}

int Graph_VertexOutdegree(Graph *g, Vertex *v){
  return Map_Size(v->inedges);
}

int Graph_VertexTotdegree(Graph *g, Vertex *v){
  return Map_Size(v->outedges)+Map_Size(v->inedges);
}


Vertex *Graph_GetFstVertex(Graph *g ){
  return VertexMap_GetFirst(g->vertices);
}

Vertex *Graph_GetNxtVertex(Graph *g, Vertex *v ){
  return VertexMap_GetNext(g->vertices,v);
}
Edge *Graph_GetFstEdge(Graph *this, Vertex *v ){
  return EdgeMap_GetFirst(this->edges);
}

Edge *Graph_GetNxtEdge(Graph *this, Vertex *v,  Edge *e ){
  return EdgeMap_GetNext(this->edges,e);
}


bool Graph_IsDirected(Graph *g){
    return g->directed;
}
//TODO replace all GetFirst/GetNext with versions of this.
NeighborItr *NeighborItr_Create(Vertex *v){
  NeighborItr *ret = (NeighborItr *)jballoc(sizeof(NeighborItr));
  ret->home = v;
  ret->curOut = v->outedges->first;
  ret->curIn = v->inedges->first;
  return ret;
}
//TODO this function doesn't depend on graph g at all. refactor
NeighborItr *Graph_GetNeighborItr(Graph *g, Vertex *v){
  return NeighborItr_Create(v);
}

Vertex *NeighborItr_GetCurrent(NeighborItr *this){
  if(this->curIn == NULL){
    if(this->curOut == NULL)
      return NULL;
    else
      return Edge_GetHead((Edge*)this->curOut->contents);
  }
  else{
    return Edge_GetTail((Edge*)this->curIn->contents);
  }
}

Edge *NeighborItr_GetCurrentEdge(NeighborItr *this){
  if(this->curIn == NULL){
    if(this->curOut == NULL)
      return NULL;
    else
      return (Edge*)this->curOut->contents;
  }
  else{
    return (Edge*)this->curIn->contents;
  }
}

Vertex *NeighborItr_Advance(NeighborItr *this){
  if(this->curIn == NULL){
    if(this->curOut == NULL)
      return NULL;
    else{
      this->curOut = this->curOut->next;
      if(this->curOut == NULL)
	return NULL;
      return Edge_GetHead((Edge*)this->curOut->contents);
    }
  }
  else{
    this->curIn = this->curIn->next ;
    return NeighborItr_GetCurrent(this);
  }
}
void NeighborItr_Destroy(NeighborItr *this){
  jbfree(this);
}

VertexItr *VertexItr_Create(Graph *g){
  VertexItr *ret = (VertexItr *)jballoc(sizeof(VertexItr));
  ret->current = g->vertices->first;
  return ret;
}


VertexItr *Graph_GetVertexItr(Graph *g){
  return VertexItr_Create(g);
}

Vertex *VertexItr_GetCurrent(VertexItr *this){
  if(this->current == NULL)
    return NULL;
  return (Vertex*)this->current->contents;
}
Vertex *VertexItr_Advance(VertexItr *this){
  if(this->current == NULL)
    return NULL;
  this->current = this->current->next;
  return VertexItr_GetCurrent(this);
}

void VertexItr_Destroy(VertexItr *this){
  jbfree(this);
}

EdgeItr *EdgeItr_Create(Graph *g){
  EdgeItr *ret = (EdgeItr *)jballoc(sizeof(EdgeItr));
  ret->current = g->edges->first;
  return ret;
}
EdgeItr *Graph_GetEdgeItr(Graph *g){
  return EdgeItr_Create(g);
}

Edge *EdgeItr_GetCurrent(EdgeItr *this){
  if(this->current == NULL)
    return NULL;
  return (Edge*)this->current->contents;
}
Edge *EdgeItr_Advance(EdgeItr *this){
  if(this->current == NULL)
    return NULL;
  this->current = this->current->next;
  return EdgeItr_GetCurrent(this);
}
void EdgeItr_Destroy(EdgeItr *this){
  jbfree(this);
}

//We iterate through 'neighbors' in an undirected graph in 
//the order of those recorded as outedges first, followed by those
//recorded as inedges.
/*Vertex *Graph_GetFstNeighbor(Graph *g, Vertex *v ){
  Vertex *ret;
  if(!(Graph_IsDirected(g)) ){
    EdgeMap *edges = v->outedges;
    if(Map_Size(edges) == 0)
      edges= v->inedges;
    Edge *tmp = EdgeMap_GetFirst(edges);
    if(tmp == NULL)
      //V has NO neighbors
      return NULL;
    ret = (edges == v->outedges) ? Edge_GetHead(tmp) : Edge_GetTail(tmp);
  }
  else{
    ret = NULL; //TODO Graph is directed, signal error
  }
  return ret;
  }*/

/*Vertex *Graph_GetNxtNeighbor(Graph *g, Vertex *v, Vertex *last){
  Vertex *ret;
  Edge *lastEdge,*nextEdge; 
  if(!(Graph_IsDirected(g)) ){
    lastEdge = Graph_GetEdge(g,v,last);
    if(lastEdge != NULL){
      if(Edge_GetTail(lastEdge) == v){
	//Last Neighbor was a child
	nextEdge = EdgeMap_GetNext(v->outedges,lastEdge);
	if(nextEdge != NULL)
	  ret = Edge_GetHead(nextEdge);
	else{

	  //last child, switch to parents
	  nextEdge = EdgeMap_GetFirst(v->inedges);
	  if(nextEdge != NULL)
	    ret = Edge_GetTail(nextEdge);
	  else 
	    ret = NULL;
	}
      }
      else{
	//Last Neighbor was a parent
	nextEdge = EdgeMap_GetNext(v->inedges,lastEdge);
	if(nextEdge != NULL)
	  ret = Edge_GetTail(nextEdge);
	else
	  ret = NULL;
      }   
  }
    else{
      ret= NULL; //Last edge not found, something is very wrong
    }
  }
  else{
    ret = NULL;//TODO Graph is directed, signal error?
  }
    return ret;
    }*/

  Vertex *Graph_GetFstChild(Graph *g, Vertex *v ){
  Vertex *ret = NULL;
  if(Graph_IsDirected(g)){
    Edge *tmp = EdgeMap_GetFirst(v->outedges);
        if(tmp == NULL)
      ret = NULL;
    else
      ret = Edge_GetHead(tmp);
  }
  else{
    ret = NULL; //TODO Graph is undirected, signal error
  }
  return ret;
}

Vertex *Graph_GetNxtChild(Graph *g, Vertex *v, Vertex *last){
    Vertex *ret = NULL;
  if(Graph_IsDirected(g)){
    Edge *tmp = Graph_GetEdge(g, v, last);
    tmp = EdgeMap_GetNext(v->outedges,tmp);
    if(tmp == NULL)
      ret = NULL;
    else
      ret = Edge_GetHead(tmp);
  }
  else{
    ret = NULL; //TODO Graph is undirected, signal error
  }
  return ret;
}

Vertex *Graph_GetFstParent(Graph *g, Vertex *v ){
 Vertex *ret; 
 Edge *tmp = EdgeMap_GetFirst(v->inedges);
 if(tmp == NULL)
   ret = NULL;
 else
   ret = Edge_GetTail(tmp);
 return ret;
} 

Vertex *Graph_GetNxtParent(Graph *g, Vertex *v, Vertex *last){
  Vertex *ret;
  Edge *tmp = Graph_GetEdge(g, last, v);
  tmp = EdgeMap_GetNext(v->inedges,tmp);
  if(tmp == NULL)
    ret = NULL;
  else
    ret = Edge_GetTail(tmp);
  return ret;
} 

Vertex *Graph_GetVertex(Graph *g, char *name ){
  return VertexMap_Get(g->vertices,name);
}
  
Vertex *Graph_AddVertex(Graph *g, char *name){
  g->graph_dirty = TRUE; 
  Vertex *ret =  VertexMap_Get(g->vertices, name);
  if(ret == NULL)
    ret = Vertex_Create(g, name);
  VertexMap_Add(g->vertices, ret);
  return ret;
}
char *srcsink(char *src, char *sink){
  char *ret = jballoc( (strlen(src)+strlen(sink) +2)*sizeof(char));
  ret[0] = (char)0;
  strcat(ret, src);
  strcat(ret, " ");
  strcat(ret, sink);
  return ret;
}
Edge *Graph_AddEdge(Graph *g, Vertex *src, Vertex *sink ){
  Edge *ret =Graph_GetEdge(g, src,sink );
  if(ret == NULL){
    g->graph_dirty = TRUE; 
    //The edge isn't present, so add it
    ret = Edge_Create(g, src, sink);
    EdgeMap_Add(g->edges,ret);
    EdgeMap_Add(src->outedges,ret);
    EdgeMap_Add(sink->inedges,ret);
  }
  return ret;
}


Edge *Graph_GetEdge(Graph *g, Vertex *src, Vertex *sink ){
  char *srcsink_str = srcsink(Vertex_GetLabel(src),Vertex_GetLabel(sink));
  Edge *ret = EdgeMap_Get(g->edges,srcsink_str);
  /* We store undirected edges in a redundant way, to make neighbor iteration
     efficient and independent of which vertex you are iterating over
    if(ret == NULL){
    if(!Graph_IsDirected(g)){ //For undirected graphs, look under both orders
      char *sinksrc_str = srcsink(Vertex_GetLabel(sink),Vertex_GetLabel(src));
      ret = EdgeMap_Get(g->edges,sinksrc_str);
      jbfree(sinksrc_str);
    }
    }*/
  jbfree(srcsink_str);
  return ret;
}


