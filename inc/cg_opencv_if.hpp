/*
 * cg_opencv_if.hpp
 *
 *  Created on: Sep 17, 2013
 *      Author: luc.martel
 */

#ifndef CG_OPENCV_IF_HPP_
#define CG_OPENCV_IF_HPP_

#include <cv.h>
#include "cg_image.hpp"

void cg_attach(cg_image<unsigned char> *dst, cv::Mat *src, unsigned int plane_id);

void cg_copy(cv::Mat *dst, cg_image<unsigned char> *src, unsigned int plane_id);



#endif /* CG_OPENCV_IF_HPP_ */
