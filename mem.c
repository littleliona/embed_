#include"mem.h"

memory_ptr mem_ptr;
block_ptr heapPtr;

memory_ptr init(void)
{
	int i;
	pool_ptr pool_node;
	block_ptr block_node;
	block_ptr temp_ptr;
	
	mem_ptr = (memory_ptr)malloc(15000);
	mem_ptr->num_of_pool = 2;
	/*mem_ptr->head = (pool_ptr)((char *)mem_ptr + sizeof(memory));*/
	
	/*50*/
	pool_node = (pool_ptr)((char *)mem_ptr + sizeof(memory));
	mem_ptr->head = pool_node;
	pool_node->available_block_num = 20;
	pool_node->block_size = 50;
	block_node = (block_ptr)((char *)pool_node + sizeof(pool));
    pool_node->head_node = block_node;
   	pool_node->next = (pool_ptr)((char *)pool_node + sizeof(pool) + 20*(sizeof(block) + 50));
    
	block_node->pool_id = Pool_A;
    block_node->state = Available;
	block_node->next = NULL;
	
	

	for (i = 0; i < 19; i++)
	{
		temp_ptr = (block_ptr)((char *)block_node + sizeof(block) + 50);
		temp_ptr->pool_id = Pool_A;
        temp_ptr->state = Available;
        temp_ptr->next = pool_node->head_node;
        pool_node->head_node = temp_ptr;
		block_node = temp_ptr;
	}

	pool_node->first_available_block = pool_node->head_node;
	/*100*/
	pool_node = pool_node->next;
	pool_node->available_block_num = 20;
	pool_node->block_size = 100;
	block_node = (block_ptr)((char *)pool_node + sizeof(pool));
    	pool_node->head_node = block_node;
    	pool_node->next = NULL;

    
    	block_node->pool_id = Pool_B;
    	block_node->state = Available;
	block_node->next = NULL;
	
	

	for (i = 0; i < 19; i++)
	{
		temp_ptr = (block_ptr)((char *)block_node + sizeof(block) + 100);
		temp_ptr->pool_id = Pool_B;
        	temp_ptr->state = Available;
        	temp_ptr->next = pool_node->head_node;
        	pool_node->head_node = temp_ptr;
			block_node = temp_ptr;
	}
	pool_node->first_available_block = pool_node->head_node;
	/*heap*/
	heapPtr = NULL;
	printf("\nInit Success!\n");
	return (memory_ptr)mem_ptr;
}

void* Malloc(int size)
{
	void* temp;
	pool_ptr pool_node;
	block_ptr block_node;
    	block_ptr temp_block;

	pool_node = mem_ptr->head;

	if (size <= 50 &&  pool_node->available_block_num != 0)
	{
		block_node = pool_node->first_available_block;
        	block_node->state = Occupy;
        	temp_block = block_node->next;
        	pool_node->available_block_num--;
        
        if(pool_node->available_block_num == 0)
        {
            pool_node->first_available_block = NULL;
        }
        else
        {
            while(temp_block!=NULL)
            {
                if(temp_block->state == Available)
                {
                    pool_node->first_available_block = temp_block;
                    break;
                }
                else
                {
                    temp_block = temp_block->next;
                }
            }
        }
        
        printf("\n Log: malloc success! pool:%d,Size:%d,Address:%d\n",block_node->pool_id,size,(void *)((char *)block_node + sizeof(block)));
	return (void *)((char *)block_node + sizeof(block));
	}
	else if (size > 50 && size < 100 &&  pool_node->next->available_block_num != 0)
	{
        	block_node = pool_node->next->first_available_block;
       		block_node->state = Occupy;
        	temp_block = block_node->next;
       		pool_node->available_block_num--;
        
        if(pool_node->next->available_block_num == 0)
        {
            pool_node->next->first_available_block = NULL;
        }
        else
        {
            while(temp_block!=NULL)
            {
                if(temp_block->state == Available)
                {
                    pool_node->next->first_available_block = temp_block;
                    break;
                }
                else
                {
                    temp_block = temp_block->next;
                }
            }
        }
		printf("\n Log: malloc success! pool:%d,Size:%d,Address:%d\n",block_node->pool_id,size,(void *)((char *)block_node + sizeof(block)));
		return (void *)((char *)block_node + sizeof(block));
	}
	else
	{
		temp = (void*)malloc(size);
		block_node = (block*)temp;
        
		block_node->pool_id = Heap;
        /*block_node->state = Occupy;*/
        block_node->next = heapPtr;
		heapPtr = block_node;
        
		printf("\n Log: malloc success! pool:%d,Size:%d,Address:%d\n",block_node->pool_id,size,(void *)((char *)block_node + sizeof(block)));
		return (void *)((char *)block_node + sizeof(block));
	}
}

void Free(void* temp_ptr)
{
	char* tempPtr_char = (char*)temp_ptr;
    
	pool_ptr pool_node;
	block_ptr block_node;
	block_ptr blockTemp,blockTempNext;
	pool_ptr poolTemp;
    
    	pool_node = mem_ptr->head;
    	block_node = (block_ptr)((char *)tempPtr_char - sizeof(block));

	
	
	if (block_node->pool_id == Pool_A)
	{
		poolTemp = pool_node;
	}
	else if (block_node->pool_id == Pool_B)
	{
		poolTemp = pool_node->next;
	}
	else if(block_node->pool_id != Heap)
	{
		printf("\n[Warning] : Incorrect address. \n");
        return ;
	}

	if (block_node->pool_id == 1 || block_node->pool_id == 2)
	{	
		if(block_node->state == Available)
        {
            printf("\n[Warning] : The block is free. \n");
            return ;
        }
        else
        {
            poolTemp->first_available_block = block_node;
            block_node->state = Available;
            poolTemp->available_block_num++;
            printf("\nLog :Free the block success!. \n");
        }
        
    }
    else
	{
		blockTemp = heapPtr;
		if(blockTemp == NULL)
		{
			printf("\nWarning : No block in the heap to free. \n");
			return;
		}
		blockTempNext = blockTemp->next;

		if (blockTemp != block_node)
		{
			while (blockTempNext != NULL)
			{
				if (blockTempNext == block_node)
				{
					blockTemp->next = blockTempNext->next;
					break;
				}
				else
				{
					blockTemp = blockTempNext;
					blockTempNext = blockTemp->next;
				}
			}
			if (blockTempNext == NULL)
			{
				printf("\nWarning : Incorrect address. \n");
				return;
			}
		}
		else
		{
			heapPtr = blockTemp->next;
		}
		free((char *)tempPtr_char - sizeof(block));
	}
}

void delete_pool(void)
{
	void* temp;
	temp = (void *)mem_ptr;
	free(temp);
	
	while (heapPtr != NULL)
	{
		temp = (void *)heapPtr;
		free(temp);
		heapPtr = heapPtr->next;
	}
	
}
