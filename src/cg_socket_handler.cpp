/*
 * cg_socket_handler.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: luc.martel
 */

#include <iostream>
#include <string.h>
#include "cg_socket_handler.hpp"

// Web Socket Handlers
// ----------------------------------------------------------------------------
void websocket_ready_handler(mg_connection *conn)
{
   //mg_request_info *pRequestInfo = mg_get_request_info(conn);

   //
   std::cout << "Mongoose has started" << std::endl;

   const char *message = "Server Ready !!";
   mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, message, strlen(message));

}

int websocket_data_handler(struct mg_connection *conn, int flags,
                           char *data, size_t data_len)
{
  int rval = 1;

  (void) flags; // Unused

  // For now we just log the packets received
  std::cout << "Received " << data_len << " bytes ..." << std::endl;

  // Returning zero means stopping websocket conversation.
  return rval;
}
// ----------------------------------------------------------------------------



