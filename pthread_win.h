/*
File:   pthread_win.h
Author: Hanson
Desc:   Define the prototype of some POSIX thread functions for Windows
Note:   Huge thanks to https://nachtimwald.com/2019/04/05/cross-platform-thread-wrapper/
*/

#pragma once

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#else
#include <pthread.h>
#endif  // _WIN32

#ifdef _WIN32
typedef HANDLE pthread_t;
typedef void pthread_attr_t;
typedef CRITICAL_SECTION pthread_mutex_t;
typedef void pthread_mutexattr_t;
typedef CONDITION_VARIABLE pthread_cond_t;
typedef void pthread_condattr_t;
#endif  // _WIN32

#ifdef _WIN32

/*
Function:   pthread_create
Desc:       Create a thread
Args:       thread: A pointer to a pthread_t to store the handle to the created thread
.           attr: Unused on Windows. Set it to NULL
.           start_routine: Thread function
.           args: Parameters to the thread
Return:     0 if the thread is created successfully. 1 otherwise
*/
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *args);

/*
Function:   pthread_join
Desc:       Wait for the specified thread to terminate
Args:       thread: The thread to be joined
.           retval: Unused on Windows. Set it to NULL
Return:     Always 0
*/
int pthread_join(pthread_t thread, void **retval);

/*
Function:   pthread_detach
Desc:       Detach the specified thread
Args:       thread: The thread to be detached
Return:     Always 0
*/
int pthread_detach(pthread_t thread);

/*
Function:   pthread_mutex_init
Desc:       Initialize a mutex
Args:       mutex: A pointer to the mutex to be initialized
.           attr: Unused on Windows. Set it to NULL
Return      0 if the mutex is initialized. 1 otherwise
*/
int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr);

/*
Function:   pthread_mutex_destroy
Desc:       Delete a mutex
Args:       mutex: A pointer to the mutex to be deleted
Return      0 if the mutex is deleted. 1 otherwise
*/
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/*
Function:   pthread_mutex_lock
Desc:       Lock a mutex
Args:       mutex: A pointer to the mutex to be locked
Return      0 if the mutex is locked. 1 otherwise
*/
int pthread_mutex_lock(pthread_mutex_t *mutex);

/*
Function:   pthread_mutex_unlock
Desc:       Unlock a mutex
Args:       mutex: A pointer to the mutex to be unlocked
Return      0 if the mutex is unlocked. 1 otherwise
*/
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/*
Function:   pthread_cond_init
Desc:       Initialize a condition variable
Args:       cond: A pointer to the condition variable to be initialized
.           attr: Unused on Windows. Set it to NULL
Return      0 if the mutex is initialized. 1 otherwise
*/
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);

/*
Function:   pthread_cond_destroy
Desc:       Delete a condition variable
Args:       mutex: A pointer to the mutex to be deleted
Return:     Always 0
WARNING:    This function is unused on Windows
*/
int pthread_cond_destroy(pthread_cond_t *cond);

/*
Function:   pthread_cond_wait
Desc:       Sleep on the specified condition variable and releases
.           the specified critical section as an atomic operation
Args:       cond: A pointer to the condition variable
.           mutex: A pointer to the mutex
Return:     0 if successful, 1 otherwise
Reference:  https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-initializeconditionvariable
*/
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/*
Function:   pthread_cond_timedwait
Desc:       Sleep on the specified conditional variable and releases
.           the specified critical section as an atomic operation, with
.           the specified timeout
Args:       cond: A pointer to the condition variable
.           mutex: A pointer to the mutex
.           abstime: The time-out interval, in milliseconds
Return:     0 if successful, 1 if the time-out interval elapses or the function fails
Reference:  https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleepconditionvariablecs
*/
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

/*
Function:   pthread_cond_signal
Desc:       Wake a single thread waiting on the specified condition variable
Args:       cond: A pointer to the condition variable
Return:     0 if successful, 1 otherwise
*/
int pthread_cond_signal(pthread_cond_t *cond);

/*
Function:   pthread_cond_broadcast
Desc:       Wake all threads waiting on the specified condition variable
Args:       cond: A pointer to the condition variable
Return:     0 if successful, 1 otherwise
*/
int pthread_cond_broadcast(pthread_cond_t *cond);

#endif  // _WIN32
