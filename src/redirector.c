/*
 * redirector.c
 *
 *  Created on: Jan 11, 2016
 *      Author: dv12rwt
 */

#include <string.h>
#include <stdlib.h>
#include "redirector.h"
#include "intern.h"

int callback_redirector(struct libwebsocket_context *ctx, struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len) {

	int res;
	char *redirect;

	res = 0;
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		redirect = getRedirect();
		if (redirect == NULL) {
			break;
		}
		res = libwebsocket_write(wsi, (unsigned char *) &redirect[LWS_SEND_BUFFER_PRE_PADDING],
				strlen(&redirect[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
		free(redirect);
		break;
	default:
		break;
	}
	return res;
}

