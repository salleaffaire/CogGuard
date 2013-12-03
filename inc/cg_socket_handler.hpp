/*
 * cg_socket_handler.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: luc.martel
 */

#ifndef CG_SOCKET_HANDLER_HPP_
#define CG_SOCKET_HANDLER_HPP_

#include "mongoose.h"

int websocket_data_handler(struct mg_connection *conn, int flags,
                           char *data, size_t data_len);

void websocket_ready_handler(mg_connection *conn);

#endif /* CG_SOCKET_HANDLER_HPP_ */

