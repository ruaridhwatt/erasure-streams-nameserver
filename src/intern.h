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

/**
 * Callback for the "intern" websockets protocol
 * @param ctx The context
 * @param wsi The client connection
 * @param reason The reason for the callback
 * @param user The allocated per user data
 * @param in The data received
 * @param len The length in bytes of the received data
 * @return 0 for success, otherwise -1
 */
int callback_intern(struct libwebsocket_context *ctx, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);

/**
 * Creates a string format of the psd
 * @param psd The per session data
 * @return The corresponding string
 */
char *getListEntry(struct per_session_data *psd);

/**
 * Gets the tail element of the media server list
 * @return The tail element of the media server list
 */
element* get_tail();

/**
 * Removes the media server with the given psd from the list of media servers
 * @param psd The per session data
 */
void remove_server(struct per_session_data *psd);

/**
 * gets the media server list in string format (lst\thost:port:id\thost:port:id\t...)
 * @return
 */
char * getServerList();

/**
 * Gets the initialization variables in string format to send to the media server
 * @param psd The media server's per session data
 * @return The initialization string
 */
char * getInitVars(struct per_session_data *psd);

/**
 * Gets a redirect string to send to the web client.
 * @return the redirect command string
 */
char *getRedirect();

/**
 * Initializes the media server list
 */
void init_server_list();

/**
 * Frees the media server list
 */
void free_server_list();

#endif /* INTERN_H_ */
