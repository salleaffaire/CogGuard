/*
 * cg_sequencer.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: luc.martel
 */

#include "cg_sequencer.hpp"


cg_scb::cg_scb() {

}
cg_scb::cg_scb(cg_task *task) {

}

cg_scb::~cg_scb() {
   if (mTask) {
      mTask->wait();
   }
   delete mTask;
}

cg_sequencer::cg_sequencer() {

   int i,j;
   for (i=0;i<32;i++) {
      mSCBs[i]            = (cg_scb *)0;
      mResourcesState[i]  = 0;
      mSCBDependencies[i] = 0;

      for (j=0;j<32;j++) {
         mSCBInputs[i][j]  = 0;
         mSCBOutputs[i][j] = 0;
      }
   }
}

cg_sequencer::~cg_sequencer() {

}

