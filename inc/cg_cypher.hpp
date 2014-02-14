/*
 * cg_cypher.hpp
 *
 *  Created on: Dec 5, 2013
 *      Author: luc.martel
 */

#ifndef CG_CYPHER_HPP_
#define CG_CYPHER_HPP_

#include "cg_buffer.hpp"

class cg_cypher {
public:
   cg_cypher() {}
   virtual ~cg_cypher() {}

   virtual unsigned int encrypt(cg_buffer<unsigned char> &buffer);
   virtual unsigned int decrypt(cg_buffer<unsigned char> &buffer);

private:

   cg_buffer<unsigned char> mKey;
   cg_buffer<unsigned char> mIV;
};


#endif /* CG_CYPHER_HPP_ */
