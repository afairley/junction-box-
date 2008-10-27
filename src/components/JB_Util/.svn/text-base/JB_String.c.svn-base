#include "JB_Util.h"
#define PATH_SEP "/"
char *String_Create(char *string){
  int length = strlen(string);
  char *ret = jballoc(length+1);
  strncpy(ret, string, length+1);
  return ret;
}

/*A string cat which is a memory leak, but less painful*/
char *strcatLK(char *s1, char *s2, char *sep){
  char* ret =(char*)jballoc(strlen(s1)+strlen(sep)+strlen(s2) +1);
  ret[0] = 0;
  strcat(ret,s1);
  strcat(ret,sep);
  strcat(ret,s2);
  return ret;
}

char *pathJoin(char *path1, char *path2){
  return strcatLK(path1,path2, PATH_SEP);
}
