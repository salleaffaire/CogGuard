/*
 * cg_media_segment_mp2ts.hpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_SEGMENT_MP2TS_HPP_
#define CG_MEDIA_SEGMENT_MP2TS_HPP_

#include "cg_media_segment.hpp"

class cg_media_segment_mp2ts : public cg_media_segment {
public:
   cg_media_segment_mp2ts();
   cg_media_segment_mp2ts(unsigned int size);
   ~cg_media_segment_mp2ts();

   cg_media_segment_mp2ts(const cg_media_segment_mp2ts &x);
   cg_media_segment_mp2ts &operator=(const cg_media_segment_mp2ts &x);
private:

};


#endif /* CG_MEDIA_SEGMENT_MP2TS_HPP_ */
