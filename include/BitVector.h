/**
 * BitVector
 * Alexander Fairley
 * An arbitrary length bitvector supporting in-place and productive
 * bitwise operations 
 */
//TODO Remove passing of flags where they aren't used
#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_

#include <unistd.h>

typedef struct bit_vector_t BitVector;

/** Contains flags which effect the functioning of certain bitvector
 *  operations*/ 
typedef int BitVector_Flags;

/* And here are some flags you can use */

/** Used to indicate that the results of a bit vector operation should
 *  be stored in itthis*/ 
#define BV_OP_IN_PLACE 1
/** Used to indicate what a freshly allocated bitvector should be
 *  filled with  initially 0 if not set. */ 
#define BV_INIT_FILL 2

/*
 * API prototypes
 */

  /** 
   * Creates a new bitvector
   * allocator_func and deallocator_func are functions that the
   * hashtable will use for its internal memory allocation and
   * deallocation.  If unspecified, malloc(3) and free(3) will be
   * used.
   *
   * returns 0 if successful, 1 otherwise.
   */
 BitVector *BitVector_Create( int size,                      
		  void * (* allocator_func)(size_t),
		  void (*free_func)(void *),
		  BitVector_Flags flags);

/**
 * Destroys a BitVector. 
 */
 void BitVector_Destroy(BitVector *this);

/**
 * Carry out various bitwise operations on the contents of 
 * *this and other, persuant to the specifications of flags
 */
 BitVector *BitVector_And(BitVector *this, BitVector *other,
		    BitVector_Flags flags);
 BitVector *BitVector_Or(BitVector *this, BitVector *other,
		   BitVector_Flags flags);
 BitVector *BitVector_Xor(BitVector *this, BitVector *other,
		    BitVector_Flags flags);
 BitVector *BitVector_Not(BitVector *this,
		      BitVector_Flags flags);
char BitVector_Equals(BitVector *this, BitVector *other, 
		      BitVector_Flags flags);
char BitVector_GetBit(BitVector *this, int index);
/**Set individual bits on or off*/
void BitVector_SetBitOn(BitVector *this,
			int index,
			BitVector_Flags flags);
void BitVector_SetBitOff(BitVector *this,
		      int index,
			 BitVector_Flags flags);
/**Set a byte's worth of memory located at index to value. The first
   index of a bit within a byte has mask 1 << 0, the second 
   has mask 1 << 1.  So, for example, to set the first bit of the
   second byte of a bitvector(index 9 wrt setBitOn)
   on and the following seven off you could call like so:
   
   BitVec.setByte(bitvec, 1, 1, 0);
   
*/
void BitVector_SetByte(BitVector *this,
		    int index,
		    char value,
		    BitVector_Flags flags);
/**Field accessors*/
char *BitVector_GetBits(BitVector *this, BitVector_Flags flags);
int BitVector_GetSize(BitVector *this, BitVector_Flags flags);
int BitVector_GetNumOnBits(BitVector *this, BitVector_Flags flags);
int BitVector_GetNumBytes(BitVector *this, BitVector_Flags flags);
/**String Representation*/
char *BitVector_ToString(BitVector *this, BitVector_Flags flags);

#endif
