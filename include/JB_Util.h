/*JB_Util.h
 * Contains general purpose data structures and function headers for
 * working with them. Contains code in addition to declarations.
 */
#ifndef H_JB_UTIL
#define H_JB_UTIL
#include "BitVector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define TRUE 1
#define FALSE 0

typedef char bool;

/******************************************************************************
 ***** Error Handling
 ******************************************************************************/

#define ERRSTRING(FILENAME, FUNCTION, TYPE) "An error of type < TYPE > occured in file < FILENAME >, function < FUNCTION >. \n"
void reportError(char *errstring);

/******************************************************************************
 ***** Memory Management
 ******************************************************************************/
/*Wrap malloc so as to avoid tedious null checks everywhere, and provide
  a central point of failure*/
void *jballoc(size_t amt);

/*Wrap free. Maybe add ref counting? */
void jbfree(void *p);

/******************************************************************************
 ***** String Helpers
 ******************************************************************************/
char *String_Create(char *string);
/*A string cat which is a memory leak, but less painful*/
char *strcatLK(char *s1, char *s2, char *sep);
char *pathJoin(char *path1, char *path2);

/******************************************************************************
 ***** Linked
 ***** Lists
 ******************************************************************************/

typedef struct list_node ListNode;
typedef struct list List;
//TODO move struct declarations into LinkedList.c 
struct list_node{
  void *contents;
  ListNode *next;
};
 struct list{
  //Size is the number of nodes in the list;
  int size;
  ListNode *first;
  ListNode *last;
 };
ListNode *ListNode_Create(void *contents);
void ListNode_Destroy(ListNode *ln);
List *List_Create();
void List_Destroy(List *l);
/*Inserts c at the end of l.*/
void List_Append(List *l, void *c);
/*Inserts c at the beginning of l.*/
void List_Prepend(List *l, void *c);
char *List_ToString(List *this, char *(cntntPrinter)(ListNode *content));

/******************************************************************************
 ***** Maps:
 ***** Provides a  mapping from strings to pointers,
 ******************************************************************************/
typedef struct map Map;
//TODO Change Map_Element to MapElement to coincide w/ naming conventions
typedef struct map_element Map_Element;
//TODO move struct declarations into Map.c 
struct map {
  Map_Element *first;
  int size;
};

struct map_element {
  char *id;                    /* key */
  void *contents;
  Map_Element *next;
};
Map_Element *Map_Element_Create(char *id, void *contents);
Map *Map_Create();
Map_Element *Map_Get(Map *this, char *id );
void Map_Add(Map *this, Map_Element *me);
int Map_Size( Map *this);
char *Map_ToString(Map *this, char *(cntntPrinter)(Map_Element *content));
Map_Element *Map_GetFirst_Element(Map *this);
Map_Element *Map_Element_GetNext_Element(Map_Element *me);

/*void Map_Delete(Map *this , Map_Element *me){
  HASH_DEL( this->hash, me);
}

void Map_SortBy(Map *this,int (*cmp)()){
  HASH_SORT( this->hash, cmp);
  } */



/******************************************************************************
 ***** IntMaps:
 ***** A simple mapping from ints to ints
 ******************************************************************************/
typedef struct int_map IntMap;
//TODO Change IntMap_Element to IntMapElement to coincide w/ naming conventions
typedef struct int_map_element IntMap_Element;

struct int_map {
  IntMap_Element *first;
  int size;
};

struct int_map_element {
  int id;                    /* key */
  int contents;
  IntMap_Element *next;
};

IntMap_Element *IntMap_Element_Create(int id, int contents);

IntMap *IntMap_Create();
IntMap_Element *IntMap_Get(IntMap *this, int id );
void IntMap_Add(IntMap *this, int id, int contents);

int IntMap_Size( IntMap *this);

char *IntMap_ToString(IntMap *this, char *(cntntPrinter)(IntMap_Element *content));

IntMap_Element *IntMap_GetFirst_Element(IntMap *this);

IntMap_Element *IntMap_Element_GetNext_Element(IntMap_Element *me);

/******************************************************************************
 ***** VoidpMaps:
 ***** A simple mapping from void pointers to void pointers
 ******************************************************************************/
typedef struct voidp_map VoidpMap;
//TODO Change VoidpMap_Element to VoidpMapElement to coincide w/ naming conventions
typedef struct voidp_map_element VoidpMap_Element;

struct voidp_map {
  VoidpMap_Element *first;
  int size;
};

struct voidp_map_element {
  void *id;                    /* key */
  void *contents;
  VoidpMap_Element *next;
};

VoidpMap_Element *VoidpMap_Element_Create(void *id, void *contents);

VoidpMap *VoidpMap_Create();
VoidpMap_Element *VoidpMap_Get(VoidpMap *this, void *id );
void VoidpMap_Add(VoidpMap *this, void *id, void *contents);

int VoidpMap_Size( VoidpMap *this);

char *VoidpMap_ToString(VoidpMap *this, char *(cntntPrinter)(VoidpMap_Element *content));

VoidpMap_Element *VoidpMap_GetFirst_Element(VoidpMap *this);

VoidpMap_Element *VoidpMap_Element_GetNext_Element(VoidpMap_Element *me);

/******************************************************************************
 ***** BitVectorMaps:
 ***** A simple mapping from BitVectors to void pointers
 ******************************************************************************/
typedef struct BitVector_map BitVectorMap;
//TODO Change BitVectorMap_Element to BitVectorMapElement to coincide w/ naming conventions
typedef struct BitVector_map_element BitVectorMap_Element;

struct BitVector_map {
  BitVectorMap_Element *first;
  int size;
};

struct BitVector_map_element {
  BitVector *id;                    /* key */
  void *contents;
  BitVectorMap_Element *next;
};

BitVectorMap_Element *BitVectorMap_Element_Create(void *id, void *contents);

BitVectorMap *BitVectorMap_Create();
BitVectorMap_Element *BitVectorMap_Get(BitVectorMap *this, BitVector *id );
void BitVectorMap_Add(BitVectorMap *this, BitVector *id, void *contents);

int BitVectorMap_Size( BitVectorMap *this);

char *BitVectorMap_ToString(BitVectorMap *this, char *(cntntPrinter)(BitVectorMap_Element *content));

BitVectorMap_Element *BitVectorMap_GetFirst_Element(BitVectorMap *this);

BitVectorMap_Element *BitVectorMap_Element_GetNext_Element(BitVectorMap_Element *me);


/******************************************************************************
 ***** Disjoint Set Forests
 ***** Mostly exactly as outlined in CLR, 
 ***** with the the minor modification that the "find_set_ext" exposed to
 ***** the outside  world takes in a void* that points to whatever
 ***** object it is that you want to have a DSF over, and looks up the
 ***** DSF node associated with that.
 ******************************************************************************/

struct disjoint_set_node{
  struct disjoint_set_node *parent;
  int rank;
  void *contents;
};
struct disjoint_set_forest{
  VoidpMap *lookup;
};
typedef struct disjoint_set_forest DSetForest;
typedef struct disjoint_set_node DSetNode;

DSetForest *DSetForest_Create();
DSetNode *DSetForest_MakeSet(DSetForest *this, void *element);
void DSetForest_Union(DSetForest *this, void *element1, void *element2);
DSetNode *DSetForest_GetRepresentative(DSetForest *this, void *element);

/******************************************************************************
 *****Variables and VariableOrderings
 ******************************************************************************/
typedef struct variable Variable;
typedef struct variable_ordering VariableOrdering;
struct variable{
  char *label;
  int cardinality;//0 for continuous variables
  int (*valid_value_func)(Variable* , double);
  double *(*enumerate_value_func)(Variable *);
  void *valueData;
};

struct variable_ordering{
  int size;
  Variable **vbls;
};

/*Allocates a Variable struct. Pointer Members of the struct will point at
  the passed in arguments.*/
Variable *Variable_Create(char *label,
			  int cardinality,
			  int (*valid_value_func)(Variable *, double) , 
			  double *(*enumerate_value_func)(Variable *),
			  void *valueData
			  ) ;

Variable *Variable_CreateDiscrete(char *label, int cardinality, double *valueData);
char *Variable_GetLabel(Variable *this);
int Variable_GetCardinality(Variable *this);
/* Deallocates the memory allocated for the Variable struct.*/
void Variable_Destroy(Variable *v);

/* Pass in a list ordering vertices from lowest to highest, 
 * get back an array backed  collection with the same ordering*/
VariableOrdering *VariableOrdering_Create(List *vIn);
char *VariableOrdering_ToString(VariableOrdering *this);

//Very slowly returns the rank of a variable in an ordering.
int VariableOrdering_GetVariableRank(VariableOrdering *this, Variable *v);
int VariableOrdering_GetVariableRankByLabel(VariableOrdering *this,
					    char *label);

#endif
