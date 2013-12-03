/*
 * cg_porting_layer.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: luc.martel
 */


#include <iostream>
#include "cg_porting_layer.hpp"

#define CG_MAX_SEM_COUNT 1024

cg_thread::cg_thread() {
#if (CG_PLATFORM == CG_WINDOWS)

#else
   pthread_create(&mThread, NULL, f, x);
#endif

}

cg_thread::~cg_thread() {
#if (CG_PLATFORM == CG_WINDOWS)
   CloseHandle(mThread);
#else

#endif
}

void
cg_thread::init(pf f, void *x) {
#if (CG_PLATFORM == CG_WINDOWS)
   mThread = CreateThread(NULL,        // default security attributes
                          0,           // default stack size
                          (LPTHREAD_START_ROUTINE) f,
                          x,           // no thread function arguments
                          0,           // default creation flags
                          &mThreadID); // receive thread identifier
#else
   pthread_create(&mThread, NULL, f, x);
#endif

}


cg_semaphore::cg_semaphore() {
#if (CG_PLATFORM == CG_WINDOWS)
#else
#endif
}

cg_semaphore::~cg_semaphore() {
#if (CG_PLATFORM == CG_WINDOWS)
   CloseHandle(mSemaphore);
#else

#endif
}

void
cg_semaphore::init(unsigned int init_val) {
#if (CG_PLATFORM == CG_WINDOWS)
   mSemaphore = CreateSemaphore(NULL,              // default security attributes
                                init_val,          // initial count
                                CG_MAX_SEM_COUNT,  // maximum count
                                NULL);             // unnamed semaphore

   if (mSemaphore == NULL) {
      std::cout << "Semaphore failed to be created ..." << std::cout;
   }
#else
   sem_init(&mSemaphore, 0, 0);
#endif
}


void
cg_semaphore::post() {
#if (CG_PLATFORM == CG_WINDOWS)
   ReleaseSemaphore(mSemaphore,  // handle to semaphore
                    1,           // increase count by one
                    NULL);
#else

#endif
}

void
cg_semaphore::wait() {
#if (CG_PLATFORM == CG_WINDOWS)

   WaitForSingleObject(mSemaphore,   // handle to semaphore
                       INFINITE);    // Wait forever

   //if (waitResult == WAIT_TIMEOUT) {
   //   std::cout << "Wait timedout" << waitResult << std::endl;
   //}
#else

#endif
}

void
cg_semaphore::timed_wait(unsigned int usec) {
#if (CG_PLATFORM == CG_WINDOWS)

   WaitForSingleObject(mSemaphore,    // handle to semaphore
                       usec/1000);    // millisecond time-out interval
#else

#endif
}

cg_mutex::cg_mutex() {
#if (CG_PLATFORM == CG_WINDOWS)
#else
#endif
}

cg_mutex::~cg_mutex() {
#if (CG_PLATFORM == CG_WINDOWS)
   CloseHandle(mMutex);
#else

#endif
}

void
cg_mutex::init() {
#if (CG_PLATFORM == CG_WINDOWS)
   mMutex = CreateMutex(NULL,              // default security attributes
                        FALSE,             // initially not owned
                        NULL);             // unnamed mutex
#else

#endif
}

void
cg_mutex::lock() {
#if (CG_PLATFORM == CG_WINDOWS)
   WaitForSingleObject(mMutex,     // handle to mutex
                       INFINITE);  // no time-out interval
#else

#endif
}

void
cg_mutex::unlock() {
#if (CG_PLATFORM == CG_WINDOWS)
   ReleaseMutex(mMutex);
#else

#endif
}


void cg_sleep(unsigned int x) {
#if (CG_PLATFORM == CG_WINDOWS)
   Sleep(x);

#else
#endif
}

