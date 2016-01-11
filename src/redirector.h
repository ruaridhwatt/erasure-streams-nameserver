/*
 * redirector.h
 *
 *  Created on: Jan 11, 2016
 *      Author: dv12rwt
 */

#ifndef REDIRECTOR_H_
#define REDIRECTOR_H_

#include<libwebsockets.h>

int callback_redirector(struct libwebsocket_context *ctx, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);


#endif /* REDIRECTOR_H_ */
