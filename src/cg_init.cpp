/*
 * cog_guard_init.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: luc.martel
 */

#include <string.h>
#include "mongoose.h"
#include "cg.hpp"
#include "cg_socket_handler.hpp"

int cg_init(cg_configuration *cg_conf)
{
   int rval = 0;

   struct mg_context  *ctx;
   struct mg_callbacks callbacks;

   const char *options[] = {
     "listening_ports", "8901",
     "document_root",   "websocket_html_root",
     NULL
   };

   // Setting all callback pointers to 0
   memset(&callbacks, 0, sizeof(callbacks));

   // Initialize
   callbacks.websocket_ready = websocket_ready_handler;

   // Respond to data received
   callbacks.websocket_data = websocket_data_handler;

   // Start the server
   ctx = mg_start(&callbacks, NULL, options);

   // Wait until the user press something
   getchar();

   // Stopping the server
   mg_stop(ctx);

   return rval;
}


