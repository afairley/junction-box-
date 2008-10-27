#include "BitVector.h"
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>



/***********************************************************************
 **** INFRASTRUCTURE
 ***********************************************************************/
struct bit_vector_t{
  char *bits;
  int size;
  int numBytes;
  void * (* allocator_func)(size_t);
  void (* deallocator_func)(void *);
};


//Private function protos
static BitVector *get_bit_vector_ret(BitVector *this,
				      BitVector_Flags flags);
static char check_vector(BitVector *bv, char value);
/***********************************************************************
 **** IMPLEMENTATION
 ***********************************************************************/
  
 BitVector *BitVector_Create(int size,                      
		  void * (* allocator_func)(size_t),
		  void (*free_func)(void *),
		  BitVector_Flags flags){
  BitVector *ret = NULL;
  int init_val;
  //initialize bits to either 0 or 1
  if(flags & BV_INIT_FILL){
    init_val = SCHAR_MAX;
  }else{
    init_val = 0;
  }
  if (allocator_func) {
    ret= allocator_func(sizeof(BitVector));
    assert(ret!= NULL);
  }
  else {
    ret= malloc(sizeof(BitVector));
    assert(ret!= NULL);
  }
  memset(ret, 0, sizeof(BitVector));
  ret->size = size;
  ret->numBytes = ceil(size/ (double)CHAR_BIT );
  ret->allocator_func = (allocator_func ? allocator_func : malloc);
  ret->deallocator_func = (free_func ? free_func : free);
  ret->bits = (char*) ret->allocator_func(ret->numBytes);
  //initialize bits to either 0 or 1
  memset(ret->bits,
	 init_val,
	 ret->numBytes);
  return ret;
}

 void BitVector_Destroy(BitVector *this){
  assert(this != NULL);
  void (*free_func)(void *) = this->deallocator_func;
  free_func(this->bits);
  free_func(this);
}

 BitVector *BitVector_And(BitVector *this, BitVector *other,
		      BitVector_Flags flags){
  int i;
  BitVector*ret = get_bit_vector_ret(this,flags);
  assert(this->size == other->size);
  for( i = 0; i < this->numBytes; i++)
    ret->bits[i] = this->bits[i] & other->bits[i];
  return ret;
}

 BitVector *BitVector_Or(BitVector *this, BitVector *other,
		   BitVector_Flags flags){
  int i;
  BitVector *ret = get_bit_vector_ret(this,flags);
  assert(this->size == other->size);
  for( i = 0; i < this->numBytes; i++)
    ret->bits[i] = this->bits[i] | other->bits[i];
  return ret;
}

 BitVector *BitVector_Xor(BitVector *this, BitVector *other,
		    BitVector_Flags flags){
  int i;
  BitVector*ret = get_bit_vector_ret(this,flags);
  assert(this->size == other->size);
  for( i = 0; i < this->numBytes; i++)
    ret->bits[i] = this->bits[i] ^ other->bits[i];
  return ret;
}

 BitVector *BitVector_Not(BitVector *this,
		      BitVector_Flags flags){
  int i;
  BitVector*ret = get_bit_vector_ret(this,flags);
  for( i = 0; i < this->numBytes; i++)
    ret->bits[i] = ~this->bits[i];
  return ret;
}

char BitVector_GetBit(BitVector *this, int index){
  assert(index < this->size);
  int mjrIndex = index/CHAR_BIT;
  int mnrIndex = index % CHAR_BIT;
  char mask = 1 << mnrIndex;
  return this->bits[mjrIndex] & mask;
}

void BitVector_SetBitOn(BitVector *this,
		   int index,
		   BitVector_Flags flags){
  assert(index < this->size);
  int mjrIndex = index/CHAR_BIT;
  int mnrIndex = index % CHAR_BIT;
  char mask = 1 << mnrIndex;
  this->bits[mjrIndex] = this->bits[mjrIndex] | mask;
  return;
}

 void BitVector_SetBitOff(BitVector *this,
		   int index,
		   BitVector_Flags flags){
  assert(index < this->size);
  int mjrIndex = index/CHAR_BIT ;
  int mnrIndex = index % CHAR_BIT ;
  char mask = ~(1 << mnrIndex);//Shift over to the right place, and the negate
  this->bits[mjrIndex] = this->bits[mjrIndex] & mask;
  return;
}
 void BitVector_SetByte(BitVector *this,
		 int index,
		 char value,
		 BitVector_Flags flags){
  this->bits[index]=value;
}  
 char *BitVector_GetBits(BitVector *this,BitVector_Flags flags){
  return this->bits;
}
 int BitVector_GetSize(BitVector *this,BitVector_Flags flags){
  return this->size;
}
 int BitVector_GetNumBytes(BitVector *this,BitVector_Flags flags){
  return this->numBytes;
}
char BitVector_Equals(BitVector *this,BitVector *other,
		      BitVector_Flags flags){
  int ret;
  BitVector *test = BitVector_Xor(this,other,0);
  ret = check_vector(test, 0);
  BitVector_Destroy(test);
  return ret;
}
  
char *BitVector_ToString(BitVector *this, BitVector_Flags flags){
  int i,j;
  char *ret = this->allocator_func(this->size+1);
  for(i = 0; i * CHAR_BIT < this->size; i++){
    char data = this->bits[i];
    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < this->size); j++){
      char mask = 1 << j; 
      ret[i* CHAR_BIT +j] = (data & mask) ? '1' : '0';
    }
  }
  ret[this->size + 1] = 0;
  return ret;
}

int BitVector_GetNumOnBits(BitVector *this, BitVector_Flags flags){
  int i,j;
  int ret = 0;
  for(i = 0; i * CHAR_BIT < this->size; i++){
    char data = this->bits[i];
    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < this->size); j++){
      char mask = 1 << j; 
      ret += (data & mask)? 1 : 0;
    }
  }
  return ret;
}

/**Private helper functions */
static char check_vector(BitVector *bv, char value){
  char ret = 1;
  int i,j;
  char *bits;
  int size;
  size = bv->size;
  bits = bv->bits;
  for(i = 0; i * CHAR_BIT < size; i++){
    char data = bits[i];
    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < size); j++){
      char mask = 1 << j; 
      char bit = (data & mask);
      if(bit != value){
	ret = 0;
	break;
      }
    }
    if(ret == 0)
      break; //terminate early if vector has a disagreable bit
  }
  return ret;
}
static BitVector *get_bit_vector_ret(BitVector *this,
				     BitVector_Flags flags){
  BitVector *ret;
  if(flags & BV_OP_IN_PLACE){
    ret = this;
  }
  else{
    ret = BitVector_Create(this->size,
		  this->allocator_func,
		  this->deallocator_func,
		  0);
  }
  return ret;
}
