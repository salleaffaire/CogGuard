/*
 * cg_algorithms.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: luc.martel
 */

#ifndef CG_ALGORITHMS_HPP_
#define CG_ALGORITHMS_HPP_

#include <list>
#include <functional>

// POSIX Thread and Semaphores
#include "cg_porting_layer.hpp"

#include "cg_image.hpp"
#include "cg_error.hpp"

#define CG_DEFAULT_WIDTH  640
#define CG_DEFAULT_HEIGHT 480


// The algorithm template provides a consistent execution model
// An algorithm object can queue pointers to object to process - of type Y
// The algorithm object does not do any memory management

template <class Y>
class cg_future_algorithm {
public:

   cg_future_algorithm() {

      mProtectList.init();
      mQueuedInputSemaphore.init(0);
      mQueuedOutputSemaphore.init(0);

      mProcessignThread.init(processing_function, this);

      mIsRunning    = true;
      mIsProcessing = true;
   }

   virtual ~cg_future_algorithm() {

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

   static unsigned int processing_function(void *x) {
      cg_future_algorithm<Y> *rval = (cg_future_algorithm<Y> *)x;

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

template<class T, class U>
unsigned int cg_test_image_sizes(cg_buffer2D<T> &input, cg_buffer2D<U> &output) {
   unsigned int rval = CG_OK;

   if (input.get_width() != output.get_width()) {
      rval = CG_ERR_IN_OUT_WIDTH_MISMATCH;
   }
   else if (input.get_height() != output.get_height()) {
      rval = CG_ERR_IN_OUT_HEIGHT_MISMATCH;
   }

   return rval;
}

template<class T, class U>
unsigned int cg_test_image_sizes(cg_buffer2D<T> &input1, cg_buffer2D<T> &input2, cg_buffer2D<U> &output) {
   unsigned int rval = CG_OK;

   if (input1.get_width() != input2.get_width()) {
      rval = CG_ERR_INPUT_WIDTH_MISMATCH;
   }
   else if (input1.get_height() != input2.get_height()) {
      rval = CG_ERR_INPUT_HEIGHT_MISMATCH;
   }
   else if (input1.get_width() != output.get_width()) {
      rval = CG_ERR_IN_OUT_WIDTH_MISMATCH;
   }
   else if (input1.get_height() != output.get_height()) {
      rval = CG_ERR_IN_OUT_HEIGHT_MISMATCH;
   }

   return rval;
}


template <class T>
unsigned int cg_zero(cg_buffer2D<T> &input) {
   T *data = input.get_data();
   unsigned int size = input.get_size();
   memset(data, 0, size*sizeof(T));
   return CG_OK;
}

template <class T, class U>
unsigned int cg_pixel_process(cg_buffer2D<T> &input1, cg_buffer2D<T> &input2, cg_buffer2D<U> &output,
                              U (*pp_func)(T, T)) {
   unsigned int rval = CG_OK;

   cg_test_image_sizes(input1, input2, output);

   if (rval == CG_OK) {

      const unsigned int width   = input1.get_width();
      const unsigned int height  = input1.get_height();
      const unsigned int in_stride1 = input1.get_stride();
      const unsigned int in_stride2 = input2.get_stride();
      const unsigned int out_stride = output.get_stride();

      T *in_data1 = input1.get_data();
      T *in_data2 = input2.get_data();
      U *out_data = output.get_data();

      unsigned int i = 0;
      unsigned int j = 0;
      for (j=0;j<height;++j) {
         for (i=0;i<width;++i) {
            out_data[i] = pp_func(in_data1[i], in_data2[i]);
         }
         in_data1 += in_stride1;
         in_data2 += in_stride2;
         out_data += out_stride;
      }
   }
   return rval;
}


template <class T, class U>
unsigned int cg_pixel_process_image(cg_image<T> &input1, cg_image<T> &input2, cg_image<U> &output,
                                    U (*pp_func)(T, T)) {
   unsigned int rval = CG_OK;

   // Test the whether the number of planes
   // -----------------------------------------------------
   unsigned int in_num_planes1 = input1.get_number_of_planes();
   unsigned int in_num_planes2 = input2.get_number_of_planes();
   unsigned int out_num_planes = output.get_number_of_planes();

   unsigned int processing_planes = (in_num_planes1 < in_num_planes2) ? in_num_planes1 : in_num_planes2;

   if (out_num_planes < processing_planes) {
      rval |= CG_ERR_NUM_PLANES_MISMATCH;
   }
   // -----------------------------------------------------

   if ((rval == CG_OK) && (processing_planes != 0)) {
      unsigned int i;
      for (i=0;i<processing_planes;i++) {
         rval = cg_pixel_process(input1.get_plane(i),
                                 input2.get_plane(i),
                                 output.get_plane(i),
                                 pp_func);
      }
   }

   return rval;
}

template <class T, class U>
unsigned int cg_o1_filter_process(cg_buffer2D<T> &input, cg_buffer2D<U> &output,
                                  U (*pp_func)(T *[])) {
   unsigned int rval = CG_OK;

   cg_test_image_sizes(input, output);

   if (rval == CG_OK) {

      const unsigned int width      = input.get_width();
      const unsigned int height     = input.get_height();
      const unsigned int in_stride  = input.get_stride();
      const unsigned int out_stride = output.get_stride();

      T *in_first_line  = input.get_data();
      U *out_first_line = output.get_data();

      T *in_data[3];
      in_data[0] = in_first_line;
      in_data[1] = in_data[0] + in_stride;
      in_data[2] = in_data[1] + in_stride;

      U *out_data = out_first_line + out_stride;

      unsigned int i = 0;
      unsigned int j = 0;
      for (j=1;j<height-1;++j) {
         for (i=1;i<width-1;++i) {
            *out_data = pp_func(in_data);

            // Update X
            in_data[0] += 1;
            in_data[1] += 1;
            in_data[2] += 1;
            out_data   += 1;
         }

         // Update Y
         in_first_line += in_stride;
         out_first_line += out_stride;

         in_data[0] = in_first_line;
         in_data[1] = in_data[0] + in_stride;
         in_data[2] = in_data[1] + in_stride;
         out_data = out_first_line;
      }
   }
   return rval;
}

template <class T, class U>
unsigned int cg_o1_filter_process_image(cg_image<T> &input, cg_image<U> &output,
                                        U (*pp_func)(T *[])) {
   unsigned int rval = CG_OK;

   // Test the whether the number of planes
   // -----------------------------------------------------
   unsigned int in_num_planes = input.get_number_of_planes();
   unsigned int out_num_planes = output.get_number_of_planes();

   unsigned int processing_planes = in_num_planes;

   if (out_num_planes < processing_planes) {
      rval |= CG_ERR_NUM_PLANES_MISMATCH;
   }
   // -----------------------------------------------------

   if ((rval == CG_OK) && (processing_planes != 0)) {
      unsigned int i;
      for (i=0;i<processing_planes;i++) {
         rval = cg_o1_filter_process(input.get_plane(i),
                                     output.get_plane(i),
                                     pp_func);
      }
   }

   return rval;
}


#endif /* CG_ALGORITHMS_HPP_ */
