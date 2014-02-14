/*
 * cg_image.hpp
 *
 *  Created on: Aug 2, 2013
 *      Author: luc.martel
 */

#ifndef CG_IMAGE_HPP_
#define CG_IMAGE_HPP_

#include "cg_log.hpp"
#include "cg_buffer.hpp"

#include <vector>


template <class T>
class cg_image {
public:

   cg_image() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image dc" << std::endl;
      #endif

      mWidth  = 0;
      mHeight = 0;

      mNumberOfPlanes = 0;
   }
   cg_image(unsigned int width, unsigned int height, unsigned int numberofplanes) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image c" << std::endl;
      #endif

      mWidth = width;
      mHeight = height;
      mNumberOfPlanes = numberofplanes;
      mPlanes.resize(numberofplanes);
   }

   cg_image(const cg_image<T> &x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image cc" << std::endl;
      #endif

      mWidth = x.mWidth;
      mHeight = x.mHeight;

      mNumberOfPlanes = x.mNumberOfPlanes;
      mPlanes = x.mPlanes;
   }

   cg_image<T> &operator=(const cg_image<T> &x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image =" << std::endl;
      #endif

      // Clear the planes
      mPlanes.clear();

      mWidth = x.mWidth;
      mHeight = x.mHeight;

      mNumberOfPlanes = x.mNumberOfPlanes;
      mPlanes = x.mPlanes;
      return *this;
   }

   ~cg_image() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image d" << std::endl;
      #endif
   }

   unsigned int get_width()  { return mWidth; }
   unsigned int get_height() { return mHeight; }
   unsigned int get_number_of_planes() { return mPlanes.size(); }
   cg_buffer2D<T> &get_plane(unsigned int plane_id) { return mPlanes[plane_id]; }
   unsigned int get_width(unsigned int plane_id)  { return mPlanes[plane_id].mWidth; }
   unsigned int get_height(unsigned int plane_id) { return mPlanes[plane_id].mHeight; }
   unsigned int get_stride(unsigned int plane_id) { return mPlanes[plane_id].mStride; }

   void allocate(unsigned int plane_id, unsigned int width, unsigned int height, unsigned int stride) {
      mPlanes[plane_id].allocate(width, height, stride);
   }

private:
   unsigned int mWidth;      // Width in pixels
   unsigned int mHeight;     // Height in pixels

   unsigned int                  mNumberOfPlanes;
   std::vector<cg_buffer2D<T> >  mPlanes;

};

cg_image<unsigned char> *cg_create_raw_8u(unsigned int width, unsigned int height);
cg_image<unsigned int> *cg_create_raw_32u(unsigned int width, unsigned int height);
cg_image<unsigned char> *cg_create_444_8u(unsigned int width, unsigned int height);

#endif /* CG_IMAGE_HPP_ */
