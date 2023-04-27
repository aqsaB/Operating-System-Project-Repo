#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 *  * Copy the data from the block device into the 'buff' argument
 *   *    Use the fileptr field of the oft to track the location of the next read/write
 *    *    operation.
 *     *    - Determine the current block and offset from the fileptr
 *      *    - Copy bytes into the 'buff' until you have exhausted the block or copied 'len' bytes
 *       *    - Repeat until 'len' bytes are copied
 *        *    - Update the fileptr field
 *         *    - Return the number of bytes read
 *          */
int fs_read(int fd, char* buff, int len) {
	int counter = 0;
	int total = len;
	int head = oft[fd].fileptr;
	int block_sz = fsd->device.blocksz;
	int read_sz = 1;
	
	if(total > oft[fd].in.size - head){
		total = oft[fd].in.size - head;
	}
	while (total>0){
	bs_read(oft[fd].in.blocks[head/block_sz],head%block_sz,&buff[counter], read_sz);
	head += read_sz;
	total -= 1;
	counter += 1;
	}
	oft[fd].fileptr = head;
	return counter;
}
