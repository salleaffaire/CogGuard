/*
 * cg_process_bg.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: luc.martel
 */

#ifndef CG_PROCESS_BG_HPP_
#define CG_PROCESS_BG_HPP_

#include <string>
#include "cg_algorithm.hpp"

class cg_alg_background_estimation : public cg_algorithm<unsigned char> {
public:
   cg_alg_background_estimation() : mWidth(CG_DEFAULT_WIDTH), mHeight(CG_DEFAULT_HEIGHT), mBackGround(cg_create_raw_8u(CG_DEFAULT_WIDTH, CG_DEFAULT_HEIGHT)) {
      init();
   }
   cg_alg_background_estimation(unsigned int width, unsigned height) : mWidth(width), mHeight(height), mBackGround(cg_create_raw_8u(width, height)) {
      init();
   }
   ~cg_alg_background_estimation() {
      delete mBackGround;
   }

   cg_image<unsigned char> *get_bg() {
      return mBackGround;
   }

   void set_alpha(unsigned char alpha) {
      mAlpha = alpha;
   }

private:
   unsigned int  mWidth;
   unsigned int  mHeight;
   unsigned char mAlpha; // 0 - > 0 : 255 -> 1

   cg_image<unsigned char> *mBackGround;

   void init() {
      cg_zero(mBackGround->get_plane(0));
   }

   void process(std::vector<cg_image<unsigned char> *> *input) {

      unsigned char *data_in = (*input)[0]->get_plane(0).get_data();
      unsigned char *data_out = mBackGround->get_plane(0).get_data();
      unsigned int size = mBackGround->get_plane(0).get_size();

      unsigned int i;
      for (i = 0;i<size;i++) {
         unsigned int temp = (mAlpha)**data_in + (255-mAlpha)**data_out;
         *data_out = (unsigned char)(temp >> 8); // Possible loss of data
         data_in++;
         data_out++;
      }
   }

};


#endif /* CG_PROCESS_BG_HPP_ */
