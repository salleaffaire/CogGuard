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
   virtual ~cg_buffer() {
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

   T &operator[](unsigned int index) {
      return mData[index];
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

   virtual T *get_data() {
      return mData;
   }

   unsigned int get_size() {
      return mSize;
   }

   // **************************************************


protected:
   bool         mOwnData;
   T           *mData;
   unsigned int mSize;

   void copy(T *data, unsigned int size) {
      memcpy(mData, data ,size);
   }


};

#endif


