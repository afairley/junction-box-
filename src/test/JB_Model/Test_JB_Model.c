#include"JB_Model.h"
#include"check.h"
#include<string.h>
#include<stdlib.h>
#include<check.h>
#include<limits.h>

//Globals
char *jbhome;
char *dirname;
/******************************************************************************
 ******************************************************************************
 *** Test Setup Code
 ******************************************************************************
 *****************************************************************************/

void setup_resource_paths(){
  char *resPath = "res/test/JB_Model";
  jbhome = getenv("JBHOME");
  dirname = pathJoin(jbhome,resPath);  

}



void setup (void)
{ 
   setup_resource_paths();
}

void teardown (void)
{
 
}

START_TEST (Test_CPT_Create){
  Variable *A,*B, *C;
  List *l;
  A = Variable_CreateDiscrete("A",2,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  //Create a table for P(C|A,B)
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);
  double *valueData1 = (double*)jballoc(sizeof(double) *8);
  valueData1[0] = 1.0/2.0;/* A=0, B=0, C=0 */
  valueData1[1] = 1.0/2.0;/* A=0, B=0, C=1 */
  valueData1[2] = 3.0/4.0;/* A=0, B=1, C=0 */
  valueData1[3] = 1.0/4.0;/* A=0, B=1, C=1 */
  valueData1[4] = 1.0/2.0;/* A=1, B=0, C=0 */
  valueData1[5] = 1.0/2.0;/* A=1, B=0, C=1 */
  valueData1[6] = 3.0/4.0;/* A=1, B=1, C=0 */
  valueData1[7] = 1.0/4.0;/* A=1, B=1, C=1 */
  Pot *p = Pot_CreateTabular("cptPot",vs1,valueData1);
  Variable **parents = jballoc(sizeof(Variable*)*2);
  parents[0]=A;
  parents[1]=B;
  CPT *cpt = CPT_Create(parents, 2, C, p);

}END_TEST

START_TEST (Test_BNet_Create){
  Variable *A,*B, *C, *D;
  List *l;
  A = Variable_CreateDiscrete("A",2,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  D = Variable_CreateDiscrete("D",2,NULL);
  l = List_Create();
  List_Append(l,D);
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);

  //Create a table for P(C|A,B)
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);
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
  parents1[0]=A;
  parents1[1]=B;
  CPT *cpt1 = CPT_Create(parents1, 2, C, p1);
  
  //Create a table for P(A|D)
  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,A);
  double *valueData2 = (double*)jballoc(sizeof(double) *4);
  valueData2[0] = 1.0/2.0;/* D=0, A=0,*/
  valueData2[1] = 1.0/2.0;/* D=0, A=1,*/
  valueData2[2] = 1.0/4.0;/* D=1, A=0,*/
  valueData2[3] = 3.0/4.0;/* D=1, A=1,*/
  Pot *p2 = Pot_CreateTabular("cptPot",vs2,valueData2);
  Variable **parents2 = jballoc(sizeof(Variable*));
  parents2[0]=D;
  CPT *cpt2 = CPT_Create(parents2, 1, A, p2);

  //Create a table for P(B|D)
  VariableSet *vs3 = VariableSet_Create(o);
  VariableSet_Add(vs3,B);
  double *valueData3 = (double*)jballoc(sizeof(double) *4);
  valueData3[0] = 1.0/4.0;/* D=0, B=0,*/
  valueData3[1] = 3.0/4.0;/* D=0, B=1,*/
  valueData3[2] = 2.0/4.0;/* D=1, B=0,*/
  valueData3[3] = 2.0/4.0;/* D=1, B=1,*/
  Pot *p3 = Pot_CreateTabular("cptPot",vs3,valueData3);
  Variable **parents3 = jballoc(sizeof(Variable*));
  parents3[0]=D;
  CPT *cpt3 = CPT_Create(parents3, 1, B, p3);

  //Create a table for P(D)
  VariableSet *vs4 = VariableSet_Create(o);
  VariableSet_Add(vs4,D);
  double *valueData4 = (double*)jballoc(sizeof(double) *2);
  valueData4[0] = 1.0/4.0;/* D=0*/
  valueData4[1] = 3.0/4.0;/* D=1*/
  Pot *p4 = Pot_CreateTabular("cptPot",vs4,valueData4);
  CPT *cpt4 = CPT_Create(NULL, 0, D, p4);  

  CPT **tables = (CPT**)jballoc(sizeof(CPT*)*4);
  tables[0] = cpt1;
  tables[1] = cpt2;
  tables[2] = cpt3;
  tables[3] = cpt4;

  BNet *bn = BNet_Create(o,tables, 4);
  char *outFileName = pathJoin(dirname,"BNTest1Out.dot");
  Graph_Write(bn->initialGraph,outFileName);
}END_TEST

Suite *jt_suite (void)
{
  Suite *s = suite_create ("Model");
  /* Core test case */
  TCase *ModelCore = tcase_create ("ModelCore");
  tcase_add_checked_fixture (ModelCore, setup, teardown);
  tcase_add_test (ModelCore,Test_CPT_Create);
  tcase_add_test (ModelCore,  Test_BNet_Create);
  tcase_set_timeout(ModelCore, 0);
  suite_add_tcase (s, ModelCore);
 
  return s;
}

int main (void){
  int number_failed;
  Suite *s = jt_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
