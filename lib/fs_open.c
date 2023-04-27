#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 *  * fs_open - Return SYSERR if the file is already open or does not exist
 *   *     otherwise, add a record to the oft in the first current FSTATE_CLOSED
 *    *     slot corresponding to the opened file and copy the inode from the block
 *     *     device to the oft record and return the index into the oft table for the
 *      *     newly opened file.
 *       */
int fs_open(char* filename, int flags) {
	for(int i = 0; i< NUM_FD; i++){
		if(oft[i].state == FSTATE_OPEN && strcmp(filename, fsd->root_dir.entry[oft[i].de].name) == 0){
			return SYSERR;
		}
	}

	for(int i = 0; i<fsd->root_dir.numentries; i++){
		if(strcmp(filename, fsd->root_dir.entry[i].name) == 0){
			int x = fsd->root_dir.entry[i].inode_block;
			/*
			if(fs_getmaskbit(x)){
				return SYSERR;
			} */
			for (int j = 0; j < NUM_FD; j++){
				if(oft[j].state == FSTATE_CLOSED){
					//printf("\n in here");
					oft[j].state = FSTATE_OPEN;
					oft[j].fileptr = 0;
					oft[j].flag = flags;
					oft[j].de = i;
					bs_read(x, 0, &oft[j].in,sizeof(inode_t));
					break;
				}
			
			}
			return i;
		}
	}
	  
	  return SYSERR;
}
