#include <future.h>
#include <xinu.h>
//#include <queue.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
    intmask mask;
    mask = disable();     
     struct future_t* f1 = (future_t *) getmem(sizeof(future_t)); 
     f1->mode = mode;
     f1->size = size;
     f1->data = getmem(size);
     f1->state = FUTURE_EMPTY;
     //f1->pid = getpid();
     if(mode == FUTURE_SHARED){
     	f1->get_queue = newqueue();
     }
     restore(mask);
     return f1;
 }

syscall future_free(future_t* f){
	intmask mask;
	mask = disable();	
	if(f->mode == FUTURE_EXCLUSIVE){
		kill(f->pid);
	}
	else if(f->mode == FUTURE_SHARED){
		while(nonempty(f->get_queue)) {
			kill(dequeue(f->get_queue));
		}
	}
//	freemem((char *) f, sizeof(future_t));

	if (((int)(freemem( (char *) f, sizeof(future_t *)))) < 1){
	 
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
	//printf("\nInside get");
	if(f->mode == FUTURE_EXCLUSIVE){
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
			//printf("\ngetting value when future is in ready state");
			return OK;
		}
	}
	else if(f->mode == FUTURE_SHARED){
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING){
			f->state = FUTURE_WAITING;
			//put pid in system queue
			pid32 p_pid = getpid();
			enqueue(p_pid, f->get_queue);
			suspend(p_pid);
			//resuming after suspend
			memcpy(out, f->data, sizeof(f->data));
			return OK;
		}
		
		else if(f->state == FUTURE_READY){
			memcpy(out, f->data, sizeof(f->data));
			return OK;
		}
	}
	restore(mask);
	return OK;
}

syscall future_set(future_t* f, void* in){
	intmask mask;
	mask = disable();
	//printf("\nInside set\n");
	if(f->mode == FUTURE_EXCLUSIVE){
		if(f->state == FUTURE_EMPTY){
			//printf("\nSetting value when future is empty");
			memcpy(f->data, in, sizeof(in));
			f->state = FUTURE_READY;
			return OK;
		}
		else if(f->state == FUTURE_READY){
			return SYSERR;
		}
		else if(f->state == FUTURE_WAITING){
			//printf("\nSetting value when future is in waiting state");
			memcpy(f->data, in, sizeof(in));
			resume(f->pid);
			//f->state = FUTURE_READY;
			return OK;
		}
	}
	else if(f->mode == FUTURE_SHARED){
		if(f->state == FUTURE_READY){
			return SYSERR;
		}
		else if(f->state == FUTURE_EMPTY){
			memcpy(f->data, in, sizeof(in));
			f->state = FUTURE_READY;
			return OK;
		}
		else if(f->state == FUTURE_WAITING){
			memcpy(f->data, in, sizeof(in));
			while(nonempty(f->get_queue)){
				resume(dequeue(f->get_queue));
			}
			f->state = FUTURE_READY;
			return OK;
		}
	}
	restore(mask);
	return OK;
}


