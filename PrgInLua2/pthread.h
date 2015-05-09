/**lua程序设计2nd源代码
*/

#include <Windows.h>


#define pthread_t unsigned int
#define pthread_attr_t int
#define pthread_condattr_t int

int pthread_create(pthread_t *tidp, pthread_attr_t attr, void(start_rtn)(void*), void *arg);
void pthread_exit(void *rval_ptr);
int pthread_detach(pthread_t tid);

class pthread_mutex_t
{
public:
	pthread_mutex_t() { h = CreateMutex(NULL, FALSE, NULL); }
	~pthread_mutex_t() {}
	
	HANDLE h;
};

class pthread_cond_t
{
public:
	pthread_cond_t() {}
	~pthread_cond_t() {}

	HANDLE h;
};

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);

pthread_t pthread_self();

/*
#define pthread_cond_t HANDLE
//#define pthread_cond_wait(cond,
#define pthread_create(t, n, f, arg) thread = _beginthread(f, 0, arg); return thread==-1;
#define pthread_detach(t)
#define pthread_exit(t)

#define pthread_self GetCurrentThread

void pthread_cond_init(&self->cond, NULL)
{
}

*/