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
class cg_image;

template <class T>
class cg_buffer2D : public cg_buffer<T> {
public:

   // Let's implement a slow one first
   class iterator {
   public:
      iterator(cg_buffer2D<T> *x) : mX(0), mY(0), mBuffer(x) {}
      ~iterator() {}

      T &operator*() {
         return (*mBuffer)(mX, mY);
      }
      void operator++() {
         bool cond = (mX == mBuffer->get_width());
         mX = (cond) ? 0 : mX + 1;
         mY = (cond) ? mY + 1 : mY;
      }

   private:
      cg_buffer2D<T> *mBuffer;
      unsigned int    mX;
      unsigned int    mY;

      // Never use this one
      iterator() {}
   };

   cg_buffer2D() : cg_buffer<T>() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D dc" << std::endl;
      #endif

      mWidth = 0;
      mHeight = 0;
      mStride = 0;
   }

   cg_buffer2D(unsigned int width, unsigned int height, unsigned int stride) : cg_buffer<T>(stride*height) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D c" << std::endl;
      #endif
   }

   cg_buffer2D(const cg_buffer2D<T> &x) : cg_buffer<T>(x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D cc" << std::endl;
      #endif

      mWidth = x.mWidth;
      mHeight = x.mHeight;
      mStride = x.mStride;
   }

   ~cg_buffer2D() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D d" << std::endl;
      #endif
   }

   // Operators =
   cg_buffer2D<T> &operator=(const cg_buffer2D<T> &x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D = const " << std::endl;
      #endif

      cg_buffer<T>::operator=(x);
      mWidth = x.mWidth;
      mHeight = x.mHeight;
      mStride = x.mStride;
      return *this;
   }

   // Operator = (used by STL vectors)
   cg_buffer2D<T> &operator=(cg_buffer2D<T> &x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer2D =" << std::endl;
      #endif

      return operator=(const_cast<const cg_buffer2D<T> &>(x));
   }

   T operator()(unsigned int x, unsigned int y) {
      return this->mData[y*mStride+x];
   }

private:
   unsigned int mWidth;
   unsigned int mHeight;
   unsigned int mStride;

   void allocate(unsigned int width, unsigned int height, unsigned stride) {
      mWidth = width;
      mHeight = height;
      mStride = stride;
      cg_buffer<T>::allocate(stride*height);
   }

   friend class cg_image<T>;
};

template <class T>
class cg_image {
public:

   cg_image() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_image dc" << std::endl;
      #endif

      mWidth  = 0;
      mHeight = 0;
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
