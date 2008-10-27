#include "JB_Util.h"
#include <string.h>
#include <check.h>

/******************************************************************************
 ******************************************************************************
 *** Globals
 ******************************************************************************
 *****************************************************************************/

//Helper Functions


int anti_strcmp(char *s1, char *s2){
  return -1 * strcmp(s1, s2);
}

/******************************************************************************
 ***** Linked
 ***** Lists
 ******************************************************************************/
/*Specialize List, to show that it can be specialized, 
 * then run tests on specialization */
typedef List StringList;

StringList *StringList_Create(){
  return (StringList*)List_Create();
}
void StringList_Destroy(StringList *sl){
  List_Destroy(sl);
}

void StringList_Append(StringList *sl, char *s){
  List_Append((List *)sl,(void *)s);
}
void StringList_Prepend(StringList *sl, char *s){
  List_Prepend((List *)sl,(void *)s);
}

StringList *list;
char *strings[4]={"A","B","C","D"};

//Prototypes
Suite *util_suite (void);


/******************************************************************************
 ***** Maps:
 ******************************************************************************/
/*Specialize Mapt, to show that it can be specialized, 
 * then run tests on specialization */
typedef Map StringMap;
typedef Map_Element StringMapItr;

void StringMap_Add(StringMap *this, char *key, char *value){
  Map_Element *me;
  me = malloc(sizeof(Map_Element));
  me->id = String_Create(key);
  me->contents = String_Create(value);
  Map_Add(this,me);
}

char *StringMap_Get(StringMap *this, char *key){
  Map_Element *me = Map_Get(this,key);
  return (char *)me->contents;
}

/*void StringMap_Delete(StringMap *this, char *key){
  Map_Element *me = Map_Get(this,key);
  Map_Delete(this, me);
  }*/

StringMapItr *StringMap_GetItrAt(StringMap *this, char *key){
  if(key == NULL){
    return Map_GetFirst_Element(this);
  }
  else{
    return Map_Get(this,key);
  }  
}

char *StringMap_ContentPrinter(Map_Element *me){
  char *ret = jballoc(strlen(me->id) +strlen( (char*)me->contents) + 4);
  sprintf(ret, "%s\n\t%s\n",me->id,(char *)me->contents);
  return ret;
}

char *StringMapItr_Current(StringMapItr *itr){
  return (char*)itr->contents;
}

StringMapItr *StringMapItr_Advance(StringMapItr *itr){
    return itr->next;
}

int Map_Element_KeyRevLexCmp(Map_Element *me1, Map_Element *me2){
  return anti_strcmp(me1->id,me2->id);
}



/******************************************************************************
 ******************************************************************************
 *** Tests 
 ******************************************************************************
 *****************************************************************************/
START_TEST (Test_StringList_Create) {
  list = StringList_Create();
  fail_unless(list != NULL, "List not initialized properly");
  fail_unless(list->first == NULL, "List not initialized properly");
  fail_unless(list->last == NULL, "List not initialized properly");
  fail_unless(list->size == 0, "List not initialized properly");
  StringList_Destroy(list);
}
END_TEST

START_TEST(Test_StringList_Append){
  list = StringList_Create();
  StringList_Append(list,strings[0]);
  fail_unless(list->size == 1, "List not appended to properly");
  fail_unless(strcmp((char*)list->first->contents,strings[0]) ==0,
	      "List not appended to properly");
  fail_unless(strcmp(list->first->contents,strings[0]) ==0, 
	      "List not appended to properly");
  StringList_Append(list,strings[1]);
  fail_unless(list->size == 2, "List not appended to properly");
  fail_unless(strcmp(list->last->contents,strings[1]) ==0,
	      "List not appended to properly");
  fail_unless(strcmp(list->first->contents,strings[0]) ==0, 
	      "List not appended to properly");
  StringList_Destroy(list);
}END_TEST

START_TEST(Test_StringList_Prepend){
  list = StringList_Create();
  StringList_Prepend(list,strings[0]);
  fail_unless(list->size == 1, "List not prepended to properly");
  fail_unless(strcmp((char*)list->first->contents,strings[0]) ==0,
	      "List not prepended to properly");
  fail_unless(strcmp(list->first->contents,strings[0]) ==0, 
	      "List not prepended to properly");
  StringList_Prepend(list,strings[1]);
  fail_unless(list->size == 2, "List not prepended to properly");
  fail_unless(strcmp(list->last->contents,strings[0]) ==0,
	      "List not prepended to properly");
  fail_unless(strcmp(list->first->contents,strings[1]) ==0, 
	      "List not prepended to properly");
  StringList_Destroy(list);
}END_TEST
  
START_TEST(Test_StringMap){
  StringMap *sm=Map_Create();
  StringMapItr *itr;
  int i;
  char *s1;
  for(i = 0; i < 4; i++){
    StringMap_Add(sm, strings[i], strings[3-i]);
  }
  itr=StringMap_GetItrAt(sm,NULL);
  for(i = 0; i < 4; i++){
    s1 = StringMap_Get(sm,strings[i]);
    char *msg = String_Create("Expected");
    strcatLK(msg,strings[3-i], " ");
    msg = strcatLK(msg,"Found", " ");
    msg = strcatLK(msg,s1, " ");
    msg = strcatLK(msg,"\n","");

    fail_unless(strcmp(s1,strings[3-i]) ==0, msg);
    itr = StringMapItr_Advance(itr);
  }
  /*  Map_SortBy(sm,Map_Element_KeyRevLexCmp);
    itr=StringMap_GetItrAt(sm,NULL);
  for(i = 0; i < 4; i++){
    s1 = StringMapItr_Current(itr);
    char *msg = String_Create("Expected");
    msg = strcatLK(msg,strings[3-i], " ");
    msg = strcatLK(msg,"Found", " ");
    msg = strcatLK(msg,s1, " ");
    msg = strcatLK(msg,"\n","");
    fail_unless(strcmp(s1,strings[i]) ==0, msg);
    itr = StringMapItr_Advance(itr);
    }*/
  s1 =Map_ToString(sm,StringMap_ContentPrinter);
  printf("\n%s\n",s1);

}END_TEST
/******************************************************************************
 ******************************************************************************
 *** Test Setup Code
 ******************************************************************************
 *****************************************************************************/
void setup (void) {}
void teardown (void) {}

int main (void)
{
  int number_failed;
  Suite *s = util_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Suite *util_suite(void){
  Suite *s = suite_create ("Util");
  TCase *tc_core = tcase_create("UtilCore");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, Test_StringList_Create);
  tcase_add_test (tc_core, Test_StringList_Append);
  tcase_add_test (tc_core, Test_StringList_Prepend);
  tcase_add_test (tc_core, Test_StringMap);
  suite_add_tcase (s, tc_core);
  return s;
}
