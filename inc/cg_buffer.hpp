/*
 * cg_media_node.hpp
 *
 *  Created on: Jul 7, 2013
 *      Author: luc.martel
 */

#ifndef CG_BUFFER_HPP_
#define CG_BUFFER_HPP_

#include "cg_log.hpp"
#include <list>
#include <cstring>

template <class T>
class cg_buffer {
public:
   // Constructors
   cg_buffer() : mOwnData(true) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer dc" << std::endl;
      #endif

      mData   = (T *)0;
      mSize   = 0;
   }

   cg_buffer(unsigned int size) : mOwnData(true) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer c" << std::endl;
      #endif

      mData   = new T[size];
      mSize   = size;
   }

   cg_buffer(unsigned int size, T *data, bool owndata) : mOwnData(owndata) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer c" << std::endl;
      #endif

      if (owndata) {
         mData   = new T[size];
         mSize   = size;
         copy(data, mSize);
      }
      else {
         mData = data;
         mSize = size;
      }
   }

   cg_buffer(const cg_buffer &x) : mOwnData(true) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer cc" << std::endl;
      #endif

      mData   = new T[x.mSize];
      mSize   = x.mSize;

      // Copy the data
      copy(x.mData, mSize);
   }

   // Destructor
   ~cg_buffer() {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer d" << std::endl;
      #endif

      if (mData && mOwnData) delete [] mData;
      mSize = 0;
   }

   // Operators
   cg_buffer &operator=(const cg_buffer &x) {
      #if (CG_LOG_IMAGE_MEMORY == 1)
      std::cout << "cg_buffer =" << std::endl;
      #endif

      // Delete if there is some allocated data
      if (mData) delete [] mData;

      mOwnData = true;
      mData   = new T[x.mSize];
      mSize   = x.mSize;

      // Copy the data
      copy(x.mData, mSize);

      return *this;
   }

   // Buffer Interface
   // **************************************************
   bool is_allocated() {
      return mData != (T *)0;
   }

   void allocate(unsigned int size) {
      if (is_allocated()) {
         delete []mData;
      }
      mData = new T [size];
      mSize = size;
   }

   bool attach(unsigned int size, T *data) {
      bool rval = true;

      if (mData && mOwnData) delete [] mData;

      // std::cout << "Attaching : " << size << " bytes." << std::endl;

      mOwnData = false;
      mData    = data;
      mSize    = size;

      return rval;
   }

   T *get_data() {
      return mData;
   }

   unsigned int get_size() {
      return mSize;
   }

   // **************************************************
   // Statistic
   // --------------------------------------------------------
   T get_max() {
      T *lp = mData;
      T lmax = *lp++;
      for (int i=0;i<mSize;++i) {
         if (*lp > lmax) {
            lmax = *lp;
         }
         lp++;
      }
      return lmax;
   }

   T get_min() {
      T *lp = mData;
      T lmin = *lp++;
      for (int i=0;i<mSize;++i) {
         if (*lp < lmin) {
            lmin = *lp;
         }
         lp++;
      }
      return lmin;
   }

   // Manipulation
   // --------------------------------------------------------
   cg_buffer<double> to_double() {
      cg_buffer<double> rval(mSize);

      double *ptTemp = rval.mData;
      T      *psTemp = mData;
      for (int i=0;i<mSize;++i) {
         *ptTemp++ = (double)*psTemp++;
      }
      return rval;
   }
   void to_double(cg_buffer<double> &x) {
      double *ptTemp = x.mData;
      T      *psTemp = mData;
      for (int i=0;i<mSize;++i) {
         *ptTemp++ = (double)*psTemp++;
      }
   }

   cg_buffer<unsigned char> scale_to_unsignedchar() {
      cg_buffer<unsigned char> rval(mSize);

      T MIN = get_min();
      T MAX = get_max();
      T PlDyn = MAX - MIN;

      float fTEMP;
      unsigned char *puc = rval.mData;
      T             *pT  = mData;

      for (int i=0;i<mSize;i++) {
         fTEMP = (float)(*pT++ - MIN) * 255 / (float)PlDyn;
         *puc++ = (unsigned char)fTEMP;
      }

      return rval;
   }
   void scale_to_unsignedchar(cg_buffer<unsigned char> &x) {
      T MIN = get_min();
      T MAX = get_max();
      T PlDyn = MAX - MIN;

      float fTEMP;
      unsigned char *puc = x.mData;
      T             *pT  = mData;

      for (int i=0;i<mSize;i++) {
         fTEMP = (float)(*pT++ - MIN) * 255 / (float)PlDyn;
         *puc++ = (unsigned char)fTEMP;
      }
   }


protected:
   bool         mOwnData;
   T           *mData;
   unsigned int mSize;

   void copy(T *data, unsigned int size) {
      memcpy(mData, data ,size);
   }
};

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

      mWidth  = width;
      mHeight = height;
      mStride = stride;
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

   unsigned int get_width() {
      return mWidth;
   }

   unsigned int get_height() {
      return mHeight;
   }

   unsigned int get_stride() {
      return mStride;
   }

   // **************************************************
   // Statistic
   // --------------------------------------------------------
   T get_max() {
     return cg_buffer<T>::get_max();
   }

   T get_min() {
      return cg_buffer<T>::get_min();
   }

   cg_buffer2D<double> to_double() {
      cg_buffer2D<double> rval(mWidth, mHeight, mStride);
      cg_buffer<T>::to_double(rval.mData);
      return rval;
   }

   cg_buffer2D<unsigned char> scale_to_unsignedchar() {
      cg_buffer2D<unsigned char> rval;
      cg_buffer<T>::scale_to_usignedchar(rval.mData);
      return rval;
   }
   // Manipulation
   // --------------------------------------------------------


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


#endif


