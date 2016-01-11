#ifndef INTERN_H_
#define INTERN_H_

#include <libwebsockets.h>
#include "llist.h"

#define IP_SIZE 45
#define ID_SIZE 6
#define PORT_SIZE 6

#define INI_KW "ini"
#define SWITCH_SERVER_KW "switch-server"
#define WS_PROTO_STR "ws://"

#define K_STR_LEN 4
#define M_STR_LEN 4

char kStr[K_STR_LEN];
char mStr[M_STR_LEN];

struct per_session_data {
	char id[ID_SIZE];
	char host[IP_SIZE];
	char port[PORT_SIZE];
};

int callback_intern(struct libwebsocket_context *ctx, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);

char *getListEntry(struct per_session_data *psd);

element* get_tail();

void remove_server(struct per_session_data *psd);

char * getServerList();

char * getInitVars(struct per_session_data *psd);

char *getRedirect();

void init_server_list();

void free_server_list();

#endif /* INTERN_H_ */
