/*
 * cg_opencv_if.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: luc.martel
 */

#include <iostream>
#include <string>
#include "cg_opencv_if.hpp"


void cg_attach(cg_image<unsigned char> *dst, cv::Mat *src, unsigned int plane_id) {

   // cg_attach only works with cv::Mat with one plane of type 8u
   if (src->type() != CV_8UC1) {
      std::cout << "Invalid CV::Mat type in cg_attach" << std::endl;
      return;
   }

   dst->get_plane(plane_id).attach(src->rows * src->cols, src->data);
}

void cg_copy(cv::Mat *dst, cg_image<unsigned char> *src, unsigned int plane_id) {

   unsigned char *src_p = src->get_plane(plane_id).get_data();
   unsigned char *dst_p = dst->data;

   unsigned int width  = src->get_width();
   unsigned int height = src->get_height();

   if (width != (unsigned int)dst->cols) {
      std::cout << "width src != dst in cg_copy" << std::endl;
      return;
   }
   if (height != (unsigned int)dst->rows) {
      std::cout << "height src != dst in cg_copy" << std::endl;
      return;
   }

   //std::cout << "Copying " << width*height << " bytes." << std::endl;
   memcpy(dst_p, src_p, width*height);
}



