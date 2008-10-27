/*JB_Mem.h
 * Version 0.01
 * Contains a very simple reference counter for pointers. 
 * Contains constant strings giving the types of pointers.
 * Contains a Labeled pointer type
 * working with them.
 */
#ifndef H_JB_MEM
#define H_JB_MEM
/******************************************************************************
 ***** MemoryManager
 * The MemoryManager maintains a list of pointers, along with reference counts,
 * and a mapping from types to destructors.
 ******************************************************************************/
//A struct for reference counting pointers
typedef struct pointer_list_node{
  void *contents;
  const char *type;
  pointer_list_node *next;
  int count;
} PointerListNode;

//A list of reference counted pointers
typedef struct pointer_list{
  int size;
  PointerListNode *first;
  PointerListNode *last;
} PointerList;

PointerList *PointerListCreate();
int PointerListDestroy();

//Increase the reference count for ptr
MemManRef(MemoryManager *manager,void *ptr);
MemManDeref(MemoryManager *manager,const char *typeName, void *ptr);


/******************************************************************************
 ***** LabeledVPtr
 ******************************************************************************/

typedef struct labeled_void_pointer{
  const char *type;
  void  *contents;
} LabeledVPtr;

//Allocate a new LabeledVPtr Object.
LabeledVPtr  *LabeledVPtrCreate(const char *type,
					  void *contents);
/*Deallocate a LabeledVPtrObject
  Returns 0 on success
 */
int LabeledVPtr *LabeledVPtrDestroy(LabeledVPtr *c);


#endif
