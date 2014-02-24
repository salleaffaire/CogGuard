/*
 * cg_image.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: luc.martel
 */

#include "cg_image.hpp"


std::ostream &operator<<(std::ostream &s, cg_wpix x)
{
  s << "X :" << x.X;
  s << std::endl;
  s << "Y :" << x.Y;
  s << std::endl;

  return s;
}

std::istream &operator>>(std::istream &s, cg_wpix x)
{
  s >> x.X;
  s >> x.Y;
  s >> x.W;

  return s;
}

int compare(const cg_wpix &X, const cg_wpix &Y)
{
  if (X >= Y) {
    return 0;
  }
  else {
    return 1;
  }
}


cg_image<unsigned char> *cg_create_raw_8u(unsigned int width, unsigned int height) {
   cg_image<unsigned char> *rval = new cg_image<unsigned char>(width, height, 1);

   unsigned int plane_0_width  = width;
   unsigned int plane_0_height = height;
   unsigned int plane_0_stride = (width & 3) ? (((width >> 2) + 1) << 2) : (width);

   //std::cout << "Width  = " << plane_0_width << std::endl;
   //std::cout << "Height = " << plane_0_height << std::endl;
   //std::cout << "Stride = " << plane_0_stride << std::endl;

   rval->allocate(0, plane_0_width, plane_0_height, plane_0_stride); // Plane 0

   return rval;
}

cg_image<unsigned int> *cg_create_raw_32u(unsigned int width, unsigned int height) {
   cg_image<unsigned int> *rval = new cg_image<unsigned int>(width, height, 1);

   unsigned int plane_0_width  = width;
   unsigned int plane_0_height = height;
   unsigned int plane_0_stride = (width & 3) ? (((width >> 2) + 1) << 2) : (width);

   //std::cout << "Width  = " << plane_0_width << std::endl;
   //std::cout << "Height = " << plane_0_height << std::endl;
   //std::cout << "Stride = " << plane_0_stride << std::endl;

   rval->allocate(0, plane_0_width, plane_0_height, plane_0_stride); // Plane 0

   return rval;
}

cg_image<unsigned char> *cg_create_444_8u(unsigned int width, unsigned int height) {
   cg_image<unsigned char> *rval = new cg_image<unsigned char>(width, height, 3);

   unsigned int plane_012_width  = width;
   unsigned int plane_012_height = height;
   unsigned int plane_012_stride = (width & 3) ? (((width >> 2) + 1) << 2) : (width);

   rval->allocate(0, plane_012_width, plane_012_height, plane_012_stride); // Plane 0
   rval->allocate(1, plane_012_width, plane_012_height, plane_012_stride); // Plane 1
   rval->allocate(2, plane_012_width, plane_012_height, plane_012_stride); // Plane 2

   return rval;
}
