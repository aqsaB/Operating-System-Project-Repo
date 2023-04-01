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
	free_blk* free_list = heap->freelist;

	intmask mask = disable();
	//printf("\nfree_list %d", (char*) free_list);
	//printf("\nheap_start %d", heap_start);
	if (free_list == NULL){
		new_block->next = NULL;
		new_block->prev = NULL;
		free_list = new_block;
		restore(mask);
		return;
	}
	while(((char*) free_list + free_list->size) < (char*) new_block && free_list->next!=NULL){
		free_list = free_list->next;
	}
	//block to be free is at the tail of the free list
	if ((char*) free_list + free_list->size == (char *) new_block){
		free_list->next = NULL;
		free_list->size += new_block_size;
//insert in between	
//	new_block->prev = free_list;
//	new_block->next = free_list->next;
//	free_list->next = new_block;
	}
	else if( (char *) new_block < (char*) free_list){
		
		
	}
	if((char *) free_list + free_list->size == (char *) new_block){
		free_list->next = new_block->next;
		free_list->size += new_block_size;	
	}
	if((char *) free_list + free_list->size == (char *) free_list->next){
		free_list->next = new_block->next->next;
		free_list->size += new_block->next->size;

	}
	heaptab[getpid()].freelist = heap->freelist;
	heaptab[getpid()].size = heap->size+new_block_size;
	//heaptab[getpid()].startaddr = (char *) heap->freelist;
	restore(mask);		
	return;

 }



