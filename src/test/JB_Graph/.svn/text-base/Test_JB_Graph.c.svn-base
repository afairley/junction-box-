#include"JB_Graph.h"
#include"check.h"
#include<string.h>
#include<stdlib.h>

/******************************************************************************
 ******************************************************************************
 *** Globals
 ******************************************************************************
 *****************************************************************************/

static char *jbhome;
/*Relative Paths to resources from $JBHOME*/
static char *Rel_testGraph1="res/test/JB_Graph/testGraph1.dot"; 
static char *Rel_testGraph2="res/test/JB_Graph/testGraph2.dot"; 
/*Pointers for full paths to resources from $JBHOME*/
static char *testGraph1, *testGraph2;
 
//Prototypes
Suite *graph_suite (void);
/******************************************************************************
 ******************************************************************************
 *** Test Setup Code
 ******************************************************************************
 *****************************************************************************/
int main (void)
{
  int number_failed;
  Suite *s = graph_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


void setup_resource_paths(){
  jbhome = getenv("JBHOME");
  testGraph1 = strcatLK(jbhome,Rel_testGraph1, "/");
  testGraph2 = strcatLK(jbhome,Rel_testGraph2, "/");
}
//Initialize all globals with a simple graph
//I realized I'm doing this all over the place, so I'll DRY
//as I change tests...It'd be better form to have this initialize
//a struct that got passed around, but no point in changing it right now
Graph *Build_Simple_Graph(){
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  int i;

  g = Graph_Create("GraphFromScratch", TRUE);
  for(i = 0; i < 5; i++){
    g_vertices[i] =Graph_AddVertex(g, vertexNames[i]);
  }
  for(i =0; i < 5; i++){
    int *indx = edgeIndices[i];
    Graph_AddEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
  }
  return g;
}
void setup (void)
{  
  setup_resource_paths();
}

void teardown (void)
{
}

/******************************************************************************
 ******************************************************************************
 *** Tests 
 ******************************************************************************
 *****************************************************************************/
/*Test_Graph_Read incidentally tests:
 * Graph_GetVertex
 * Graph_GetEdge
 * Vertex_GetLabel
 */
START_TEST (Test_Graph_Read)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  Vertex *v;
  Edge *e;
  int i;
  char *msg;  
  
   g = Graph_Read(testGraph1);
  fail_unless( g != NULL, "Graph_Read returned NULL");
  for(i =0; i < 4; i++){
    g_vertices[i] = Graph_GetVertex(g,vertexNames[i]);
    v = g_vertices[i];
    fail_unless( (v != NULL), "Graph is missing a vertex ", vertexNames[i]);
    fail_unless(  strcmp(Vertex_GetLabel(v) , vertexNames[i])  == 0 ,
		  "Mislabeled vertex.",  vertexNames[i]);
  }
  for(i =0; i < 3; i++){
    int *indx = edgeIndices[i];
    e = Graph_GetEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
    msg = strcatLK("Missing expected edge ", 
		   vertexNames[indx[0]] ,"");
    msg = strcatLK(msg, vertexNames[indx[1]], "->");
    fail_unless( (e!=NULL) , msg);

    msg = strcatLK("","Edge Name Corruption" ,"");
    msg = strcatLK(msg, 
		   strcatLK(vertexNames[indx[0]], vertexNames[indx[1]],  "->"),
		   " ");
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetTail(e)) ,
			 vertexNames[indx[0]])  == 0 ,
		  msg);
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetHead(e)) ,
			 vertexNames[indx[1]])  == 0 ,
		  msg);


  }
}
END_TEST

START_TEST (Test_Graph_Copy)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *o,*g;
  Vertex *g_vertices[5];
  Vertex *v;
  Edge *e;
  int i;
  char *msg;  
  o = Build_Simple_Graph();
  g = Graph_Copy(o);
  fail_unless( g != NULL, "Graph_Copy returned NULL");
  for(i =0; i < 4; i++){
    g_vertices[i] = Graph_GetVertex(g,vertexNames[i]);
    v = g_vertices[i];
    fail_unless( (v != NULL), "Graph is missing a vertex ", vertexNames[i]);
    fail_unless(  strcmp(Vertex_GetLabel(v) , vertexNames[i])  == 0 ,
		  "Mislabeled vertex.",  vertexNames[i]);
  }
  for(i =0; i < 3; i++){
    int *indx = edgeIndices[i];
    e = Graph_GetEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
    msg = strcatLK("Missing expected edge ", 
		   vertexNames[indx[0]] ,"");
    msg = strcatLK(msg, vertexNames[indx[1]], "->");
    fail_unless( (e!=NULL) , msg);

    msg = strcatLK("","Edge Name Corruption" ,"");
    msg = strcatLK(msg, 
		   strcatLK(vertexNames[indx[0]], vertexNames[indx[1]],  "->"),
		   " ");
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetTail(e)) ,
			 vertexNames[indx[0]])  == 0 ,
		  msg);
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetHead(e)) ,
			 vertexNames[indx[1]])  == 0 ,
		  msg);


  }
}
END_TEST


/* START_TEST (Test_Graph_ToGraphViz) */
/* { */
/* } */
/* END_TEST */

/*Test_Graph_Read incidentally tests:
 *Graph_AddVertex
 *Graph_AddEdge
 *Graph_GetVertex
 *Graph_GetEdge
 *Vertex_GetLabel
 */
START_TEST (Test_Graph_Write)
{ 
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  Vertex *v;
  Edge *e;
  int i;
  char *msg;

  //Allocate a fresh graph in memory.DIFFERENT THAN Build_Simple_Graph()
  g = Graph_Create("GraphFromScratch", TRUE);
  for(i = 0; i < 5; i++){
    g_vertices[i] =Graph_AddVertex(g, vertexNames[i]);
  }
  for(i =0; i < 3 ; i++){
    int *indx = edgeIndices[i];
    Graph_AddEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
  }

  //Add vertex and edges to graph and write to file
  g_vertices[4] = Graph_AddVertex(g, "E");
  Graph_AddEdge(g, g_vertices[4],g_vertices[3]);
  Graph_AddEdge(g, g_vertices[4],g_vertices[2]);
  Graph_Write(g, testGraph2);
  Graph_Destroy(g);
  g = Graph_Read(testGraph2);
  //Verify that written graph has appropriate structure
    fail_unless( g != NULL, "Graph_Read returned NULL");
  for(i =0; i < 5; i++){
    g_vertices[i] = Graph_GetVertex(g,vertexNames[i]);
    v = g_vertices[i];
    fail_unless( (v != NULL), "Graph is missing a vertex ", vertexNames[i]);
    fail_unless(  strcmp(Vertex_GetLabel(v) , vertexNames[i])  == 0 ,
		  "Mislabeled vertex.",  vertexNames[i]);
  }
  for(i =0; i < 5; i++){
    int *indx = edgeIndices[i];
    e = Graph_GetEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
    msg = strcatLK("Missing expected edge ", 
		   vertexNames[indx[0]] ,"");
    msg = strcatLK(msg, vertexNames[indx[1]], "->");
    fail_unless( (e!=NULL) , msg);

    msg = strcatLK("","Edge Name Corruption" ,"");
    msg = strcatLK(msg, 
		   strcatLK(vertexNames[indx[0]], vertexNames[indx[1]],  "->"),
		   " ");
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetTail(e)) ,
			 vertexNames[indx[0]])  == 0 ,
		  msg);
    fail_unless(  strcmp(Vertex_GetLabel(Edge_GetHead(e)) ,
			 vertexNames[indx[1]])  == 0 ,
		  msg);
  }

}
END_TEST
START_TEST (Test_Graph_AddVertex )
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  Graph *g;
  Vertex *g_vertices[5];
  int i;
  
  g = Graph_Create("GraphFromScratch", TRUE);
  g_vertices[0] =Graph_AddVertex(g, vertexNames[i]);
  fail_unless(g_vertices[0] != NULL);
}
END_TEST
START_TEST (Test_Graph_GetVertex)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  Graph *g;
  Vertex *g_vertices[5];
  Vertex *v;

  g = Graph_Create("GraphFromScratch", TRUE);
  g_vertices[0] =Graph_AddVertex(g, vertexNames[0]);
  fail_unless(g_vertices[0] != NULL);
  v = Graph_GetVertex(g, vertexNames[0]); 
  fail_unless(strcmp(Vertex_GetLabel(v), vertexNames[0]) == 0 );
}
END_TEST
START_TEST (Test_Graph_AddEdge)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  Edge *e;
  int i;
  
  g = Graph_Create("GraphFromScratch",TRUE);
  for(i = 0; i < 2; i++){
    g_vertices[i] =Graph_AddVertex(g, vertexNames[i]);
  }
  for(i =0; i < 1; i++){
    int *indx = edgeIndices[i];
    e = Graph_AddEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
  }
  fail_unless( e != NULL );
}
END_TEST

/* START_TEST (Test_Graph_AddEdge_Void) */
/* { */
/* } */
/* END_TEST */
/* START_TEST (Test_Graph_AddVertex_Void) */
/* { */
/* } */
/* END_TEST */

START_TEST (Test_Graph_GetFstChild)
{
  Graph *g;
  Vertex *u,*v;

  g = Build_Simple_Graph();
  //Ensure that child is as expected
  v = Graph_GetVertex(g,"A");
  u = Graph_GetFstChild(g,v);
  //Error msg is the actual label of u
  fail_unless(strcmp("B", Vertex_GetLabel(u))==0, Vertex_GetLabel(u));
}
END_TEST
//TODO rename test to reflect itr...
START_TEST (Test_Graph_GetFstNeighbor)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  Vertex *u,*v;
  int i;
  
  g = Graph_Create("GraphFromScratch",FALSE);
  for(i = 0; i < 5; i++){
    g_vertices[i] =Graph_AddVertex(g, vertexNames[i]);
  }
  for(i =0; i < 5; i++){
    int *indx = edgeIndices[i];
    Graph_AddEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
  }
  v = Graph_GetVertex(g,"A");
  NeighborItr * itr =  Graph_GetNeighborItr(g,v);
  u =NeighborItr_GetCurrent(itr);
  //Error msg is the actual label of u
  fail_unless(strcmp("B", Vertex_GetLabel(u))==0, Vertex_GetLabel(u));
}
END_TEST
START_TEST (Test_Graph_GetFstParent)
{
  Graph *g;
  Vertex *u,*v;
  g = Build_Simple_Graph();
  v = Graph_GetVertex(g,"B");
  u = Graph_GetFstParent(g,v);
  //Error msg is the actual label of u
  fail_unless(strcmp("A", Vertex_GetLabel(u))==0, Vertex_GetLabel(u));
}
END_TEST
/* START_TEST (Test_Graph_GetFstVertex) */
/* { */

/* } */
/* END_TEST */
/* START_TEST (Test_Graph_GetNxtVertex) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */
START_TEST (Test_Graph_GetNxtChild)
{
  Graph *g;
  Vertex *u,*v;
  int flag = 0;
  g = Build_Simple_Graph();
  //Ensure that 2nd child is as expected
  v = Graph_GetVertex(g,"B");
  u = Graph_GetFstChild(g,v);
  u = Graph_GetNxtChild(g,v,u);
  //u should now point to C or D. We make no assumption about ordering
  flag = ( (strcmp("C", Vertex_GetLabel(u))==0) 
                        ||
           (strcmp("D", Vertex_GetLabel(u))==0) );
  //Error msg is the actual label of u
  fail_unless(flag, Vertex_GetLabel(u));
}
END_TEST
START_TEST (Test_Graph_GetNxtNeighbor)
{
  char *vertexNames[5] = {"A","B","C","D",
			  "E"             };
  int edgeIndices[5][2] = {{0,1},{1,2},{1,3},
			   {4,2},{4,3}	 };
  Graph *g;
  Vertex *g_vertices[5];
  Vertex *u,*v;
  int i;
  int flag = 0;
  g = Graph_Create("GraphFromScratch",FALSE);
  for(i = 0; i < 5; i++){
    g_vertices[i] =Graph_AddVertex(g, vertexNames[i]);
  }
  for(i =0; i < 5; i++){
    int *indx = edgeIndices[i];
    Graph_AddEdge(g,g_vertices[indx[0]],g_vertices[indx[1]]);
  }
  //Ensure that 2nd child is as expected
  v = Graph_GetVertex(g,"B");
  NeighborItr * itr =  Graph_GetNeighborItr(g,v);
  u =NeighborItr_Advance(itr);
  //u should now point to A,C, or D. We make no assumption about ordering
  flag = ( ( strcmp("A", Vertex_GetLabel(u))==0) 
                        ||
	   ( strcmp("C", Vertex_GetLabel(u))==0) 
                        ||
	  (strcmp("D", Vertex_GetLabel(u))==0) ) ;
  //Error msg is the actual label of u
  fail_unless(flag, Vertex_GetLabel(u));
}
END_TEST
START_TEST (Test_Graph_GetNxtParent)
{
  Graph *g;
  Vertex *u,*v;
  int flag = 0;
  g = Build_Simple_Graph();
  //Ensure that 2nd child is as expected
  v = Graph_GetVertex(g,"C");
  u = Graph_GetFstParent(g,v);
  u = Graph_GetNxtParent(g,v,u);
  //u should now point to B, or E. We make no assumption about ordering
  flag =(  (strcmp("B", Vertex_GetLabel(u))==0) 
                        ||
         (strcmp("E", Vertex_GetLabel(u))==0) );
  //Error msg is the actual label of u
  fail_unless(flag, Vertex_GetLabel(u));
}
END_TEST

START_TEST (Test_Graph_Create_Undirected)
{
  Graph *g = Graph_Create("No direction, son", FALSE);
  fail_unless(!Graph_IsDirected(g), "Graph was directed, it shouldn't be.\n");
}
END_TEST
/* START_TEST (Test_Graph_Destory) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */
/* START_TEST (Test_Edge_Create) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */
/* START_TEST (Test_Edge_Destroy) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */

/* START_TEST (Test_Vertex_Create) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */
/* START_TEST (Test_Vertex_Destroy) */
/* { */
/*   //TODO Add a trivial test for this */
/* } */
/* END_TEST */

Suite *
graph_suite (void)
{
  Suite *s = suite_create ("Graph");
  /* Core test case */
  TCase *tc_core = tcase_create ("GraphCore");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core,   Test_Graph_Create_Undirected);
  tcase_add_test (tc_core, Test_Graph_AddVertex );
  tcase_add_test (tc_core, Test_Graph_GetVertex);
  tcase_add_test (tc_core, Test_Graph_GetFstChild);
  tcase_add_test (tc_core, Test_Graph_GetFstNeighbor);
  tcase_add_test (tc_core, Test_Graph_GetFstParent);
  tcase_add_test (tc_core, Test_Graph_GetNxtChild);
  tcase_add_test (tc_core, Test_Graph_GetNxtNeighbor);
  tcase_add_test (tc_core, Test_Graph_GetNxtParent);
  tcase_add_test (tc_core,  Test_Graph_Copy);
  tcase_add_test (tc_core, Test_Graph_Read);
  tcase_add_test (tc_core, Test_Graph_Write);
  tcase_add_test (tc_core, Test_Graph_AddEdge);
  //  tcase_add_test (tc_core, Test_Edge_Create);
  //  tcase_add_test (tc_core, Test_Edge_Destroy);
  //  tcase_add_test (tc_core, Test_Graph_AddEdge_Void);
  //  tcase_add_test (tc_core, Test_Graph_AddVertex_Void);
  //  tcase_add_test (tc_core, Test_Graph_Create);
  //  tcase_add_test (tc_core, Test_Graph_Destory);
  //  tcase_add_test (tc_core, Test_Graph_GetFstVertex);
  //  tcase_add_test (tc_core, Test_Graph_GetNxtVertex);
  //  tcase_add_test (tc_core, Test_Graph_ToGraphViz);
  //  tcase_add_test (tc_core, Test_Vertex_Create);
  //  tcase_add_test (tc_core, Test_Vertex_Destroy);
  suite_add_tcase (s, tc_core);
  return s;
}




