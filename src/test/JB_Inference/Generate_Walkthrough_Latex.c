#include"JB_Inference.h"
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

int main (void){
  BNet *bn = BuildSimpleBNet();
  VertexOrdering *elim_order = BNet_StateSpaceWeightedElimOrdering(bn);
  BNet_BuildElimTree(bn, elim_order);
  //initialize
  BNet_InitializePotentials(bn);

  printf("\n************************************\n");
  printf("Output from Test_BNet_CollectDistribute\n");
  printf("************************************\n");
  
  Print_BNet(bn);

  //collect
  BNet_CollectEvidence(bn);

  printf("\n************************************\n");
  printf("After Evidence Collection\n");
  printf("************************************\n");

  Print_BNet(bn);

  //distribute
  BNet_DistributeEvidence(bn);
  printf("\n************************************\n");
  printf("After Evidence Distribution\n");
  printf("************************************\n");

  Print_BNet(bn);

}
