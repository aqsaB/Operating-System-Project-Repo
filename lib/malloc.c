#include <xinu.h>
#include <heap.h>

extern heap_t heaptab[];
void initheap(char* startaddr, uint32 size) {
	  /* This function is called when the heap is constructed */
	  /*   Use as needed to setup the free list               */
	intmask mask = disable();
	struct free_blk* block = (free_blk *) startaddr;
	block->prev = NULL;
	block->next = NULL;
	block->size = size;
	pid32 pid = getpid();
	heap_t heap = heaptab[pid];
	heap.freelist = block;
	//heaptab[pid] = heap;
	restore(mask);	
	return;
}

void* malloc(uint32 size) {
	  /* Implement memory allocation within the process heap here       */
	  /*   Your implementation MUST NOT use getmem                      */
	  /*   Your implementation SHOULD use explicit in-memory free lists */
	pid32 pid = getpid();
    	heap_t heap = heaptab[pid];
	free_blk* free_block = heap.freelist;
	size = roundmb(size);
	intmask mask = disable();
	while( free_block->size < size && free_block->next != NULL){
		free_block = free_block->next;
	}
	if(free_block == NULL || free_block->size < size){
		restore(mask);
		return (char*) SYSERR;
	}
	free_blk* left = free_block->prev;
	free_blk* right = free_block->next;
	uint32 res = (uint32) truncmb(free_block->size - size);

	if(res<8){
		if(left == NULL && right == NULL){
			heap.freelist = NULL;
		}
		else if(left == NULL){
			heap.freelist = right;
			right->prev = NULL;
		}
		else{
			left->next = NULL;
		}
	}
	else{
		free_blk* res_blk = (free_blk*) ((char*) free_block + size);
		res_blk->next = right;
		res_blk->prev = left;

		if(left == NULL && right == NULL){
			res_blk->size = res;
			heap.freelist = res_blk;
		}
		else if(left != NULL && right != NULL){
			res_blk->size = right->size + res;
			res_blk->next = right->next;
			left->next = res_blk;
		}
		else if (left == NULL){
			res_blk->size = res;
			heap.freelist = (free_blk*) res;
		}
		else{
			res_blk->size = res;
			left->next = res_blk;
		}
	}
	heaptab[pid] = heap;
	restore(mask);
	return (char*) free_block;
}

void free(char* block, uint32 size) {
	  /* Implement memory free within process heap here */
	  /*   Your implementation MUST implement coalesing */
	heap_t* heap = &heaptab[getpid()];
	char* heap_start = heap->startaddr;
	char* heap_end = heap->startaddr + heap->size;
	uint32 new_block_size = (uint32) roundmb(size);

	free_blk* new_block = (free_blk*) block;
	new_block->size = new_block_size;
	free_blk* free_block = heap->freelist;

	intmask mask = disable();
	//free list is empty so new block becomes the free list
	if (free_block==NULL){
//		printf("\n%d freelist is null",free_block->size);
		//new_block->next = NULL;
		//new_block->prev = NULL;
		free_block = new_block;
		free_block->size += new_block_size;
		free_block->next = NULL;
		heap->freelist = free_block;
//		printf("\n%d new size of freelist",heap->freelist->size);
		restore(mask);
		return;
	}
	//printf("\nIterating over the free list %d \t %d", (char*) free_block, (char*) new_block);
	else if((char*) new_block < (char*) free_block ){
//		printf("here");
		if((char*) new_block + new_block_size == (char*) free_block){
//			printf("Inside if loop");
			new_block->size += free_block->size;
			new_block->next = free_block->next;
			heap->freelist = new_block;
		}
		else{
//			printf("Inside else loop");
			new_block->next = free_block;
			heap->freelist = new_block;
		}

		restore(mask);
		return;
	}
	//iterate over the free list and check for the address of the block to be freed
	while((char*) free_block + free_block->size < (char*) new_block && free_block->next!=NULL){
		printf("\nIterating over the free list %d \t %d", (char*) free_block, (char*) new_block);
		free_block = free_block->next;
	}

	//block to be free is at the tail of the free list
	if ((char*) free_block + free_block->size == (char *) new_block){
		free_block->next = NULL;
		free_block->size += new_block_size;
	}
	//insert in between
	
	else if( (char *) new_block < (char*) free_block){
		new_block->prev = free_block;
	        new_block->next = free_block->next;
       		free_block->next = new_block;	
		
	}
	if((char *) free_block + free_block->size == (char *) new_block){
		free_block->next = new_block->next;
		free_block->size += new_block_size;	
	}
	if((char *) free_block + free_block->size == (char *) free_block->next){
		free_block->next = new_block->next->next;
		free_block->size += new_block->next->size;

	}
	
	restore(mask);		
	return;

 }



