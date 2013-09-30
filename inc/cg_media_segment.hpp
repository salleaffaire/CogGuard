/*
 * cg_media_segment.hpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_SEGMENT_HPP_
#define CG_MEDIA_SEGMENT_HPP_

#include "cg_buffer.hpp"

class cg_media_segment : cg_buffer<unsigned char> {
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

   void set_duration(unsigned int duration)
   {
      mDuration = duration;
   }

private:
   unsigned int mDuration; // Dutarion in msec

};


#endif /* CG_MEDIA_SEGMENT_HPP_ */
