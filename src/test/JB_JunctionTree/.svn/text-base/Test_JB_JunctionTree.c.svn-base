#include"JB_JunctionTree.h"
#include"check.h"
#include<string.h>
#include<stdlib.h>
#include<check.h>
#include<limits.h>

//Globals
#define NUM_NODES 26
#define NUM_GRAPHS_TO_MORALIZE 200

char *labels[NUM_NODES];
char *msg;
char *jbhome;
char *dirname;

//Prototypes
Suite *jt_suite (void);

//Helper Functions

int vbl_label_cmp( const void *v1, const void *v2){
  return strcmp((*(Variable **)v1)->label,
		(*(Variable **)v2)->label);
}
/*simply iterates through the vertices of g using Graph_GetFstVertex,
  Graph_GetNxtVertex and produces a lexicographically ordered 
  VariableOrdering
  The variables contained in this ordering are dummies,this is only
  for testing purposes.
*/
VariableOrdering *defaultVariableOrdering(Graph *g){
  Vertex * cur;
  int numVariables, i;
  numVariables = Graph_GetNumVertices(g);
  Variable **vbls = jballoc(numVariables * sizeof(Variable*) );
  VariableOrdering *ret = jballoc(sizeof(VariableOrdering));
  for( cur = Graph_GetFstVertex(g),
	 i = 0;
       cur != NULL;
       cur = Graph_GetNxtVertex(g,cur),
       i++){
    vbls[i] = Variable_Create(Vertex_GetLabel(cur), 0, NULL, NULL, NULL);
  }
  qsort(vbls, numVariables,
	sizeof(Variable*),  vbl_label_cmp);
  ret->size = numVariables;
  ret->vbls = vbls;
  return ret;
}

//This just saves me from typing the alphabet
void initLabels(){
  char idx;
  for(idx = 'A'; idx <= 'Z'; idx++){
    char *c = malloc(2 *(sizeof(char)));
    c[0] = idx;
    c[1] = 0;
    labels[idx - 'A'] = c;
  }
}

//This will generate a random DAG
Graph *randomDigraph(){
  int i,j,k;
  int num_nodes = (rand() % NUM_NODES);
  Agraph_t *g;
  Agnode_t *parent, *child;
  Agedge_t *e;
   
  g = agopen("Random", Agdirected, NULL);
  for(i = num_nodes; i >0; i--){
    int numParents = (rand() % i)/2 ;
    int parents[numParents];
    child = agnode(g, labels[i],TRUE);
    for(j = 0; j < numParents; j++){
      int parentNum = (rand() % i);
      int alreadyParent = 0;
      //ensure no node is a parent of the same child twice
      for(k = 0; k < j; k ++){
	if(parents[k] ==  parentNum){
	  alreadyParent = 1;
	  break;
	}
      }
      if(alreadyParent){
	j--;
	continue;
      }
      else{
	parent = agnode(g, labels[parentNum],TRUE);
	parents[j] = parentNum;
	
	e = agedge(g, parent, child, "",TRUE);
	agsafeset(parent, "color", "red", "");
      }
    }
  }
  return Graph_Wrap(g);
}

//Helper function for building error messages on why a graph fails to be moral
char *MoralFailing(char *expl, Vertex *v1, Vertex *v2){
  char *name1 = Vertex_GetLabel(v1);
  char *name2 = Vertex_GetLabel(v2);
  char *ret = strcatLK(expl,name1," ");
  ret = strcatLK(ret,name2," " );
  return ret;
}
//Verify that moral is the moralization of orig
char *CheckMorality(Graph *moral, Graph *orig){
  char *ret = NULL;
  Vertex *current,*currentMoral;
  for(current = Graph_GetFstVertex(orig);
      current != NULL;
      current = Graph_GetNxtVertex(orig, current))
    {
      List *parents = List_Create();
      Vertex *curParent1, *moralParent1;
      ListNode *cursor1, *cursor2;
      currentMoral =Graph_GetVertex(moral, Vertex_GetLabel(current));
      for(curParent1 = Graph_GetFstParent(orig, current);
	  curParent1 != NULL;
	  curParent1 = Graph_GetNxtParent(orig, current, curParent1))
	{
	  moralParent1 = Graph_GetVertex(moral, Vertex_GetLabel(curParent1));
	  Edge *filialBond = Graph_GetEdge(moral, moralParent1, currentMoral);
	      if(filialBond == NULL){
		ret = MoralFailing("A parent child bond is missing,"
				       ,moralParent1,currentMoral); 
		break;
	      }
	  List_Append(parents,curParent1);
	}
        if(ret != 0)
	  break; //An edge is missing to what should be a parent.
       for(cursor1 = parents->first;
	  cursor1 != parents->last;
	  cursor1 = cursor1->next)
	{
	  curParent1 = (Vertex*)cursor1->contents;
	  moralParent1 = Graph_GetVertex(moral, Vertex_GetLabel(curParent1));
	  for(cursor2 = cursor1->next;
	      cursor2 != NULL;
	      cursor2 = cursor2->next)
	    {
	      Vertex *curParent2 = (Vertex*)cursor2->contents;
	      Vertex *moralParent2 = Graph_GetVertex(moral,
						  Vertex_GetLabel(curParent2));
	      Edge *maritalBond = Graph_GetEdge(moral,
						moralParent1, moralParent2);
	      if(maritalBond == NULL){
		ret = MoralFailing("Unmarried parents!,"
				       ,moralParent1,moralParent2); 
		break;
	      }
	    }
	  if(ret != NULL)
	    break; //Unmarried Parents!
	}
       if(ret != NULL)
	 break;
    }
  return ret;
}

/******************************************************************************
 ******************************************************************************
 *** Test Setup Code
 ******************************************************************************
 *****************************************************************************/

void setup_resource_paths(){
  char *resPath = "res/test/JB_JunctionTree";
  jbhome = getenv("JBHOME");
  dirname = pathJoin(jbhome,resPath);  

}



void setup (void)
{ 
   initLabels();
   setup_resource_paths();
}

void teardown (void)
{
 
}

int main (void)
{
  int number_failed;
  Suite *s = jt_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
//TODO Move into TestJB_Util
START_TEST (Test_BitVec_Create){
  BitVector *bv1, *bv2, *bv3, *bv4, *bv5, *bv6;
  bv1 = BitVector_Create(5, NULL,NULL,0);
  //  checkVector(bv1, 0);
  bv2 = BitVector_Create(5, NULL,NULL,BV_INIT_FILL);
  bv3 = BitVector_Create(32, NULL,NULL,0);
  bv4 = BitVector_Create(32, NULL,NULL,BV_INIT_FILL);
  bv5 = BitVector_Create(3200, NULL,NULL,0);
  bv6 = BitVector_Create(3200, NULL,NULL,BV_INIT_FILL);
}END_TEST

START_TEST (Test_JT_ElimSet_Contains){
  Variable *A, *B;
  Vertex *vtxA, *vtxB;
  ElimSet *es1, *es2 , *es3;
  vtxA =Vertex_Create(NULL, "A");
  vtxB =Vertex_Create(NULL, "B");
  A =Variable_Create("A",0,NULL,NULL,NULL);
  B =Variable_Create("B",0,NULL,NULL,NULL);
  List *l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  VariableOrdering *o = VariableOrdering_Create(l);
  
  es1 = ElimSet_Create(o); // will hold just {A}
  es2 = ElimSet_Create(o); // {B}
  es3 = ElimSet_Create(o); // {A,B}
  ElimSet_Add(es1,vtxA);
  ElimSet_Add(es2,vtxB);
  ElimSet_Add(es3,vtxA);
  ElimSet_Add(es3,vtxB);
  fail_unless(ElimSet_Contains(es3, es1), "{A B} didn't contain {A}");
  fail_unless(ElimSet_Contains(es3, es2), "{A B} didn't contain {B}");
  fail_unless(!ElimSet_Contains(es1, es3), "{A B} was contained in {A}");
  fail_unless(!ElimSet_Contains(es2, es3), "{A B} was contained in {B}");
}END_TEST

/*Generate random 26 node DAGS, moralize them, 
 *and verify that they were moralized. 
 *Output dot descriptions of the graphs for visual inspection if
 *so desired*/
START_TEST (Test_JT_MoralGraph){
  int i;
  char *basename1 = jballoc(15);
  char *basename2 = jballoc(15);
  for(i = 0; i < NUM_GRAPHS_TO_MORALIZE; i++){
    //allocate filenames in a location independent way
    //TODO build some utility code that does this work.
    sprintf(basename1, "Graph%d.dot", i);
    sprintf(basename2, "MGraph%d.dot", i);
    char *fullname1 = pathJoin(dirname,basename1);
    char *fullname2 = pathJoin(dirname,basename2);

    //Moralize a random graph
    Graph *immoral = randomDigraph();
    Graph_Write(immoral, fullname1);
    Graph *moral = MoralGraph(immoral);
    Graph_Write(moral, fullname2);

    //Check that the resulting graph is a moralization of the original
    char *flag = CheckMorality(moral, immoral);
    msg = strcatLK("\nThis graph wasn't moralized properly ", fullname1," ");
    if(flag != NULL)
      msg = strcatLK(msg, flag, "\n");
    fail_unless(!flag,msg);
    //TODO Add destructor code
  }
}END_TEST

 /* This test is a hack. I wanted to see that this thing worked
    reasonably well, and didn't want to write a rigorous test
    because that would be about 0.5hr work. So, I wrote a test that
    intentionally fails, and prints out output from which I can verify
    that this  thing works the way I want
START_TEST(Test_JT_VertexOrdering_ToString){
  char *basename1 = jballoc(15);
  sprintf(basename1, "MGraph8.dot");
  int length = strlen(dirname)+strlen(basename1) +2;
  char *fullname1 =jballoc(length);
  fullname1[0]=(char)NULL;
  strcat(fullname1,dirname);
  strcat(fullname1,"/");
  strcat(fullname1,basename1);
  
  Graph *g = Graph_Read(fullname1);
  VertexOrdering *o = defaultVertexOrdering(g);
  char *msg = VertexOrdering_ToString(o);
  fail_unless(FALSE,msg);
}END_TEST
 */
#define NUM_TRI_TESTS 7
START_TEST (Test_JT_GatherElimSets){
  //allocate filenames in a location independent way
  char *testFiles[NUM_TRI_TESTS] ={"TriTest1.dot","TriTest2.dot","TriTest3.dot",
				   "TriTest4.dot","TriTest5.dot","TriTest6.dot",
				   "TriTest7.dot"};
  char *testOutFiles[NUM_TRI_TESTS] = {"TriTestOut1.dot",
				       "TriTestOut2.dot",
				       "TriTestOut3.dot",
				       "TriTestOut4.dot", 
				       "TriTestOut5.dot",
				       "TriTestOut6.dot",
				       "TriTestOut7.dot"};
  char *basename =  pathJoin(dirname,"tri");
  int i;
  for(i =0; i < NUM_TRI_TESTS; i++){
    char *fullname = pathJoin(basename,testFiles[i]);
      
    Graph *in = Graph_Read(fullname);
    Graph *moral = MoralGraph(in);
    Graph *tri = Graph_Copy(moral);
    
    VertexOrdering *o = defaultVertexOrdering(tri);
    VariableOrdering *o2 = defaultVariableOrdering(tri);
    GatherElimSets(tri,o,o2);

  /*  char *msg1 = VertexOrdering_ToString(o);
  char *msg2 = ElimSetCollection_ToString(esc);
  char *msg = jballoc(strlen(msg1) + strlen(msg2) +3);
  msg[0] = (char)NULL;
  strcat(msg,"\n");
  strcat(msg,msg1);
  strcat(msg,"\n");
  strcat(msg,msg2);*/
  
    jbfree(fullname);
    fullname = pathJoin(basename,testOutFiles[i]);
    Graph_Write(tri,fullname);
    jbfree(fullname);
  }
}END_TEST

 //This test requires manual inspection of ReduceTestOut*.txt
 //You should see that only cliques remain.
START_TEST (Test_JT_ReduceElimSets){
  //allocate filenames in a location independent way
  char *testFiles[NUM_TRI_TESTS] =
                        {"TriTest1.dot","TriTest2.dot","TriTest3.dot",
			 "TriTest4.dot","TriTest5.dot","TriTest6.dot",
			"TriTest7.dot"};
  char *testOutFiles[NUM_TRI_TESTS] = {"ReduceTestOut1.txt",
				       "ReduceTestOut2.txt",
				       "ReduceTestOut3.txt",
				       "ReduceTestOut4.txt", 
				       "ReduceTestOut5.txt",
				       "ReduceTestOut6.txt",
  				       "ReduceTestOut7.txt"};
  char *basename =  pathJoin(dirname,"tri");
  int i;
  for(i =0; i < NUM_TRI_TESTS; i++){
    char *fullname = pathJoin(basename,testFiles[i]);
      
    Graph *in = Graph_Read(fullname);
    Graph *moral = MoralGraph(in);
    Graph *tri = Graph_Copy(moral);
    
    VertexOrdering *o = defaultVertexOrdering(tri);
    VariableOrdering *o2 = defaultVariableOrdering(tri);
    ElimSetCollection *esc = GatherElimSets(tri,o,o2);
    char *orig_sets = ElimSetCollection_ToString(esc);
    ElimSetCollection_RemoveNonCliques(esc,o);
    char *reduced_sets = ElimSetCollection_ToString(esc);
    
    jbfree(fullname);
    fullname = pathJoin(basename,testOutFiles[i]);
    FILE *fp = fopen(fullname,"w");
    fputs(fullname,fp);
    fputs("Elimination Ordering\n",fp);
    fputs(VertexOrdering_ToString(o),fp);
    fputs("\n",fp);
    fputs("Elimination Sets before reduction\n",fp);
    fputs("\n",fp);
    fputs(orig_sets,fp);
    fputs("\n",fp);
    fputs("Elimination Sets after reduction\n",fp);
    fputs("\n",fp);
    fputs(reduced_sets,fp);
    jbfree(fullname);
    jbfree(orig_sets);
    jbfree(reduced_sets);
  }
}END_TEST


START_TEST (Test_JT_BuildElimTree){
  //allocate filenames in a location independent way
  char *testFiles[NUM_TRI_TESTS] =
                        {"TriTest1.dot","TriTest2.dot","TriTest3.dot",
			 "TriTest4.dot","TriTest5.dot","TriTest6.dot",
			"TriTest7.dot"};
  char *testOutFiles[NUM_TRI_TESTS] = {"JTBuildTestOut1.dot",
				       "JTBuildTestOut2.dot",
				       "JTBuildTestOut3.dot",
				       "JTBuildTestOut4.dot", 
				       "JTBuildTestOut5.dot",
				       "JTBuildTestOut6.dot",
				       "JTBuildTestOut7.dot"};
  char *basename =  pathJoin(dirname,"tri");
  int i;
  for(i =0; i < NUM_TRI_TESTS; i++){
    char *fullname = pathJoin(basename,testFiles[i]);
      
    Graph *in = Graph_Read(fullname);
    Graph *moral = MoralGraph(in);
    Graph *tri = Graph_Copy(moral);
    
    VertexOrdering *o = defaultVertexOrdering(tri);
    VariableOrdering *o2 = defaultVariableOrdering(tri);
    ElimSetCollection *esc = GatherElimSets(tri,o,o2);
    ElimSetCollection_RemoveNonCliques(esc,o);
    ElimTree *et = ElimTree_Create(esc);
    /*  char *msg1 = VertexOrdering_ToString(o);
	char *msg2 = ElimSetCollection_ToString(esc);
	char *msg = jballoc(strlen(msg1) + strlen(msg2) +3);
	msg[0] = (char)NULL;
	strcat(msg,"\n");
	strcat(msg,msg1);
	strcat(msg,"\n");
	strcat(msg,msg2);*/
    
    jbfree(fullname);
    fullname = pathJoin(basename,testOutFiles[i]);
    Graph_Write(et,fullname);
  }
}END_TEST

Suite *jt_suite (void)
{
  Suite *s = suite_create ("Jt");
  /* Core test case */
  TCase *JTCore = tcase_create ("JTCore");
  tcase_add_checked_fixture (JTCore, setup, teardown);
  tcase_add_test (JTCore, Test_JT_MoralGraph);
  tcase_add_test (JTCore,  Test_JT_ElimSet_Contains);
  tcase_add_test (JTCore, Test_JT_GatherElimSets);
  tcase_add_test (JTCore,Test_JT_ReduceElimSets);
  tcase_add_test (JTCore,  Test_JT_BuildElimTree);
  /*  tcase_add_test (JTCore, Test_JT_VertexOrdering_ToString);*/
  tcase_set_timeout(JTCore, 0);
  suite_add_tcase (s, JTCore);

  TCase *BVCore = tcase_create ("BitVectorCore");
  tcase_add_test (BVCore,  Test_BitVec_Create);
  suite_add_tcase (s, BVCore);
  return s;
}
