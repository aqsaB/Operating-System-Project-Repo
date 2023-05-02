#include <xinu.h>
extern sid32 run_sem;
//extern sid32 print_sem;
typedef enum {
FUTURE_EMPTY,
FUTURE_WAITING,
FUTURE_READY
} future_state_t;

typedef enum {
FUTURE_EXCLUSIVE,
FUTURE_SHARED,
FUTURE_QUEUE
} future_mode_t;

typedef struct future_t {
void *data;
uint size;
future_state_t state;
future_mode_t mode;
qid16 get_queue;
pid32 pid;
} future_t;


future_t* future_alloc(future_mode_t mode, uint size, uint nelems);
syscall future_free(future_t*);
syscall future_get(future_t*, void*);
syscall future_set(future_t*, void*);

//added for future shared
int future_fib(int nargs, char *args[]);
int future_free_test(int nargs, char *args[]);

syscall future_prod(future_t*, int value);
syscall future_cons(future_t*);
