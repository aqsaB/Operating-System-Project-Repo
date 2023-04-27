#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;

/*
 *  * fs_create - Return SYSERR if not enough space is available
 *   *     in the directory or the filename already exists.
 *    *     Otherwise:
 *     *        1. Find an available block on the block store
 *      *        2. Create an inode_t for the new file
 *       *        3. Update the root directory
 *        *        4. Write the inode and free bitmask back to the block device
 *         */
int get_block(){
	for(int k = 0; k<fsd->device.nblocks; k++){
		if(fs_getmaskbit(k) == 0){
			return k;
		}
	}
	return -1;
}
syscall fs_create(char* filename) {
	if(fsd->root_dir.numentries == DIR_SIZE){
		return SYSERR;
	}

	for(int i = 0; i<fsd->root_dir.numentries; i++){
		if(strcmp(fsd->root_dir.entry[i].name, filename) == 0){
			return SYSERR;
		}
	}
	int k = get_block();

	inode_t inode;
	inode.id = k;
	inode.size = 0;

	//printf("Total number of blocks %d, k = %d", fsd->device.nblocks,k);
	//copying k into inode_block to get the index of the new block
	int index = fsd->root_dir.numentries;
	//memcpy(fsd->root_dir.entry[fsd->root_dir.numentries++].inode_block,k,sizeof(int));
	fsd->root_dir.entry[index].inode_block = k;
	fs_setmaskbit(k);
	bs_write(k,0,&inode, sizeof(inode_t));
	fsd->root_dir.numentries++;
	memcpy(fsd->root_dir.entry[index].name, filename, strlen(filename)+1);
	//fs_print_fsd();
	//fs_print_root();
	//printf("\n");
	  
	  return OK;
}
