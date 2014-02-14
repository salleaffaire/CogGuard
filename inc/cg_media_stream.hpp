/*
 * cg_media_stream.hpp
 *
 *  Created on: Dec 5, 2013
 *      Author: luc.martel
 */

#ifndef CG_MEDIA_STREAM_HPP_
#define CG_MEDIA_STREAM_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "cg_buffer.hpp"
#include "cg_cypher.hpp"


class cg_media_stream {
public:
   enum MEDIASOURCETYPE {
      CG_FROM_FILE = 1,
   };
   enum DIRECTION {
      CG_IN  = 1,
      CG_OUT = 2,
   };

   cg_media_stream(cg_media_stream::DIRECTION dir) : mDir(dir) {}
   virtual ~cg_media_stream() {}


   virtual unsigned int read_word(unsigned int &word) = 0;
   virtual unsigned int write_word(unsigned int word) = 0;
   virtual unsigned int reset() = 0;
   virtual unsigned int get_size() = 0;
   virtual unsigned int close() = 0;
   cg_media_stream::DIRECTION get_dir() {
      return mDir;
   }

protected:
   cg_media_stream::DIRECTION mDir;

private:
   cg_media_stream() {}
};

class cg_media_stream_in : public cg_media_stream {
public:
   cg_media_stream_in() : cg_media_stream(cg_media_stream::CG_IN) {}
   virtual ~cg_media_stream_in() {}

   virtual unsigned int read_word(unsigned int &word) = 0;
   virtual unsigned int write_word(unsigned int word) {
      return 0;
   }
   virtual unsigned int reset() = 0;
   virtual unsigned int get_size() = 0;
   virtual unsigned int close() = 0;

};

class cg_media_stream_out : public cg_media_stream {
public:
   cg_media_stream_out() :  cg_media_stream(cg_media_stream::CG_OUT) {}
   virtual ~cg_media_stream_out() {}

   virtual unsigned int read_word(unsigned int &word) {
      return 0;
   }
   virtual unsigned int write_word(unsigned int word) = 0;
   virtual unsigned int reset() = 0;
   virtual unsigned int get_size() = 0;
   virtual unsigned int close() = 0;

};





#endif /* CG_MEDIA_STREAM_HPP_ */
