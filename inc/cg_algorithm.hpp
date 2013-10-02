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
#include <pthread.h>
#include <ctime>
#include <semaphore.h>

#include "cg_image.hpp"

#define CG_DEFAULT_WIDTH  640
#define CG_DEFAULT_HEIGHT 480


// The algorithm template provides a consistent execution model
// An algorithm object can queue pointers to object to process - of type Y
// The algorithm object does not do any memory management

template <class Y>
class cg_algorithm {
public:
   typedef void *(*pf_t)(void *);

   cg_algorithm() {
      pf_t pf;

      pf = processing_function;

      pthread_mutex_init(&mProtectList, 0);
      sem_init(&mQueuedInputSemaphore, 0, 0);
      sem_init(&mQueuedOutputSemaphore, 0, 0);

      pthread_create(&mProcessignThread, NULL, pf, (void *)this);

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
         pthread_mutex_lock(&mProtectList);
         mQueuedInputs.push_back(input);
         pthread_mutex_unlock(&mProtectList);

         sem_post(&mQueuedInputSemaphore);
      }
      else {
         rval = false;
      }
      return rval;
   }

   void wait() {
      // Wait until there is a result ready
      sem_wait(&mQueuedOutputSemaphore);
   }

   void stop() {
      mIsRunning = false;
      while (mQueuedInputs.size() != 0) {
         sem_post(&mQueuedInputSemaphore);
      }
   }

   static void *processing_function(void *x) {
      cg_algorithm<Y> *rval = (cg_algorithm<Y> *)x;

      while (rval->mIsRunning) {
         // Wait for an input
         sem_wait(&(rval->mQueuedInputSemaphore));

         // Take from the processing list
         pthread_mutex_lock(&(rval->mProtectList));
         Y *temp =  rval->mQueuedInputs.front();
         rval->mQueuedInputs.pop_front();
         pthread_mutex_unlock(&(rval->mProtectList));

         // Call the main processing routine
         if (rval->mIsRunning) rval->process(temp);

         // Something is ready !!
         sem_post(&(rval->mQueuedOutputSemaphore));
      }

      rval->mIsProcessing = false;

      return (void *)x;
   }

private:
   virtual void process(Y *input) = 0;

   // Assynchronous processing classes
   pthread_t        mProcessignThread;

   // Protection
   sem_t            mQueuedInputSemaphore;
   sem_t            mQueuedOutputSemaphore;
   pthread_mutex_t  mProtectList;

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
