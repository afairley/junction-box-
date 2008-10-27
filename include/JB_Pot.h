/* JB_Pot.h
 * Contains structs and typedefs for working with potentials and charges,
 * as well as function declarations
 * Version 0.01: Potentials will be represented as tree structures,
 * whose internal  nodes are operators on potentials, and whose leaves
 * are either Probability tables or algebraic representations of
 * probability distributions. */
#ifndef H_JB_POT
#define H_JB_POT
//TODO Refactor so that a dependency is not induced on JB_JunctionTree just for ElimSets
#include "JB_JunctionTree.h"
#include <math.h>
/***********************************************************************
 *** Variable
 ***********************************************************************/

typedef struct variable_set VariableSet;
typedef struct marg_memo MargMemo;
typedef struct mul_memo MulMemo;
typedef enum pot_type PotType;
typedef struct pot Pot;
typedef struct mul_ind MulInd;

/*typedef union val_data_t ValDataT;
  ValDataT;*/

struct variable_set{
  BitVector *bv;
  VariableOrdering *o;
};

struct marg_memo{
  BitVectorMap *bvm;
};

struct mul_memo{
  BitVectorMap *bvm;
};

// We memoize the indices that potentials share for
// summation/multiplication/division
struct mul_ind{
  VariableSet *vbls; //The variables in the Clique
  /**We multiply/add/divide sep[i < firstIndices] by 
   * {foreach j < mulSize[i] secondIndices[i][j]*/
  int firstIndices;
  // The <size> cardinalities of each marginalized variables' domain
  int mulSize;
  int **secondIndices;
};

//Types of potentials
enum pot_type { 
  TABULAR, 
  GAUSSIAN,
  COND_GAUSSIAN, 
  INFER
  //Add new leaf potential types here
};

struct pot{
  char *label;
  VariableSet *vbls;
  PotType type;
  /**valueData points at something whose nature depends
     on type. If we have a tabular potential than we will
     store the potential values in double * whose size is
     the cardinality of the product space of the
     variables domain.  The data is stored as the
     following example makes clear:
     Assume that phi is a tabular potential over A,B,C
     Where |dom(A)| = 3, |dom(B)|=2 , |dom(C)| = 2
     We just count in Z_{3 X 2 X 2}, "with a carry".  
     This lets us compute the index of a simple assignment
     by the forumula 
     a*(|dom(B)|*|dom(C)| +1) + b*(|dom(C)| + 1) + c.
     e.g., the value of phi(1,1,0) will be at 
     valueData[7]
     
     beating a dead horse,
     valueData will point at a double array whose
     contents are 
     {phi(A=0, B=0, C=0), phi(A=0, B=0, C=1), phi(A=0, B=1, C=0),
     phi(A=0, B=1, C=1), phi(A=1, B=0, C=0), phi(A=1, B=0, C=1),
     phi(A=1, B=1, C=0), phi(A=1, B=1, C=1), phi(A=2, B=0, C=0),
     phi(A=2, B=0, C=1), phi(A=2, B=1, C=0), phi(A=2, B=1, C=1)}
  */
  union val_data_t {
    double *tabData;
    void *algebraicData;
  }                              valueData;
  int tableSize;
  void *owner;
};

//A default valid value function for discrete variables
int Variable_DiscreteValidValue(Variable *this, double value);
//A default enum value function for discrete variables
double *Variable_DiscreteEnumValue(Variable *this);

VariableSet *VariableSet_Create(VariableOrdering *o);

VariableSet *VariableSet_CreateFromBv(VariableOrdering *o,BitVector *bv);
void VariableSet_Add(VariableSet *this, Variable *v);
void VariableSet_Destroy(VariableSet *this);

char *VariableSet_GetLabel(VariableSet *this);
//Return the number of vbls actually present in this
int VariableSet_GetNumVbls(VariableSet *this);
//Return the number of vbls in the ordering backing this
int VariableSet_GetSize(VariableSet *this);
Variable *VariableSet_GetFstVbl(VariableSet *this);
int *VariableSet_GetVblCardinalities(VariableSet *this);
//TODO unify nomenclature. ToString for all data structure
char *VariableSet_ToString(VariableSet *this);

//Add a vertex to an elimination set
//Should have no effect if the vertex is already in the set
void VariableSet_Add(VariableSet *this, Variable *v);
List *VariableSet_GetVariableList(VariableSet *this);
//Enumerates all of the values assignable to the variables in this set,
//as well as the cardinality of the space
int **VariableSet_EnumerateValues(VariableSet *this,int *cardinalityReturn);
int *VariableSet_ComputeIndices(VariableSet *vbls,
				int **values, 
				int numIndices);
int VariableSet_ContainedIn(VariableSet *this, VariableSet *other);
int VariableSet_ComputeIndex(VariableSet *vbls, int *values);
/*Take the cross product of the singleton firstIndices with the set
  secondIndicesSet. First and Second must be disjoint.*/
int** IndexCross(VariableSet *first,
		 int * firstIndices,
		 VariableSet *second,
		 int **secondIndicesSet, 
		 int secondSampleCard);

MulMemo *MulMemo_Create();
void MulMemo_Destroy(MulMemo *mm);
MulInd *MulMemo_GetMulInd(MulMemo *this, VariableSet *vbls,
			  VariableSet *target);

MulInd *MulMemo_GetMargInd(MargMemo *this, VariableSet *vbls,
			    VariableSet *target);
MulInd *VariableSet_ComputeMargInd(VariableSet *this,
				    VariableSet *target);
MulInd *VariableSet_ComputeMulInd(VariableSet *this/*a sepset*/,
				  VariableSet *target);

/***********************************************************************
 *** Pot : Potentials implementation
 ***********************************************************************/
int Pot_ComputeIndex(VariableSet *vbls, int *values);

Pot *Pot_Create(char *label,
		VariableSet *vbls,
                // if NULL, inferred from vbls.
		PotType type,
		void *valueData); 
Pot *Pot_CreateTabular(char *label,
			VariableSet *vbls,
			double *valueData);
Pot *Pot_CreateTabularUnity(char *label,
			VariableSet *vbls);
Pot *Pot_CreateTabularUnityLog(char *label,
			VariableSet *vbls);
Pot *Pot_CreateTabularUnInit(char *label,
			     VariableSet *vbls);
void Pot_DestroyTabular(Pot *p);

/*Evaluates a Potential at the specified values
 They should of course be ordered consistently with the variable
 ordering underlying the VariableSet in the pot*/
double Pot_EvaluateTabular(Pot *this, int *values);

/*Returns a pointer to a potential representing the sum of the passed
 in potentials. The second potential is mutated, and it is a pointer
 to it which is returned
 <this is unneeded for our current work>
Pot *Pot_Sum(Pot *p, Pot *q, IndexMap *im);
*/

/*Returns a pointer to a potential representing the product of the passed
 in potentials The second potential, <q> is mutated, and it is a pointer
 to it which is returned.  Indices not mentioned in <mi> are left unaltered.*/
Pot *Pot_ProductTabular(Pot *p, Pot *q, MulInd *mi);
Pot *Pot_ProductTabularLog(Pot *p, Pot *q, MulInd *mi);
Pot *Pot_ProductTabularObsLog(Pot *p, Pot *q, MulInd *mi);

/*Returns a pointer to a potential representing the quotient of the passed
 in potentials
 <this is unneeded for our current work>
Pot *Pot_Quotient(Pot *dividend, Pot *divisor, IndexMap *mi);
 */

/*Quotient optimized for when a potential is being divided by a new
 version of itself. Returns a freshly allocated potential which
 should be freed when done with*/
Pot *Pot_UpdateRatioTabular(Pot *newSep, Pot *oldSep);
Pot *Pot_UpdateRatioTabularLog(Pot *newSep, Pot *oldSep);

/*Returns a freshly allocated potential representing the sum-margin
 over the variables  specified in MulInd.*/
Pot *Pot_SumMarginTabular(Pot *p, MulInd *m);
Pot *Pot_SumMarginTabularLog(Pot *p, MulInd *m);
double *Pot_SumMarginTabularSingleVbl(Pot *p, Variable *v);
double *Pot_SumMarginTabularSingleVblLog(Pot *p, Variable *v);


/*Looks up the indices for either multiplying into the Pot represented
  by vs, or Marginalizing onto the Variables represented by vs*/
MulInd *Pot_LookupIndices(Pot * this, VariableSet *vs);

#endif
