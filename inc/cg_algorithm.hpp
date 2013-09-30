/*
 * cg_algorithms.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: luc.martel
 */

#ifndef CG_ALGORITHMS_HPP_
#define CG_ALGORITHMS_HPP_

#include <string>
#include <vector>
#include <memory>
#include <pthread.h>

#include "cg_image.hpp"


#define CG_DEFAULT_WIDTH  640
#define CG_DEFAULT_HEIGHT 480

// Forward declaration
void *processingfunction(void *x);

template <class T>
class cg_algorithm {
public:
   cg_algorithm() {
      pthread_create(&mProcessignThread, NULL, processingfunction, (void *)this);
   }

   virtual ~cg_algorithm() {}

   bool run(std::vector<cg_image<T> *> *input) {
      process(input);
      return true;
   }

private:
   virtual void process(std::vector<cg_image<T> *> *input) = 0;

   // Assynchronous processing classes
   pthread_t                    mProcessignThread;
   std::vector<cg_image<T> *>  *mQueuedInput;
};


// Simple algorithms on 2D buffers

template <class T>
unsigned int cg_zero(cg_buffer2D<T> &input) {
   T *data = input.get_data();
   unsigned int size = input.get_size();
   memset(data, 0, size*sizeof(T));
   return 0;
}


void *processingfunction(void *x) {
   void *rval = x;

   return x;
}


#endif /* CG_ALGORITHMS_HPP_ */
