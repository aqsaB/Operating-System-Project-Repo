#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 *  * Copy 'len' bytes from 'buff' into the blocks associated with the file 'fd'
 *   *   - Until 'len' bytes are copied...
 *    *   - Check if a new block is needed (find free block and add to the file's blocks if necessary)
 *     *   - Write bytes from the 'buff' into the block starting at the fileptr
 *      *   - Write 'len' bytes or fill the block, whichever is less
 *       *   - Repeat until write is complete
 *        *   - Update inode state to reflect changes
 *         *   - Return the number of bytes written
 *          */
int get_free_block(){
	for(int k = 0; k<fsd->device.nblocks; k++){
		if(fs_getmaskbit(k) == 0){
			return k;
		}
	}
	return -1;
}
int fs_write(int fd, char* buff, int len) {
	int counter = 0;
	int write_sz = 1;
	int head = oft[fd].fileptr;
	int block_sz = fsd->device.blocksz;

	int total = len;
	
	while(total>0){
		/*if (head%block_sz == 0 head > oft[fd].in.size){
			int page_index = head/block_sz;
			oft[fd].in.blocks[page_index];	
		}*/

		bs_write(oft[fd].in.blocks[head/block_sz], head%block_sz, &buff[counter], write_sz);
		head += write_sz;
		counter += 1;
		total-=1;

	}
	oft[fd].fileptr = head;
	oft[fd].in.size = head;

	return counter;
}
