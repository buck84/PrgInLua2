/**lua程序设计2nd源代码
*/
#include <process.h>
#include "pthread.h"

int pthread_create(pthread_t *tidp, pthread_attr_t attr, void (start_rtn)(void*), void *arg)
{
	*tidp= _beginthread(start_rtn, 0, arg);
	if(*tidp!=-1)
		return 0;
	else
		return -1;
}

void pthread_exit(void *rval_ptr)
{
}

int pthread_detach(pthread_t tid)
{
	return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	return 0;
}

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr)
{
	cond->h = CreateEvent(NULL, TRUE, FALSE, NULL);
	return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
	return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
	::SetEvent(cond->h);
	return 0;
}

pthread_t pthread_self()
{
	return (unsigned int)GetCurrentThread();
}