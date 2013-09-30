/*
 * cg_media_segment_mp2ts.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: luc.martel
 */

#include "cg_media_segment_mp2ts.hpp"

cg_media_segment_mp2ts::cg_media_segment_mp2ts() : cg_media_segment()
{

}

cg_media_segment_mp2ts::cg_media_segment_mp2ts(unsigned int size) : cg_media_segment(size)
{

}

cg_media_segment_mp2ts::cg_media_segment_mp2ts(const cg_media_segment_mp2ts &x) : cg_media_segment(x)
{
   //std::cout << "cg_media_segment CC" << std::endl;
}


cg_media_segment_mp2ts::~cg_media_segment_mp2ts()
{
   //std::cout << "cg_media_segment destructor" << std::endl;
}


cg_media_segment_mp2ts &cg_media_segment_mp2ts::operator=(const cg_media_segment_mp2ts &x)
{
   //std::cout << "cg_media_segment operator=" << std::endl;
   return *this;
}




