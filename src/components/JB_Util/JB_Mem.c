#include "JB_Util.h"
/******************************************************************************
 ***** Memory Management
 ******************************************************************************/
/*Wrap malloc so as to avoid tedious null checks everywhere, and provide
  a central point of failure*/
void *jballoc(size_t amt){
  void *ret = malloc(amt);
  if(ret == NULL){
    reportError("Out of memory");
  }
  return ret;
}

/*Wrap free. Maybe add ref counting? */
void jbfree(void *p){
  free(p);
}

