/*junctionbox_java.i*/
%module junctionbox
%include "carrays.i"
%array_functions(double, doubleArray);
%typemap(javabase) SWIGTYPE, SWIGTYPE *, SWIGTYPE &,
		    SWIGTYPE [], SWIGTYPE (CLASS::*) "SWIG"

%typemap(javacode) SWIGTYPE, SWIGTYPE *, SWIGTYPE &, SWIGTYPE [], 
                   SWIGTYPE (CLASS::*) %{
  protected long getPointer() {
    return swigCPtr;
  }
%}
%{
	#include"JB_Inference.h"
	int List_Length(List *l){
	    return l->size;
	}	
%}

int List_Length(List *l);

void *jballoc(size_t amt);

void jbfree(void *p);

List *List_Create();
void List_Destroy(List *this);
void List_Append(List *l, void *contents);

BNet *BuildSimpleBNet();
VertexOrdering *BNet_StateSpaceWeightedElimOrdering(BNet *this);
Variable *Variable_CreateDiscrete(char *label, int cardinality, double *valueData);
VariableOrdering* VariableOrdering_Create(List *l);
VariableSet *VariableSet_Create(VariableOrdering *o);
void VariableSet_Add(VariableSet *this, Variable *v);
Pot *Pot_CreateTabular(char *label,
			VariableSet *vbls,
			double *valueData);
//TODO set up typemap for Variable** properly instead of boxing in List
CPT *CPT_Create_ParentList(List *parents,
		Variable *tableFor, Pot *pot);
Observation *Observation_Create(VariableSet *v, int value);
void BNet_InitializePotentials(BNet *this);
void BNet_BuildElimTree(BNet *this, VertexOrdering *o);
void BNet_CollectEvidence(BNet *this);
void BNet_DistributeEvidence(BNet *this);
List *BNet_AllContClqPot(BNet *this, Variable *vbl);
double *Pot_SumMarginTabularSingleVbl(Pot *p, Variable *v);
MulInd *VariableSet_ComputeMargInd(VariableSet *this,
			           VariableSet *target);
void BNet_ComputeSumMarginals(BNet *this);
BNet *BNet_Create_List(VariableOrdering *vbls, 
		       List *tables);
BNet *BNet_Create_List2(VariableOrdering *vbls, 
		       List *tables,
		       List *obs);
void BNet_InitializePotentials(BNet *this);
void BNet_InitializePotentialsLog(BNet *this);
void BNet_CollectEvidenceLog(BNet *this);
void BNet_DistributeEvidenceLog(BNet *this);
BNet *BuildSimpleBNetDObsLog(int val);
void Print_BNetLog(BNet *bn);
char *Pot_ToStringLog(Pot *this);
Observation *Observation_CreateLog(VariableSet *v, int value);
Pot *Pot_CreateTabularUnityLog(char *label,
			VariableSet *vbls);
Pot *Pot_ProductTabularLog(Pot *p, Pot *q, MulInd *mi);
Pot *Pot_UpdateRatioTabularLog(Pot *newSep, Pot *oldSep);
Pot *Pot_SumMarginTabularLog(Pot *p, MulInd *m);
Pot *Pot_SumMarginTabularSingleVblLog(Pot *p, Variable *v);

void Print_BNet(BNet *bn);
