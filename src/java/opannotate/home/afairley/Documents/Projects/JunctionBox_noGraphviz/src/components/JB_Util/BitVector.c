               :#include "BitVector.h"
               :#include <limits.h>
               :#include <stdlib.h>
               :#include <assert.h>
               :#include <string.h>
               :#include <math.h>
               :
               :
               :
               :/***********************************************************************
               : **** INFRASTRUCTURE
               : ***********************************************************************/
               :struct bit_vector_t{
               :  char *bits;
               :  int size;
               :  int numBytes;
               :  void * (* allocator_func)(size_t);
               :  void (* deallocator_func)(void *);
               :};
               :
               :
               ://Private function protos
               :static BitVector *get_bit_vector_ret(BitVector *this,
               :				      BitVector_Flags flags);
               :static char check_vector(BitVector *bv, char value);
               :/***********************************************************************
               : **** IMPLEMENTATION
               : ***********************************************************************/
               :  
               : BitVector *BitVector_Create(int size,                      
               :		  void * (* allocator_func)(size_t),
               :		  void (*free_func)(void *),
               :		  BitVector_Flags flags){ /* BitVector_Create total:     11  0.9061 */
               :  BitVector *ret = NULL;
               :  int init_val;
               :  //initialize bits to either 0 or 1
               :  if(flags & BV_INIT_FILL){
               :    init_val = SCHAR_MAX;
               :  }else{
               :    init_val = 0;
               :  }
               :  if (allocator_func) {
     1  0.0824 :    ret= allocator_func(sizeof(BitVector));
               :    assert(ret!= NULL);
               :  }
               :  else {
               :    ret= malloc(sizeof(BitVector));
               :    assert(ret!= NULL);
               :  }
     1  0.0824 :  memset(ret, 0, sizeof(BitVector));
               :  ret->size = size;
     5  0.4119 :  ret->numBytes = ceil(size/ (double)CHAR_BIT );
     1  0.0824 :  ret->allocator_func = (allocator_func ? allocator_func : malloc);
     1  0.0824 :  ret->deallocator_func = (free_func ? free_func : free);
     2  0.1647 :  ret->bits = (char*) ret->allocator_func(ret->numBytes);
               :  //initialize bits to either 0 or 1
               :  memset(ret->bits,
               :	 init_val,
               :	 ret->numBytes);
               :  return ret;
               :}
               :
               : void BitVector_Destroy(BitVector *this){ /* BitVector_Destroy total:      2  0.1647 */
               :  assert(this != NULL);
               :  void (*free_func)(void *) = this->deallocator_func;
     1  0.0824 :  free_func(this->bits);
               :  free_func(this);
     1  0.0824 :}
               :
               : BitVector *BitVector_And(BitVector *this, BitVector *other,
               :		      BitVector_Flags flags){ /* BitVector_And total:     26  2.1417 */
               :  int i;
     2  0.1647 :  BitVector*ret = get_bit_vector_ret(this,flags);
               :  assert(this->size == other->size);
     8  0.6590 :  for( i = 0; i < this->numBytes; i++)
    16  1.3180 :    ret->bits[i] = this->bits[i] & other->bits[i];
               :  return ret;
               :}
               :
               : BitVector *BitVector_Or(BitVector *this, BitVector *other,
               :		   BitVector_Flags flags){
               :  int i;
               :  BitVector *ret = get_bit_vector_ret(this,flags);
               :  assert(this->size == other->size);
               :  for( i = 0; i < this->numBytes; i++)
               :    ret->bits[i] = this->bits[i] | other->bits[i];
               :  return ret;
               :}
               :
               : BitVector *BitVector_Xor(BitVector *this, BitVector *other,
     1  0.0824 :		    BitVector_Flags flags){ /* BitVector_Xor total:     14  1.1532 */
               :  int i;
               :  BitVector*ret = get_bit_vector_ret(this,flags);
               :  assert(this->size == other->size);
     2  0.1647 :  for( i = 0; i < this->numBytes; i++)
    11  0.9061 :    ret->bits[i] = this->bits[i] ^ other->bits[i];
               :  return ret;
               :}
               :
               : BitVector *BitVector_Not(BitVector *this,
               :		      BitVector_Flags flags){
               :  int i;
               :  BitVector*ret = get_bit_vector_ret(this,flags);
               :  for( i = 0; i < this->numBytes; i++)
               :    ret->bits[i] = ~this->bits[i];
               :  return ret;
               :}
               :
    20  1.6474 :char BitVector_GetBit(BitVector *this, int index){ /* BitVector_GetBit total:    149 12.2735 */
     8  0.6590 :  assert(index < this->size);
    16  1.3180 :  int mjrIndex = index/CHAR_BIT;
    14  1.1532 :  int mnrIndex = index % CHAR_BIT;
    24  1.9769 :  char mask = 1 << mnrIndex;
    49  4.0362 :  return this->bits[mjrIndex] & mask;
    18  1.4827 :}
               :
               :void BitVector_SetBitOn(BitVector *this,
               :		   int index,
               :		   BitVector_Flags flags){ /* BitVector_SetBitOn total:      1  0.0824 */
               :  assert(index < this->size);
               :  int mjrIndex = index/CHAR_BIT;
               :  int mnrIndex = index % CHAR_BIT;
               :  char mask = 1 << mnrIndex;
     1  0.0824 :  this->bits[mjrIndex] = this->bits[mjrIndex] | mask;
               :  return;
               :}
               :
               : void BitVector_SetBitOff(BitVector *this,
               :		   int index,
               :		   BitVector_Flags flags){
               :  assert(index < this->size);
               :  int mjrIndex = index/CHAR_BIT ;
               :  int mnrIndex = index % CHAR_BIT ;
               :  char mask = ~(1 << mnrIndex);//Shift over to the right place, and the negate
               :  this->bits[mjrIndex] = this->bits[mjrIndex] & mask;
               :  return;
               :}
               : void BitVector_SetByte(BitVector *this,
               :		 int index,
               :		 char value,
               :		 BitVector_Flags flags){
               :  this->bits[index]=value;
               :}  
               : char *BitVector_GetBits(BitVector *this,BitVector_Flags flags){
               :  return this->bits;
               :}
               : int BitVector_GetSize(BitVector *this,BitVector_Flags flags){ /* BitVector_GetSize total:      2  0.1647 */
     1  0.0824 :  return this->size;
     1  0.0824 :}
               : int BitVector_GetNumBytes(BitVector *this,BitVector_Flags flags){
               :  return this->numBytes;
               :}
               :char BitVector_Equals(BitVector *this,BitVector *other,
               :		      BitVector_Flags flags){ /* BitVector_Equals total:      3  0.2471 */
               :  int ret;
               :  BitVector *test = BitVector_Xor(this,other,0);
               :  ret = check_vector(test, 0);
     3  0.2471 :  BitVector_Destroy(test);
               :  return ret;
               :}
               :  
               :char *BitVector_ToString(BitVector *this, BitVector_Flags flags){
               :  int i,j;
               :  char *ret = this->allocator_func(this->size+1);
               :  for(i = 0; i * CHAR_BIT < this->size; i++){
               :    char data = this->bits[i];
               :    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < this->size); j++){
               :      char mask = 1 << j; 
               :      ret[i* CHAR_BIT +j] = (data & mask) ? '1' : '0';
               :    }
               :  }
               :  ret[this->size + 1] = 0;
               :  return ret;
               :}
               :
     1  0.0824 :int BitVector_GetNumOnBits(BitVector *this, BitVector_Flags flags){ /* BitVector_GetNumOnBits total:    708 58.3196 */
               :  int i,j;
               :  int ret = 0;
    63  5.1895 :  for(i = 0; i * CHAR_BIT < this->size; i++){
    66  5.4366 :    char data = this->bits[i];
   394 32.4547 :    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < this->size); j++){
    43  3.5420 :      char mask = 1 << j; 
   139 11.4498 :      ret += (data & mask)? 1 : 0;
               :    }
               :  }
     1  0.0824 :  return ret;
     1  0.0824 :}
               :
               :/**Private helper functions */
               :static char check_vector(BitVector *bv, char value){ /* check_vector total:     42  3.4596 */
               :  char ret = 1;
               :  int i,j;
               :  char *bits;
               :  int size;
               :  size = bv->size;
               :  bits = bv->bits;
     3  0.2471 :  for(i = 0; i * CHAR_BIT < size; i++){
     1  0.0824 :    char data = bits[i];
    18  1.4827 :    for(j = 0; j < CHAR_BIT && (i * CHAR_BIT + j < size); j++){
     1  0.0824 :      char mask = 1 << j; 
    11  0.9061 :      char bit = (data & mask);
     5  0.4119 :      if(bit != value){
     1  0.0824 :	ret = 0;
     1  0.0824 :	break;
               :      }
               :    }
     1  0.0824 :    if(ret == 0)
               :      break; //terminate early if vector has a disagreable bit
               :  }
               :  return ret;
               :}
               :static BitVector *get_bit_vector_ret(BitVector *this,
               :				     BitVector_Flags flags){ /* get_bit_vector_ret total:      5  0.4119 */
               :  BitVector *ret;
               :  if(flags & BV_OP_IN_PLACE){
               :    ret = this;
               :  }
               :  else{
     3  0.2471 :    ret = BitVector_Create(this->size,
               :		  this->allocator_func,
               :		  this->deallocator_func,
               :		  0);
               :  }
     1  0.0824 :  return ret;
     1  0.0824 :}
/* 
 * Total samples for file : "/home/afairley/Documents/Projects/JunctionBox_noGraphviz/src/components/JB_Util/BitVector.c"
 * 
 *    963 79.3245
 */


/* 
 * Command line: opannotate --source -o opannotate/ libjunctionbox.so 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: Core 2, speed 2200 MHz (estimated)
 * Counted CPU_CLK_UNHALTED events (Clock cycles when not halted) with a unit mask of 0x00 (Unhalted core cycles) count 100000
 */
