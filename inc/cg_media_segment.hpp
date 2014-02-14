/*
 * cg_media_segment.hpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_SEGMENT_HPP_
#define CG_MEDIA_SEGMENT_HPP_

#include "cg_buffer.hpp"

class cg_media_segment {
public:
   cg_media_segment();
   cg_media_segment(unsigned int size);

   ~cg_media_segment();

   cg_media_segment(const cg_media_segment &x);
   cg_media_segment &operator=(const cg_media_segment &x);

   unsigned int get_duration()
   {
      return mDuration;
   }

private:
   unsigned int             mDuration; // Dutarion in msec
   cg_buffer<unsigned char> mBuffer;

   void set_duration(unsigned int duration)
   {
      mDuration = duration;
   }
};


#endif /* CG_MEDIA_SEGMENT_HPP_ */
