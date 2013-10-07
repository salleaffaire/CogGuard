/*
 * cg_algorithms.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: luc.martel
 */

#ifndef CG_ALGORITHMS_HPP_
#define CG_ALGORITHMS_HPP_

#include <list>

// POSIX Thread and Semaphores
#include "cg_porting_layer.hpp"

#include "cg_image.hpp"

#define CG_DEFAULT_WIDTH  640
#define CG_DEFAULT_HEIGHT 480


// The algorithm template provides a consistent execution model
// An algorithm object can queue pointers to object to process - of type Y
// The algorithm object does not do any memory management

template <class Y>
class cg_algorithm {
public:

   cg_algorithm() {

      LPTHREAD_START_ROUTINE pf;
      pf = processing_function;

      mProtectList.init();
      mQueuedInputSemaphore.init(0);
      mQueuedOutputSemaphore.init(0);

      mProcessignThread.init(pf, this);

      mIsRunning    = true;
      mIsProcessing = true;
   }

   virtual ~cg_algorithm() {

   }

   bool run(Y *input) {
      bool rval = true;

      // Only post if the algorithm is running
      if (mIsRunning) {
         // Add to the processing list
         mProtectList.lock();
         mQueuedInputs.push_back(input);
         mProtectList.unlock();

         mQueuedInputSemaphore.post();
      }
      else {
         rval = false;
      }
      return rval;
   }

   void wait() {
      // Wait until there is a result ready
      mQueuedOutputSemaphore.wait();
   }

   // Clean up and exit
   void stop() {
      mIsRunning = false;
      while (mQueuedInputs.size() != 0) {
         mQueuedInputSemaphore.post();
      }
   }

   static long unsigned int WINAPI processing_function(void *x) {
      cg_algorithm<Y> *rval = (cg_algorithm<Y> *)x;

      while (rval->mIsRunning) {
         // Wait for an input
         rval->mQueuedInputSemaphore.wait();

         // Take from the processing list
         rval->mProtectList.lock();
         Y *temp =  rval->mQueuedInputs.front();
         rval->mQueuedInputs.pop_front();
         rval->mProtectList.unlock();

         // Call the main processing routine
         if (rval->mIsRunning) rval->process(temp);

         // Something is ready !!
         rval->mQueuedOutputSemaphore.post();
      }

      rval->mIsProcessing = false;

      return 0;
   }

protected:
   // Algorithm ID
  unsigned int      mID;

private:
   virtual void process(Y *input) = 0;

   // Assynchronous processing classes
   cg_thread        mProcessignThread;

   // Protection
   cg_semaphore     mQueuedInputSemaphore;
   cg_semaphore     mQueuedOutputSemaphore;
   cg_mutex         mProtectList;

   // First implementation will use only one
   //  1- pointer to a vector of input images
   std::list<Y *>   mQueuedInputs;



   bool             mIsRunning;
   bool             mIsProcessing;
};


// Simple algorithms on 2D buffers

template <class T>
unsigned int cg_zero(cg_buffer2D<T> &input) {
   T *data = input.get_data();
   unsigned int size = input.get_size();
   memset(data, 0, size*sizeof(T));
   return 0;
}

#endif /* CG_ALGORITHMS_HPP_ */
