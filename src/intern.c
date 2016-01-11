#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "intern.h"

const int nrNSCommands = 2;
const char *nsCommandStr[] = { "prt" };
enum nsCommand {
	PUT_PORT = 0, UNKNOWN = 2
};

llist *serverList = NULL;
int idCounter = 0;

enum nsCommand getNSCommand(char *in) {
	int i;
	for (i = 0; i < nrNSCommands; i++) {
		if (strcmp(in, nsCommandStr[i]) == 0) {
			break;
		}
	}
	return (enum nsCommand) i;
}

int callback_intern(struct libwebsocket_context *ctx, struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len) {

	static element *tail = NULL;

	enum nsCommand c;
	struct per_session_data *psd;
	int res;
	char client_name[IP_SIZE];
	char client_ip[IP_SIZE];
	char* port, *listEntry, *toSend;

	psd = (struct per_session_data *) user;
	res = 0;

	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:

		libwebsockets_get_peer_addresses(ctx, wsi, libwebsocket_get_socket_fd(wsi), client_name, sizeof(client_name),
				client_ip, sizeof(client_ip));

		fprintf(stderr, "Received network connect from %s (%s)\n", client_name, client_ip);

		strcpy(psd->host, client_ip);
		sprintf(psd->id, "%d", ++idCounter);

		toSend = getInitVars(psd);

		res = libwebsocket_write(wsi, (unsigned char*) &toSend[LWS_SEND_BUFFER_PRE_PADDING],
				strlen(&toSend[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
		fprintf(stderr, "sent ini: %d\n", res);
		free(toSend);

		break;

	case LWS_CALLBACK_RECEIVE:
		fprintf(stderr, "%s\n", (char*) in);
		c = getNSCommand(strtok(in, "\t"));

		switch (c) {
		case PUT_PORT:
			port = strtok(NULL, "\t");

			if (port == NULL) {
				res = -1;
				break;
			}
			if (psd->port[0] != '\0') {
				break;
			}

			strcpy(psd->port, port);
			listEntry = getListEntry(psd);

			if (tail == NULL) {
				tail = llist_first(serverList);
			}
			tail = llist_insert(tail, serverList, listEntry);
			tail = llist_next(tail);

			toSend = getServerList();

			res = libwebsocket_write(wsi, (unsigned char*) &toSend[LWS_SEND_BUFFER_PRE_PADDING],
					strlen(&toSend[LWS_SEND_BUFFER_PRE_PADDING]), LWS_WRITE_TEXT);
			free(toSend);
			break;
		default:
			break;
		}

		break;

	case LWS_CALLBACK_CLOSED:
		fprintf(stderr, "connection closed\n");
		fprintf(stderr, "%s\n", psd->id);
		remove_server(psd);
		tail = get_tail();
		break;
	default:
		break;
	}
	return 0;
}

char *getListEntry(struct per_session_data *psd) {
	char *buf;
	size_t strSize;

	strSize = strlen(psd->host) + 1;
	strSize += strlen(psd->port) + 1;
	strSize += strlen(psd->id) + 1;
	buf = malloc(strSize * sizeof(char));
	strcpy(buf, psd->host);
	strcat(buf, ":");
	strcat(buf, psd->port);
	strcat(buf, ":");
	strcat(buf, psd->id);
	return buf;
}

element* get_tail() {
	element *position;
	position = llist_first(serverList);
	while (!llist_isEnd(position)) {
		position = llist_next(position);
	}
	return position;

}

void remove_server(struct per_session_data *psd) {
	element *pos;
	char *psdStr;

	psdStr = getListEntry(psd);
	pos = llist_first(serverList);
	while (!llist_isEnd(pos)) {
		if (strcmp(psdStr, (char *) llist_inspect(pos)) == 0) {
			llist_remove(pos, serverList);
			break;
		}
		pos = llist_next(pos);
	}
	free(psdStr);
}

char * getServerList() {
	element *position;
	char *listContains;
	size_t buff_len;

	position = llist_first(serverList);
	buff_len = LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING + (sizeof(char) * (strlen("lst") + 1));
	listContains = malloc(buff_len);
	strcpy(&listContains[LWS_SEND_BUFFER_PRE_PADDING], "lst");

	while (!llist_isEnd(position)) {
		buff_len += sizeof(char) * (strlen((char*) llist_inspect(position)) + 1);
		listContains = realloc(listContains, buff_len);
		strcat(&listContains[LWS_SEND_BUFFER_PRE_PADDING], "\t");
		strcat(&listContains[LWS_SEND_BUFFER_PRE_PADDING], llist_inspect(position));

		position = llist_next(position);

	}
	return listContains;
}

char *getInitVars(struct per_session_data *psd) {
	char *buf, *dest;
	size_t strSize;

	strSize = strlen(INI_KW) + 1;
	strSize += strlen(psd->id) + 1;
	strSize += strlen(kStr) + 1;
	strSize += strlen(mStr) + 1;

	buf = malloc(LWS_SEND_BUFFER_PRE_PADDING + (strSize * sizeof(char *)) + LWS_SEND_BUFFER_POST_PADDING);
	dest = strcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], INI_KW);
	dest = strcat(dest, "\t");
	dest = strcat(dest, psd->id);
	dest = strcat(dest, "\t");
	dest = strcat(dest, kStr);
	dest = strcat(dest, "\t");
	dest = strcat(dest, mStr);

	return buf;
}

char *getRedirect() {
	char *listEntry, *buf, *pos, *dest;
	size_t strSize;

	if (llist_isEmpty(serverList)) {
		return NULL;
	}
	listEntry = (char *) llist_inspect(llist_first(serverList));
	pos = strrchr(listEntry, ':');

	strSize = strlen(SWITCH_SERVER_KW) + 1;
	strSize += strlen(WS_PROTO_STR);
	strSize += pos - listEntry + 1;

	buf = malloc(LWS_SEND_BUFFER_PRE_PADDING + (strSize * sizeof(char *)) + LWS_SEND_BUFFER_POST_PADDING);
	dest = strcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], SWITCH_SERVER_KW);
	dest = strcat(dest, "\t");
	dest = strcat(dest, WS_PROTO_STR);
	dest = strncat(dest, listEntry, pos - listEntry);
	dest[strSize - 1] = '\0';
	return buf;
}

void init_server_list() {
	serverList = llist_empty(free);
}

void free_server_list() {
	llist_free(serverList);
}

