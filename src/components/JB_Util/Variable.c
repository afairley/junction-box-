#include<string.h>
#include<JB_Util.h>
//A default valid value function for discrete variables
int Variable_DiscreteValidValue(Variable *this, double value){
  //TODO Implement this stub related to discrete vs continuous vbls
  return 1;
}
//A default enum value function for discrete variables
double *Variable_DiscreteEnumValue(Variable *this){
  //TODO Implement this stub related to discrete vs continuous vbls
  return NULL;
}

/*Allocates a Variable struct. Pointer Members of the struct will point at
  the passed in arguments.*/
Variable *Variable_Create(char *label, 
			  int cardinality,
			  int (*valid_value_func)(Variable *, double) , 
			  double *(*enumerate_value_func)(Variable *),
			  void *valueData
			  ) {
  Variable *ret = jballoc(sizeof(Variable));
  ret->label = (char*)jballoc(sizeof(char)*strlen(label));
  strcpy(ret->label,label);
  ret->cardinality = cardinality;
  ret->valid_value_func = valid_value_func;
  ret->enumerate_value_func = enumerate_value_func;
  ret->valueData = valueData;
  //printf("%s\n",ret->label); //REMOVE
  return ret;
}
Variable *Variable_CreateDiscrete(char *label,
				  int cardinality,
				  double *valueData){
  //printf("%s\n",label); //REMOVE
  return Variable_Create(label,
			 cardinality,
			 Variable_DiscreteValidValue,
			 Variable_DiscreteEnumValue,
			 valueData);
}
char *Variable_GetLabel(Variable *this){
  return this->label;
}
int Variable_GetCardinality(Variable *this){
  return this->cardinality;
}
/* Deallocates the memory allocated for the Variable struct.*/
void Variable_Destroy(Variable *v){
  //TODO Implement memory management
}
