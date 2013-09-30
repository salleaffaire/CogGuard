/*
 * cg_streaming_client.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#include "cg_streaming_client.hpp"

cg_streaming_client::cg_streaming_client()
{

}

cg_streaming_client::~cg_streaming_client()
{

}

cg_streaming_client::cg_streaming_client(const cg_streaming_client &x)
{

}

cg_streaming_client &
cg_streaming_client::operator=(const cg_streaming_client &x)
{

   return *this;
}

// Interface
// ------------------------------------------------------------
// Pop the next available segment
cg_media_segment *
cg_streaming_client::get_segment()
{
   cg_media_segment *rval = (cg_media_segment *)0;

   while (need_filling()) {
      fetch_next_segment();
   }

   return rval;
}
// ------------------------------------------------------------


