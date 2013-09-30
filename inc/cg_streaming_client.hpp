/*
 * cg_streaming_client.hpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#ifndef CG_STREAMING_CLIENT_HPP_
#define CG_STREAMING_CLIENT_HPP_

#include "cg_media_segment.hpp"

class cg_streaming_client {
public:
   cg_streaming_client();
   virtual ~cg_streaming_client();

   cg_streaming_client(const cg_streaming_client &x);
   cg_streaming_client &operator=(const cg_streaming_client &x);

   // Interface
   // ------------------------------------------------------------
   // Pop the next available segment
   cg_media_segment *get_segment();
   // ------------------------------------------------------------

protected:

   // Implementation specific
   virtual bool need_filling() = 0;
   virtual void fetch_next_segment() = 0;
   virtual cg_media_segment *pop_segment() = 0;
   virtual int run() = 0;
};


#endif /* CG_STREAMING_CLIENT_HPP_ */
