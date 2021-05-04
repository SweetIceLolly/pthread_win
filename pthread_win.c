/*
File:   pthread_win.c
Author: Hanson
Desc:   Implement some POSIX thread functions for Windows
Note:   Huge thanks to https://nachtimwald.com/2019/04/05/cross-platform-thread-wrapper/
*/

#include "pthread_win.h"

#ifdef _WIN32

DWORD timespec_to_ms(const struct timespec *abstime);

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *args) {
    UNREFERENCED_PARAMETER(attr);
    if (thread && start_routine) {
        *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, args, 0, NULL);
        if (!*thread) {
            return 1;
        }
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_join(pthread_t thread, void **retval) {
    UNREFERENCED_PARAMETER(retval);
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    return 0;
}

int pthread_detach(pthread_t thread) {
    CloseHandle(thread);
    return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr) {
    UNREFERENCED_PARAMETER(attr);
    if (mutex) {
        InitializeCriticalSection(mutex);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    if (mutex) {
        DeleteCriticalSection(mutex);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
    if (mutex) {
        EnterCriticalSection(mutex);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    if (mutex) {
        LeaveCriticalSection(mutex);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr) {
    UNREFERENCED_PARAMETER(attr);
    if (cond) {
        InitializeConditionVariable(cond);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_cond_destroy(pthread_cond_t *cond) {
    return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    if (cond && mutex) {
        return pthread_cond_timedwait(cond, mutex, NULL);
    }
    else {
        return 1;
    }
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime) {
    if (cond && mutex) {
        if (!SleepConditionVariableCS(cond, mutex, timespec_to_ms(abstime))) {
            return 1;
        }
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_cond_signal(pthread_cond_t *cond) {
    if (cond) {
        WakeConditionVariable(cond);
        return 0;
    }
    else {
        return 1;
    }
}

int pthread_cond_broadcast(pthread_cond_t *cond) {
    if (cond) {
        WakeAllConditionVariable(cond);
        return 0;
    }
    else {
        return 1;
    }
}

/*
Function:   timespec_to_ms
Desc:       Convert timespec structure to milliseconds
Args:       abstime: A pointer to a timespec structure
Return:     Converted time interval. If abstime is NULL, INFINITE will be returned
*/
DWORD timespec_to_ms(const struct timespec *abstime) {
    if (!abstime) {
        return INFINITE;
    }

    DWORD t = ((abstime->tv_sec - time(NULL)) * 1000) + (abstime->tv_nsec / 1000000);
    if (t < 0) {
        return 1;
    }
    else {
        return t;
    }
}

#endif  // _WIN32