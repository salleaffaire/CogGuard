/*
 * cg_streaming_client_hls.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#include "cg_streaming_client_hls.hpp"

cg_streaming_client_hls::cg_streaming_client_hls()
{

}

cg_streaming_client_hls::~cg_streaming_client_hls()
{

}

cg_streaming_client_hls::cg_streaming_client_hls(const cg_streaming_client_hls &x) :
   cg_streaming_client(x)
{

}

cg_streaming_client_hls &
cg_streaming_client_hls::operator=(const cg_streaming_client_hls &x)
{
   return *this;
}


// Porting API
// -------------------------------------------------------------
bool
cg_streaming_client_hls::need_filling()
{
   bool rval = false;

   return rval;
}

void
cg_streaming_client_hls::fetch_next_segment()
{
   //cg_media_segment *rval = (cg_media_segment *)0;
}

cg_media_segment *
cg_streaming_client_hls::pop_segment()
{
   cg_media_segment *rval = (cg_media_segment *)0;

   return rval;
}

int
cg_streaming_client_hls::run()
{
   int rval = 0;

   return rval;
}

// -------------------------------------------------------------
