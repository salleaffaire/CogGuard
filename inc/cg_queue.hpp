/*
 * cg_queue.hpp
 *
 *  Created on: Sep 27, 2013
 *      Author: luc.martel
 */

#ifndef CG_QUEUE_HPP_
#define CG_QUEUE_HPP_

#include <deque>
#include <vector>
#include <semaphore.h>
#include <pthread.h>

#define CG_DEFAULT_MAX_QUEUABLE 16

template <class T>
class cg_sync_static_queue {
public:
   cg_sync_static_queue() {
      // Create the synchronization objects
      sem_init(&mSyncCountEmpty, 0, CG_DEFAULT_MAX_QUEUABLE);
      sem_init(&mSyncCountFull, 0, 0);
      pthread_mutex_init(&mProtectEmptyList, 0);
      pthread_mutex_init(&mProtectFullList, 0);

      // Create the elements
      mElements.resize(CG_DEFAULT_MAX_QUEUABLE);

      // All elements go in the empty list to start
      typename std::vector<T>::iterator it = mElements.begin();
      for (;it != mElements.end();++it) {
         mFIFOEmpty.push_back(&(*it));
      }
   }

   cg_sync_static_queue(unsigned int max_queuable) {
      // Create the synchronization objects
      sem_init(&mSyncCountEmpty, 0, max_queuable);
      sem_init(&mSyncCountFull, 0, 0);
      pthread_mutex_init(&mProtectEmptyList, 0);
      pthread_mutex_init(&mProtectFullList, 0);

      // Create the elements
      mElements.resize(max_queuable);
   }

   ~cg_sync_static_queue() {
      // Empty the queue

      // Destroy semaphore and mutex
   }

   void put_empty(T *x) {

      // Protect the empty List
      pthread_mutex_lock(&mProtectEmptyList);
      mFIFOEmpty.push_back(x);
      pthread_mutex_unlock(&mProtectEmptyList);

      // Took a full - hey that means that there is now an empty available
      sem_post(&mSyncCountEmpty);
   }

   void put_full(T *x) {

      // Protect the full List
      pthread_mutex_lock(&mProtectFullList);
      mFIFOFull.push_back(x);
      pthread_mutex_unlock(&mProtectFullList);

      // Took a full - hey that means that there is now an empty available
      sem_post(&mSyncCountFull);
   }

   T *get_empty() {
      T *rval = 0;

      // Wait for an empty slot
      sem_wait(&mSyncCountEmpty);

      // Protect the empty List
      pthread_mutex_lock(&mProtectEmptyList);
      rval = mFIFOEmpty.front();
      mFIFOEmpty.pop_front();
      pthread_mutex_unlock(&mProtectEmptyList);

      return rval;
   }

   T *get_full() {
      T *rval = 0;

      // Wait for a full slot
      sem_wait(&mSyncCountFull);

      // Protect the full List
      pthread_mutex_lock(&mProtectFullList);
      rval = mFIFOFull.front();
      mFIFOFull.pop_front();
      pthread_mutex_unlock(&mProtectFullList);

      return rval;
   }

private:

   sem_t            mSyncCountEmpty;
   sem_t            mSyncCountFull;
   pthread_mutex_t  mProtectEmptyList;
   pthread_mutex_t  mProtectFullList;

   std::vector<T>   mElements;

   std::deque<T *>  mFIFOEmpty;
   std::deque<T *>  mFIFOFull;

};


#endif /* CG_QUEUE_HPP_ */
