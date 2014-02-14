/*
 * cg_media_stream_producer_identity.hpp
 *
 *  Created on: Dec 6, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_STREAM_PRODUCER_IDENTITY_HPP_
#define CG_MEDIA_STREAM_PRODUCER_IDENTITY_HPP_

#include "cg_media_stream_producer.hpp"

// ---------------------------------------------------------------------
// Identity - turns a stream into the same stream
// ---------------------------------------------------------------------
// Dummy producer
class cg_media_stream_producer_identity : public cg_media_stream_producer {
public:
   cg_media_stream_producer_identity() : cg_media_stream_producer(1) {}
   virtual ~cg_media_stream_producer_identity() {}

   virtual unsigned int run_loop() {
         unsigned int current_word;
         unsigned int bits_read;
         // Read word will return 0 when 0 bits are returned
         while ((bits_read = mInputs[0]->read_word(current_word)) != 0) {
            std::cout << "Read " << bits_read << " bits." << std::endl;
            mOutput->write_word(current_word);
         }
      return 0;
   }

};


#endif /* CG_MEDIA_STREAM_PRODUCER_IDENTITY_HPP_ */
