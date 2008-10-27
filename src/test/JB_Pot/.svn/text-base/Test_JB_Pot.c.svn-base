#include"check.h"
#include"JB_Util.h"
#include"JB_Pot.h"
#include<string.h>
#include<stdlib.h>
#include<check.h>
#include<limits.h>

//Globals
char *jbhome;
char *dirname;

//Prototypes
Suite *jt_suite (void);

//Helper Functions
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

/******************************************************************************
 ******************************************************************************
 *** Test Setup Code
 ******************************************************************************
 *****************************************************************************/

void setup_resource_paths(){
  char *resPath = "res/test/JB_Pot";
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

START_TEST (Test_VariableSet_ComputeIndex){
  char *msg;
  Variable *A,*B,*C;
  int a,b,c;
  List *l;
  A = Variable_CreateDiscrete("A",3,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  msg = strcatLK(VariableOrdering_ToString(o),"\n","");
  VariableSet *vs = VariableSet_Create(o);
  VariableSet_Add(vs,A);
  VariableSet_Add(vs,B);
  VariableSet_Add(vs,C);
  for(a = 0; a < 3; a++){
    for(b = 0; b < 2; b++){
      for(c = 0; c < 2; c++){
	int vals[3] = {a,b,c};
	int index = VariableSet_ComputeIndex(vs, vals);
	char buf[10];
	sprintf(buf, "%i", index);
	msg = strcatLK(msg,buf," ");
      }
      msg = strcatLK(msg,"\n","");
    }
    msg = strcatLK(msg,"\n\n","");
  }
  fail_unless(0,msg);
}END_TEST

START_TEST (Test_VariableSet_ComputeIndices){
  char *msg;
  Variable *A,*B,*C;
  int a,b,c, index = 0;
  int **valsTable = jballoc(12 * sizeof(int*));
  List *l;
  int *indices;
  A = Variable_CreateDiscrete("A",3,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  msg = strcatLK(VariableOrdering_ToString(o),"\n","");
  VariableSet *vs = VariableSet_Create(o);
  VariableSet_Add(vs,A);
  VariableSet_Add(vs,B);
  VariableSet_Add(vs,C);
  for(a = 0; a < 3; a++){
    for(b = 0; b < 2; b++){
      for(c = 0; c < 2; c++){
	int *vals = jballoc(3 * sizeof(int));
	vals[0] = a;
	vals[1] = b;
	vals[2] = c;
	valsTable[index++] = vals;
      }
    }
  }
  indices = VariableSet_ComputeIndices(vs,valsTable,12);
  for(index = 0; index < 12; index++){
    char buf[10];
    sprintf(buf, "%i", indices[index]);
    msg = strcatLK(msg,buf," ");
  }
  fail_unless(0,msg);
}END_TEST

START_TEST (Test_VariableSet_EnumerateValues){
  char *msg;
  Variable *A,*B,*C;
  int **indexTable;
  int sampleSpaceCard , i, j;
  List *l;
  A = Variable_CreateDiscrete("A",5,NULL);
  B = Variable_CreateDiscrete("B",3,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  VariableSet *vs = VariableSet_Create(o);
  VariableSet_Add(vs,A);
  VariableSet_Add(vs,B);
  VariableSet_Add(vs,C); 
  indexTable = VariableSet_EnumerateValues(vs, &sampleSpaceCard);
  msg = "";
  for(i =0; i < sampleSpaceCard; i++){
    for( j = 0; j < 3; j++){
    char buf[10];
    sprintf(buf, "%i", indexTable[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  fail_unless(0,msg);
}END_TEST

START_TEST (Test_IndexCross){
  char *msg;
  Variable *A,*B,*C;
  int bval = 2;
  int **indexTable1,**indexTable2;
  int sampleSpaceCard , i, j;
  List *l;
  A = Variable_CreateDiscrete("A",3,NULL);
  B = Variable_CreateDiscrete("B",3,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,C); 
  VariableSet_Add(vs2,B);

  indexTable1 = VariableSet_EnumerateValues(vs1, &sampleSpaceCard);
  indexTable2 = IndexCross(vs2,&bval, vs1, indexTable1,sampleSpaceCard);
  msg = "";
  for(i =0; i < sampleSpaceCard; i++){
    for( j = 0; j < 3; j++){
    char buf[10];
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  fail_unless(0,msg);
}END_TEST
 /*We check that a variable set produces the correct indices when
   asked to marginalize onto a single variable.*/

void setup_vbls_for_ComputeMargInd(  Variable **A,Variable **B,Variable **C,
				     Variable **D, 
				     VariableSet **vs1,   VariableSet **vs2){
  *D = Variable_CreateDiscrete("D",3,NULL);
  *A = Variable_CreateDiscrete("A",3,NULL);
  *B = Variable_CreateDiscrete("B",2,NULL);
  *C = Variable_CreateDiscrete("C",2,NULL);
  List *l = List_Create();
  List_Append(l,*D);
  List_Append(l,*A);
  List_Append(l,*B);
  List_Append(l,*C);
  VariableOrdering *o = VariableOrdering_Create(l);
  *vs1 = VariableSet_Create(o);
  *vs2 = VariableSet_Create(o);
}

START_TEST (Test_ComputeMargInd1){
  char *msg;
  Variable *A,*B,*C, *D;
  int **indexTable2;
  int i, j;
  VariableSet *vs1, *vs2;
  setup_vbls_for_ComputeMargInd(  &A, &B, &C,
				  &D, &vs1, &vs2);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);

  VariableSet_Add(vs2,B);
  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  msg = "\n";
  msg =  strcatLK(msg,"\n************************************\n","");
  msg =  strcatLK(msg,"Output from TestComputeMargInd1\n","");
  msg =  strcatLK(msg,"We marginalize onto the second index in a 3x2x2 space"
		  ,"");
  msg =  strcatLK(msg,"************************************\n","");
  indexTable2 = mi->secondIndices;
  char buf[10];
  for(i =0; i < mi->firstIndices; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < mi->mulSize; j++){
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  printf(msg);
}END_TEST
 /*We check that a variable set produces the correct indices when
   asked to marginalize onto the first two variables.*/
START_TEST (Test_ComputeMargInd2){
  char *msg;
  Variable *A,*B,*C, *D;
  int **indexTable2;
  int i, j;
  VariableSet *vs1, *vs2;
  setup_vbls_for_ComputeMargInd(  &A, &B, &C,
				  &D, &vs1, &vs2);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);

  VariableSet_Add(vs2,A);  
  VariableSet_Add(vs2,B);  
  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  msg = "\n";
  msg =  strcatLK(msg,"\n************************************\n","");
  msg =  strcatLK(msg,"Output from TestComputeMargInd1\n","");
  msg =  strcatLK(msg,
		  "We marginalize onto the first two indices in a 3x2x2 space"
		  ,"");
  msg =  strcatLK(msg,"************************************\n","");
  indexTable2 = mi->secondIndices;
  char buf[10];
  for(i =0; i < mi->firstIndices; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < mi->mulSize; j++){
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  printf(msg);
}END_TEST

 /*We check that a variable set produces the correct indices when
   asked to marginalize onto the first two variables.*/
START_TEST (Test_ComputeMargInd3){
  char *msg;
  Variable *A,*B,*C, *D;
  int **indexTable2;
  int i, j;
  VariableSet *vs1, *vs2;
  setup_vbls_for_ComputeMargInd(  &A, &B, &C,
				  &D, &vs1, &vs2);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);

  VariableSet_Add(vs2,B);  
  VariableSet_Add(vs2,C);
  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  msg = "\n";
  msg =  strcatLK(msg,"\n************************************\n","");
  msg =  strcatLK(msg,"Output from TestComputeMargInd3\n","");
  msg =  strcatLK(msg,
		  "We marginalize onto the last two indices in a 3x2x2 space"
		  ,"");
  msg =  strcatLK(msg,"************************************\n","");
  indexTable2 = mi->secondIndices;
  char buf[10];
  for(i =0; i < mi->firstIndices; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < mi->mulSize; j++){
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  printf(msg);
}END_TEST

 /*We check that a variable set produces the correct indices when
   asked to marginalize onto the first and last variables.*/
START_TEST (Test_ComputeMargInd4){
  char *msg;
  Variable *A,*B,*C, *D;
  int **indexTable2;
  int i, j;
  VariableSet *vs1, *vs2;
  setup_vbls_for_ComputeMargInd(  &A, &B, &C,
				  &D, &vs1, &vs2);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);

  VariableSet_Add(vs2,A);  
  VariableSet_Add(vs2,C);

  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  msg = "\n";
  msg =  strcatLK(msg,"\n************************************\n","");
  msg =  strcatLK(msg,"Output from TestComputeMargInd4\n","");
  msg =  strcatLK(msg,
		  "We marginalize onto the first and last indices in a 3x2x2 space"
		  ,"");
  msg =  strcatLK(msg,"************************************\n","");
  indexTable2 = mi->secondIndices;
  char buf[10];
  for(i =0; i < mi->firstIndices; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < mi->mulSize; j++){
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  printf(msg);
}END_TEST

START_TEST (Test_ComputeMulInd){
  char *msg;
  Variable *A,*B,*C;
  int **indexTable2;
  int i, j;
  List *l;
  A = Variable_CreateDiscrete("A",3,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  C = Variable_CreateDiscrete("C",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  List_Append(l,C);
  VariableOrdering *o = VariableOrdering_Create(l);
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C); 
  VariableSet_Add(vs2,B);
  MulInd *mi = VariableSet_ComputeMulInd(vs2,vs1);
  msg = "\n";
  indexTable2 = mi->secondIndices;
  char buf[10];
  for(i =0; i < mi->firstIndices; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < mi->mulSize; j++){
    sprintf(buf, "%i", indexTable2[i][j]);
    msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  fail_unless(0,msg);
}END_TEST

START_TEST (Test_Pot_CreateTabular){
  char *msg;
  Variable *A,*B;
  int i, j;
  List *l;
  A = Variable_CreateDiscrete("A",2,NULL);
  B = Variable_CreateDiscrete("B",2,NULL);
  l = List_Create();
  List_Append(l,A);
  List_Append(l,B);
  VariableOrdering *o = VariableOrdering_Create(l);
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  double *valueData = (double*)jballoc(sizeof(double) *4);
  valueData[0] = 1.0/2.0;
  valueData[1] = 1.0/2.0;
  valueData[2] = 3.0/4.0;
  valueData[3] = 1.0/4.0;
  Pot *p = Pot_CreateTabular("testPot",vs1,valueData);
  msg = "\n";
  char buf[10];
  int *values = (int*)jballoc(sizeof(int)*2);
  for(i =0; i < 2; i++){
    sprintf(buf, "%i :\t", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < 2; j++){
      values[0]=i;
      values[1]=j;
      sprintf(buf, "%f",Pot_EvaluateTabular(p, values));
      msg = strcatLK(msg,buf," ");
    }
    msg = strcatLK(msg,"\n"," ");
  }
  
  fail_unless(0,msg);
}END_TEST

START_TEST (Test_Pot_ProductTabular){
  char *msg;
  Variable *A,*B, *C;
  int i, j, k;
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

  Pot *p = Pot_CreateTabular("testPot",vs1,valueData1);

  //Create a table for P(A)
  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,A);
  VariableSet_Add(vs2,B);
  double *valueData2 = (double*)jballoc(sizeof(double) *4);
  valueData2[0] = 1.0/2.0;/* A=0, B=0,*/
  valueData2[1] = 1.0/4.0;/* A=0, B=1,*/
  valueData2[2] = 3.0/16.0;/* A=1, B=0,*/
  valueData2[3] = 1.0/16.0;/* A=1, B=1,*/
  Pot *q = Pot_CreateTabular("testPot",vs2,valueData2);
 
  MulInd *mi = VariableSet_ComputeMulInd(vs2,vs1);
  printf("\n************************************\n");
  printf("Output from Test_PotProductTabular\n");
  printf("************************************\n");
  printf("Multiplying \n %s\n",Pot_ToString(q));
  printf(" by \n %s\n",Pot_ToString(p));
  printf(" with MulInd \n %s \n", MulInd_ToString(mi));
  p = Pot_ProductTabular(q,p,mi);
  msg = "\n produces \n";
    char buf[10];
  for(i =0; i < 2; i++){
    sprintf(buf, "i =%i", i);
    msg = strcatLK(msg,buf," ");
    for( j = 0; j < 2; j++){
      sprintf(buf, ",j =%i", j);
      msg = strcatLK(msg,buf," ");
      for( k = 0; k < 2; k++){
	sprintf(buf, ",k =%i:\t", k);
	msg = strcatLK(msg,buf," ");
	int *values = (int*)jballoc(sizeof(int)*3);
	values[0]=i;
	values[1]=j;
	values[2]=k;
	sprintf(buf, "%f",Pot_EvaluateTabular(p, values));
	msg = strcatLK(msg,buf," ");
	msg = strcatLK(msg,"\n","");
      }
    }
  }
  printf(msg);
  // fail_unless(0,"Test Message");
}END_TEST
START_TEST (Test_Pot_SumMarginTabular){
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

  //Create a table for P(A,B,C)
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);
  double *valueData1 = (double*)jballoc(sizeof(double) *8);
  valueData1[0] = 2.0/16.0;/* A=0, B=0, C=0 */
  valueData1[1] = 1.0/16.0;/* A=0, B=0, C=1 */

  valueData1[2] = 3.0/16.0;/* A=0, B=1, C=0 */
  valueData1[3] = 2.0/16.0;/* A=0, B=1, C=1 */

  valueData1[4] = 2.0/16.0;/* A=1, B=0, C=0 */
  valueData1[5] = 2.0/16.0;/* A=1, B=0, C=1 */

  valueData1[6] = 3.0/16.0;/* A=1, B=1, C=0 */
  valueData1[7] = 1.0/16.0;/* A=1, B=1, C=1 */
  Pot *p = Pot_CreateTabular("testPot",vs1,valueData1);

  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,A);
  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  printf("\n************************************\n");
  printf("Output from Test_Pot_SumMarginTabular\n");
  printf("************************************\n");

  printf("Given pot \n %s\n",Pot_ToString(p));
  printf("\nMarginalizing along A %s\n", MulInd_ToString(mi));
  Pot *q = Pot_SumMarginTabular(p,mi);
  printf("produces \n %s\n",Pot_ToString(q));
  
  vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,B);
  mi = VariableSet_ComputeMargInd(vs1,vs2);
  printf("\nMarginalizing along B %s\n", MulInd_ToString(mi));
  q = Pot_SumMarginTabular(p,mi);
  printf("produces \n %s\n",Pot_ToString(q));
  
  vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,C);
  mi = VariableSet_ComputeMargInd(vs1,vs2);
  printf("\n Marginalizing along C %s\n", MulInd_ToString(mi));
  q = Pot_SumMarginTabular(p,mi);
  printf("produces \n %s\n",Pot_ToString(q));
}END_TEST

START_TEST (Test_Pot_UpdateRatioTabular){
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

  //Create a table for P(A,B,C)
  VariableSet *vs1 = VariableSet_Create(o);
  VariableSet_Add(vs1,A);
  VariableSet_Add(vs1,B);
  VariableSet_Add(vs1,C);
  double *valueData1 = (double*)jballoc(sizeof(double) *8);
  valueData1[0] = 2.0/16.0;/* A=0, B=0, C=0 */
  valueData1[1] = 1.0/16.0;/* A=0, B=0, C=1 */

  valueData1[2] = 3.0/16.0;/* A=0, B=1, C=0 */
  valueData1[3] = 2.0/16.0;/* A=0, B=1, C=1 */

  valueData1[4] = 2.0/16.0;/* A=1, B=0, C=0 */
  valueData1[5] = 2.0/16.0;/* A=1, B=0, C=1 */

  valueData1[6] = 3.0/16.0;/* A=1, B=1, C=0 */
  valueData1[7] = 1.0/16.0;/* A=1, B=1, C=1 */
 
  double *valueData2 = (double*)jballoc(sizeof(double) *2);
  valueData2[0] = 0.5;
  valueData2[1] = 0.5;
  
  VariableSet *vs2 = VariableSet_Create(o);
  VariableSet_Add(vs2,A);
  
  MulInd *mi = VariableSet_ComputeMargInd(vs1,vs2);
  Pot *p = Pot_CreateTabular("testPot",vs1,valueData1);
  Pot *pMargA = Pot_SumMarginTabular(p,mi);
  Pot *s = Pot_CreateTabular("oldSep(A)",vs2,valueData2);
  printf("%s\n",Pot_ToString(p));
  printf("%s\n",Pot_ToString(pMargA));
  printf("%s\n",Pot_ToString(s));
  printf("%s\n",Pot_ToString(Pot_UpdateRatioTabular(pMargA,s))) ;  
}END_TEST
Suite *jt_suite (void)
{
  Suite *s = suite_create ("Jt");
  /* Core test case */
  TCase *JTCore = tcase_create ("JTCore");
  tcase_add_checked_fixture (JTCore, setup, teardown);
  tcase_add_test (JTCore, Test_VariableSet_ComputeIndex);
  tcase_add_test (JTCore, Test_VariableSet_ComputeIndices); 
  tcase_add_test (JTCore, Test_VariableSet_EnumerateValues); 
  tcase_add_test (JTCore, Test_IndexCross);
  tcase_add_test (JTCore, Test_ComputeMargInd1);
  tcase_add_test (JTCore, Test_ComputeMargInd2);
  tcase_add_test (JTCore, Test_ComputeMargInd3);
  tcase_add_test (JTCore, Test_ComputeMargInd4);
  tcase_add_test (JTCore, Test_ComputeMulInd);
  tcase_add_test (JTCore, Test_Pot_CreateTabular);
  tcase_add_test (JTCore, Test_Pot_ProductTabular);
  tcase_add_test (JTCore, Test_Pot_SumMarginTabular);
  tcase_add_test (JTCore, Test_Pot_UpdateRatioTabular);
  
  tcase_set_timeout(JTCore, 0);
  suite_add_tcase (s, JTCore);
  return s;
}
