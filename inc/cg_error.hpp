/*
 * cg_error.hpp
 *
 *  Created on: Oct 7, 2013
 *      Author: luc.martel
 */

#ifndef CG_ERROR_HPP_
#define CG_ERROR_HPP_

#define CG_OK                                0

#define CG_ERR_INPUT_WIDTH_MISMATCH          (1 << 4)
#define CG_ERR_INPUT_HEIGHT_MISMATCH         (1 << 5)
#define CG_ERR_IN_OUT_WIDTH_MISMATCH         (1 << 6)
#define CG_ERR_IN_OUT_HEIGHT_MISMATCH        (1 << 7)

#define CG_ERR_NUM_PLANES_MISMATCH           (1 << 8)

#endif /* CG_ERROR_HPP_ */
