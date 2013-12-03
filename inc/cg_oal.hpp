/*
 * cg_oal.hpp
 *
 *  Created on: Nov 1, 2013
 *      Author: luc.martel
 */

#ifndef CG_OAL_HPP_
#define CG_OAL_HPP_

// ---------------------------------------------------------------
// OS Abstraction Layer (That's what OAL stands for)
// ---------------------------------------------------------------

#include "cg_porting_layer.hpp"

class cg_task {
public:
   cg_task() {
      mRunning = false;
      mExit    = false;
      mProtectState.init();

      mProcess.init(process, (void *)this);
      mWait.init(0);
   }
   virtual ~cg_task() {
   }

   void start() {
      set_running(true);
      set_paused(true);
   }
   void pause() {
      set_paused(false);
   }
   void stop() {
      exit();
   }
   void wait() {
      mWait.wait();
   }

   static unsigned int process(void *p) {
      cg_task *pObject = (cg_task *)p;

      while ((pObject->mRunning) && (!pObject->mExit)) {
         if (!pObject->mPaused) {
            bool runResult = pObject->run();
            pObject->set_running(runResult);
         }
      }

      pObject->mWait.post();

      return 0;
   }

   virtual bool run() = 0;


   void set_running(bool r) {
      mProtectState.lock();
      mRunning = r;
      mProtectState.unlock();
   }
   void set_paused(bool r) {
      mProtectState.lock();
      mRunning = r;
      mProtectState.unlock();
   }

private:

   cg_thread    mProcess;
   cg_mutex     mProtectState;
   cg_semaphore mWait;

   bool         mRunning;
   bool         mPaused;
   bool         mExit;

   void exit() {
      mProtectState.lock();
      mExit = true;
      mProtectState.unlock();
   }
};


#endif /* CG_OAL_HPP_ */
