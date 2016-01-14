/*
 * redirector.h
 *
 *  Created on: Jan 11, 2016
 *      Author: dv12rwt
 */

#ifndef REDIRECTOR_H_
#define REDIRECTOR_H_

#include<libwebsockets.h>

/**
 * Callback for the "redirector" websockets protocol. Redirects all who connect, given at least one media server
 * exists.
 * @param ctx The context
 * @param wsi The client connection
 * @param reason The reason for the callback
 * @param user The allocated per user data
 * @param in The data received
 * @param len The length in bytes of the received data
 * @return 0 for success, otherwise -1
 */
int callback_redirector(struct libwebsocket_context *ctx, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);


#endif /* REDIRECTOR_H_ */
