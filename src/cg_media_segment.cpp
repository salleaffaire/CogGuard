/*
 * cg_media_segment.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#include "cg_log.hpp"
#include "cg_media_segment.hpp"

cg_media_segment::cg_media_segment() : cg_buffer<unsigned char>()
{

}

cg_media_segment::cg_media_segment(unsigned int size) : cg_buffer<unsigned char>(size)
{

}

cg_media_segment::cg_media_segment(const cg_media_segment &x) : cg_buffer<unsigned char>(x)
{
   //std::cout << "cg_media_segment CC" << std::endl;
}


cg_media_segment::~cg_media_segment()
{
   //std::cout << "cg_media_segment destructor" << std::endl;
}


cg_media_segment &cg_media_segment::operator=(const cg_media_segment &x)
{
   //std::cout << "cg_media_segment operator=" << std::endl;
   return *this;
}


