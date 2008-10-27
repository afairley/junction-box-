/* JB_Graph.h
 * Contains structs and typedefs for working with graphs, as well as
 * function declarations.  
 * Version 0.01: Graphs are implemented by wrapping libcgraph from graphviz
 * This buys us a free file format, which can be conveniently visualized */
#ifndef H_JB_GRAPH
#define H_JB_GRAPH
#include <stdio.h>
#include <sys/stat.h>
#include "JB_Util.h"

typedef struct vertex Vertex;
typedef struct edge Edge;
typedef struct graph Graph;
typedef struct neighbor_itr NeighborItr;
typedef struct map_itr VertexItr;
typedef struct map_itr EdgeItr;
typedef Map VertexMap;
typedef Map EdgeMap;

struct vertex{
 /*  Agnode_t *node; */
  char *label;
  EdgeMap *outedges;
  EdgeMap *inedges;
  Graph *owner;
  void *elim_set;
  void *variable;
  void *pot;
  void *margMemo;
};

struct edge{
/*   Agedge_t *edge; */
  char *src_sink;//the name of the src of the edge follwed by a space, followed by the sink name
                 //src_sink is the hashkey for the graph to track the edge by
  Vertex *src;
  Vertex *sink;
  Graph *owner;
  void *elim_set;
  void *pot;
  void *mulMemo;
};

struct graph{
/*   Agraph_t *graph; */
  bool graph_dirty;
  char directed;
  char *name;
  //Keep pointers to all vertex and edge objects that belong to this graph
  VertexMap *vertices;
  EdgeMap *edges;
};

/***********************************************************************
 * Freeing the pointers returned by all of these functions is the
 * responsibility of the caller  
 ***********************************************************************/
Vertex *Vertex_Create(Graph *owner, char *name);
void Vertex_Destroy(Vertex *v);
char *Vertex_GetLabel(Vertex *v);
int Vertex_Indegree(Vertex *v);
int Vertex_Outdegree(Vertex *v);
int Vertex_Totdegree(Vertex *v);
void Vertex_SetElimSet(Vertex *this, void *es);
void *Vertex_GetElimSet(Vertex *this);
Edge *Edge_Create(Graph *owner, Vertex *src, Vertex *sink);
/* Edge *Edge_Create_cgraph(Agedge_t *edge); */
void Edge_SetElimSet(Edge *this, void *es);
void *Edge_GetElimSet(Edge *this);
void  Edge_Destroy(Edge *e);
char* Edge_GetLabel(Edge *e);
Vertex* Edge_GetTail(Edge *e); 
Vertex* Edge_GetHead(Edge *e); 
char *Edge_GetSrcSink(Edge *this);

/* Graph *Graph_Wrap(Agraph_t *graph); */
Graph *Graph_Create(char *name, bool directed);
Graph *Graph_Copy(Graph *this);
int Graph_Destroy(Graph *g);
char* Graph_GetName(Graph *v);
int Graph_GetNumVertices(Graph *this);
int Graph_GetNumEdges(Graph *this);
void Graph_SetDirected(Graph *this, bool setting);
/* Graph *Graph_Read(char* filename); */
/* int Graph_Write(Graph *g, char* filename); */
/* char *Graph_ToGraphViz(Graph *g); */
Vertex *Graph_GetFstVertex(Graph *g );
Vertex *Graph_GetNxtVertex(Graph *g, Vertex *v );

NeighborItr *Graph_GetNeighborItr(Graph *g, Vertex *v);
Vertex *NeighborItr_GetCurrent(NeighborItr *this);
Edge *NeighborItr_GetCurrentEdge(NeighborItr *this);
Vertex *NeighborItr_Advance(NeighborItr *this);
void NeighborItr_Destroy(NeighborItr *this);

VertexItr *Graph_GetVertexItr(Graph *g);
Vertex *VertexItr_GetCurrent(VertexItr *this);
Vertex *VertexItr_Advance(VertexItr *this);
void VertexItr_Destroy(VertexItr *this);

EdgeItr *Graph_GetEdgeItr(Graph *g);
Edge *EdgeItr_GetCurrent(EdgeItr *this);
Edge *EdgeItr_Advance(EdgeItr *this);
void EdgeItr_Destroy(EdgeItr *this);

Vertex *Graph_GetFstChild(Graph *g, Vertex *v );
Vertex *Graph_GetNxtChild(Graph *g, Vertex *v, Vertex *last);
Vertex *Graph_GetFstParent(Graph *g, Vertex *v );
Vertex *Graph_GetNxtParent(Graph *g, Vertex *v, Vertex *last);
Edge *Graph_GetFstEdge(Graph *this, Vertex *v );
Edge *Graph_GetNxtEdge(Graph *this, Vertex *v, Edge *e );

Vertex *Graph_AddVertex(Graph *g, char *name);
void Graph_AddVertex_Void(Graph *g, char *name);
Edge *Graph_AddEdge(Graph *g, Vertex *source, Vertex *sink );
void Graph_ConnectAll(Graph *g, List *l);
void Graph_AddEdge_Void(Graph *g, Vertex *source, Vertex *sink);
Vertex *Graph_GetVertex(Graph *g, char *name );
Edge *Graph_GetEdge(Graph *g, Vertex *source, Vertex *sink );

int Graph_VertexIndegree(Graph *g, Vertex *v);
int Graph_VertexOutdegree(Graph *g, Vertex *v);
int Graph_VertexTotdegree(Graph *g, Vertex *v);
bool Graph_IsDirected(Graph *g);
char *VertexMap_ContentPrinter(Map_Element *me);
char *EdgeMap_ContentPrinter(Map_Element *me);
#endif
