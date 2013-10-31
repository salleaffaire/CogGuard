/*
 * cg_profile.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: luc.martel
 */

#include "cg_profile.hpp"

cg_profiler::cg_profiler() : mTsTableSize(CG_PROFILER_DEFAULT_SIZE), mCurrentIndex(0) {
   mTsTable = new unsigned int [CG_PROFILER_DEFAULT_SIZE];
}

cg_profiler::cg_profiler(unsigned int size) : mTsTableSize(size), mCurrentIndex(0) {
   mTsTable = new unsigned int [size];
}

cg_profiler::~cg_profiler() {
   delete [] mTsTable;
}

void cg_profiler::start() {
   gettimeofday(&mCurrTimeVal, NULL);
}

unsigned int cg_profiler::stop() {
   timeval t1;
   gettimeofday(&t1, NULL);

   // Calculate usec delta
   unsigned int ellapsedTime = (t1.tv_sec - mCurrTimeVal.tv_sec) * 1000000; // Sec to usec
   ellapsedTime += (t1.tv_usec - mCurrTimeVal.tv_usec);

   // Store delta
   mTsTable[mCurrentIndex++] = ellapsedTime;

   // Update the current Time Value
   mCurrTimeVal = t1;

   // Update the rolling table index
   if (mCurrentIndex >= CG_PROFILER_DEFAULT_SIZE) {
      mCurrentIndex = 0;
   }

   return ellapsedTime;
}
