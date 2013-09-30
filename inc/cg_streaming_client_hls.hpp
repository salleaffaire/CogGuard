/*
 * cg_streaming_client_hls.hpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#ifndef CG_STREAMING_CLIENT_HLS_HPP_
#define CG_STREAMING_CLIENT_HLS_HPP_

#include <string>
#include <list>
#include "cg_streaming_client.hpp"

class cg_streaming_client_hls : public cg_streaming_client
{
public:
   cg_streaming_client_hls();
   virtual ~cg_streaming_client_hls();

   cg_streaming_client_hls(const cg_streaming_client_hls &x);
   cg_streaming_client_hls &operator=(const cg_streaming_client_hls &x);

protected:

   // Implementation specific
   virtual bool need_filling();
   virtual void fetch_next_segment();
   virtual cg_media_segment *pop_segment();
   virtual int run();

private:

   std::string mManifestName;
   std::list<cg_media_segment *> mSegmentList;
   unsigned int mSegmentsDuration;
};


#endif /* CG_STREAMING_CLIENT_HLS_HPP_ */
