/*
 * cg_sequencer.hpp
 *
 *  Created on: Nov 1, 2013
 *      Author: luc.martel
 */

#ifndef CG_SEQUENCER_HPP_
#define CG_SEQUENCER_HPP_

#include "cg_oal.hpp"

class cg_scb {
public:
   cg_scb();
   cg_scb(cg_task *task);
   ~cg_scb();

   void init(cg_task *task);

private:
   bool       mIsInit;
   cg_task   *mTask;
};

class cg_sequencer {
public:
   cg_sequencer();
   ~cg_sequencer();

private:
   cg_scb       *mSCBs[32];
   unsigned int  mResourcesState[32];

   unsigned int  mSCBDependencies[32];
   unsigned int  mSCBInputs[32][32];
   unsigned int  mSCBOutputs[32][32];
};


#endif /* CG_SEQUENCER_HPP_ */
