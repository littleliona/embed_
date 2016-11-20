#ifndef MEM_H
#define MEM_H
#include<stdio.h>

#define Pool_A	    1
#define Pool_B	    2
#define Heap        3
#define Available   1
#define Occupy      2
typedef struct block *block_ptr;
typedef struct pool *pool_ptr;
typedef struct memory *memory_ptr;

typedef struct block
{
	int pool_id;
    	int state;
	block_ptr next;
}block;

typedef struct pool
{
	int available_block_num;
	int block_size;
	block_ptr first_available_block;
	block_ptr head_node;
	pool_ptr next;
}pool;

typedef struct memory
{
	int num_of_pool;
	pool_ptr head;
}memory;

memory_ptr init(void);
void* Malloc(int size);
void Free(void* tempPtr);
void delete_pool(void);

#endif
