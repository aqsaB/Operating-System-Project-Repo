#include <future.h>
#include <xinu.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
    intmask mask;
    mask = disable();     
     struct future_t* f1 = (future_t *) getmem(sizeof(future_t)); 
     //{(void *) getmem(size),size,FUTURE_EMPTY, mode,getpid()};
     f1->mode = mode;
     f1->size = size;
     f1->data = getmem(size);
     f1->state = FUTURE_EMPTY;
     f1->pid = getpid();
     restore(mask);
     return f1;
 }

syscall future_free(future_t* f){
	intmask mask;
	mask = disable();	
	kill(f->pid);
	
	if (((int)(freemem( f, sizeof(f)))) < 1){
		printf("Error freeing future");
		return SYSERR;
	}
	restore(mask);
	return OK;

}

syscall future_get(future_t* f, void* out){
//if we call future_get and another process is already waiting on this future then we will return syserr
	intmask mask;
	mask = disable();
	//printf("\nInside get\n");
	if(f->state == FUTURE_EMPTY){
		f->state = FUTURE_WAITING;
		f->pid = getpid();
		suspend(f->pid);
		//printf("\nconsumer process resuming after suspend");
		memcpy(out, f->data, sizeof(f->data));
		f->state = FUTURE_EMPTY;
		return OK;
	}
	else if(f->state == FUTURE_WAITING){
		return SYSERR;
	}
	else if(f->state == FUTURE_READY){
		memcpy(out, f->data, sizeof(f->data));
		f->state = FUTURE_EMPTY;
		return OK;
	}
	restore(mask);
	return OK;
	
}

syscall future_set(future_t* f, void* in){
	intmask mask;
	mask = disable();
	//printf("\nInside set\n");
	if(f->state == FUTURE_EMPTY){
		f->state = FUTURE_READY;
		memcpy(f->data, in, sizeof(in));
		return OK;
	}
	else if(f->state == FUTURE_READY){
		return SYSERR;
	}
	else if(f->state == FUTURE_WAITING){
		memcpy(f->data, in, sizeof(in));
		resume(f->pid);
	//	f->state = FUTURE_READY;
		return OK;
	}
	restore(mask);
	return OK;
}


