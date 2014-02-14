/*
 * cg_media_stream_memory.hpp
 *
 *  Created on: Dec 6, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_STREAM_MEMORY_HPP_
#define CG_MEDIA_STREAM_MEMORY_HPP_

#include "cg_media_stream.hpp"

// ---------------------------------------------------------------------
// MEDIA STREAM (INPUT)
// In memory stream
// ---------------------------------------------------------------------

class cg_media_stream_in_memory : public cg_media_stream_in {
public:
   cg_media_stream_in_memory() : mSizeInWords(0) {}
   // Source from file
   cg_media_stream_in_memory(std::string &filename) : mFileName(filename),
                                                      mIndex(0),
                                                      mSizeInWords(0),
                                                      mSourceType(CG_FROM_FILE) {}
   virtual ~cg_media_stream_in_memory() {}

   virtual unsigned int read_word(unsigned int &word) {
      word = mBuffer[mIndex++];
      return (mIndex != (mSizeInWords+1)) * 32;
   }

   // Can't write to an input stream
   virtual unsigned int write_word(unsigned int word) {
      return 0;
   }

   virtual unsigned int reset() {
      unsigned int rval = 0;

      std::ifstream iFile;
      iFile.open(mFileName.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

      if (!iFile.fail()) {
         // Get the size
         int size = iFile.tellg();

         // Seek to the beginning
         iFile.seekg(0, std::ios::beg);

         // Convert to size in words
         mSizeInWords = (size + 3) >> 2;

         // Allocate a word buffer to hold the file content
         mBuffer.allocate(mSizeInWords);

         // Read from the file
         iFile.read((char *)mBuffer.get_data(), size);

         iFile.close();
      }
      else {
         rval = 1;
      }

      mIndex = 0;

      return rval;
   }

   virtual unsigned int get_size() {
      return mSizeInWords;
   }

   virtual unsigned int close() {
      return 0;
   }

private:
   std::string                 mFileName;
   cg_buffer<unsigned int>     mBuffer;
   unsigned int                mIndex;
   unsigned int                mSizeInWords;

   cg_media_stream::MEDIASOURCETYPE mSourceType;
};


// ---------------------------------------------------------------------
// MEDIA STREAM (OUTPUT)
// In memory stream
// ---------------------------------------------------------------------

class cg_media_stream_out_memory : public cg_media_stream_out {
public:
   cg_media_stream_out_memory() : mSizeInWords(0) {}
   // Source from file
   cg_media_stream_out_memory(std::string &filename,
                              unsigned int sizeinwords) : mFileName(filename),
                                                          mIndex(0),
                                                          mSizeInWords(sizeinwords),
                                                          mSourceType(CG_FROM_FILE) {}
   virtual ~cg_media_stream_out_memory() {}

   // Can't read from an output stream
   virtual unsigned int read_word(unsigned int &word) {
      return 0;
   }

   virtual unsigned int write_word(unsigned int word) {
      mBuffer[mIndex++] = word;
      return 32;
   }

   virtual unsigned int reset() {
      unsigned int rval = 0;

      mBuffer.allocate(mSizeInWords << 2);

      mIndex = 0;

      return rval;
   }

   virtual unsigned int get_size() {
      return mSizeInWords;
   }

   virtual unsigned int close() {
      std::ofstream oFile;

      oFile.open(mFileName.c_str(), std::ios::out|std::ios::binary);
      oFile.write((char *)mBuffer.get_data(), mIndex << 2);
      oFile.close();

      return 0;
   }

private:
   std::string                 mFileName;
   cg_buffer<unsigned int>     mBuffer;
   unsigned int                mIndex;
   unsigned int                mSizeInWords;

   cg_media_stream::MEDIASOURCETYPE mSourceType;
};



#endif /* CG_MEDIA_STREAM_MEMORY_HPP_ */
