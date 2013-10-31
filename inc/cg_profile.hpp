/*
 * cg_profile.hpp
 *
 *  Created on: Oct 23, 2013
 *      Author: luc.martel
 */

#ifndef CG_PROFILE_HPP_
#define CG_PROFILE_HPP_

#include <sys/time.h>

#define CG_PROFILER_DEFAULT_SIZE 1024

class cg_profiler {
public:
   cg_profiler();
   cg_profiler(unsigned int size);
   ~cg_profiler();

   void start();
   unsigned int stop();

private:

   timeval       mCurrTimeVal;

   unsigned int *mTsTable;
   unsigned int  mTsTableSize;
   unsigned int  mCurrentIndex;
   unsigned int  mCurrentTs;
};

#endif /* CG_PROFILE_HPP_ */
