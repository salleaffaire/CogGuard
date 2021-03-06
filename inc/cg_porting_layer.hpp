/*
 * cg_porting_layer.hpp
 *
 *  Created on: Oct 2, 2013
 *      Author: luc.martel
 */

#ifndef CG_PORTING_LAYER_HPP_
#define CG_PORTING_LAYER_HPP_

#define CG_WINDOWS 1
#define CG_UNIX    2

#define CG_PLATFORM CG_WINDOWS

#if (CG_PLATFORM == CG_WINDOWS)
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

typedef unsigned int (*pf)(void *);
//typedef void *(*pf_t)(void *);

class cg_thread {
public:
   cg_thread();
   ~cg_thread();

   void init(pf f, void *x);

private:
#if (CG_PLATFORM == CG_WINDOWS)
   HANDLE mThread;
   DWORD  mThreadID;
#else
   pthread_t mThread;
#endif

};

class cg_mutex {
public:
   cg_mutex();
   ~cg_mutex();

   void init();
   void lock();
   void unlock();

private:
#if (CG_PLATFORM == CG_WINDOWS)
   HANDLE mMutex;
#else
#endif
};

class cg_semaphore {
public:
   cg_semaphore();
   ~cg_semaphore();

   void init(unsigned int init_val);
   void post();
   void wait();
   void timed_wait(unsigned int usec);

private:
#if (CG_PLATFORM == CG_WINDOWS)
   HANDLE mSemaphore;
#else
   sem_t mSemaphore;
#endif
};

void cg_sleep(unsigned int x);

#endif /* CG_PORTING_LAYER_HPP_ */
