/*
 * cog_guard.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: luc.martel
 */

#ifndef CG_HPP_
#define CG_HPP_

class cg_configuration {
public:
   cg_configuration();
   ~cg_configuration();

   bool mVideoSocket;
   int  mVideoSocketPort;

};

int cg_init(cg_configuration *cg_conf);

#endif /* COG_GUARD_HPP_ */
