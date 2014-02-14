/*
 * cg_types.hpp
 *
 *  Created on: Jan 19, 2014
 *      Author: lmartel
 */

#ifndef CG_TYPES_HPP_
#define CG_TYPES_HPP_

template <int np>
struct policies {
   uint8_t a;
   uint8_t b[np];
};

typedef policies<1> policies1_t;


#endif /* CG_TYPES_HPP_ */
