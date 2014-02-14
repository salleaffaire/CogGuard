/*
 * cg_media_stream_producer.hpp
 *
 *  Created on: Dec 6, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_STREAM_PRODUCER_HPP_
#define CG_MEDIA_STREAM_PRODUCER_HPP_

#include "cg_media_stream.hpp"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// STREAM Producer classes
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

class cg_media_stream_producer {
public:
   cg_media_stream_producer() : mOutput((cg_media_stream *)0) {}
   cg_media_stream_producer(unsigned int numberofinputs) : mNumberOfInputs(numberofinputs),
                                                           mOutput((cg_media_stream *)0){}

   virtual ~cg_media_stream_producer() {}

   void set_input_size(int size) {
      mInputs.resize(size);
   }

   int get_input_size() {
      return mInputs.size();
   }

   void set_input_stream(cg_media_stream *s, unsigned int index) {
      unsigned int size = get_input_size();
      if (index+1 > size) {
         set_input_size(index+1);
      }
      mInputs[index] = s;
   }

   void set_output_stream(cg_media_stream *s) {
      mOutput = s;
   }

   unsigned int run() {
      unsigned int rval = 0;

      // Check if we have the right number of inputs
      if (mInputs.size() != mNumberOfInputs) {
         rval = 1;
      }
      else {
         // And if they are initialized
         for (unsigned int i=0;i<mNumberOfInputs;i++) {
            if (mInputs[i] == (cg_media_stream *) 0) {
               rval = 1;
            }
         }
      }

      // Check if Output stream is set
      if (mOutput == 0) {
         rval = 2;
      }

      // Check if we have the right stream direction for all inputs
      if (rval == 0) {

         for (unsigned int i=0;i<mNumberOfInputs;i++) {
            if (mInputs[i]->get_dir() != cg_media_stream::CG_IN) {
               rval = 4;
            }
         }
      }

      // Check if we have the right stream direction for output
      if (rval == 0) {
         if (mOutput->get_dir() != cg_media_stream::CG_OUT) {
            rval = 8;
         }
      }

      // Processing loop
      if (!rval) {
         // Resting all streams
         for (unsigned int i=0;i<mNumberOfInputs;i++) {
            mInputs[0]->reset();
         }
         mOutput->reset();

         run_loop();
      }
      return rval;
   }

   virtual unsigned int run_loop() = 0;

protected:
   unsigned int                    mNumberOfInputs;
   std::vector<cg_media_stream *>  mInputs;
   cg_media_stream                *mOutput;

};



#endif /* CG_MEDIA_STREAM_PRODUCER_HPP_ */
