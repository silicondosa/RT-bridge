// Hello World server
#include <Windows.h>
#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <synchapi.h>

int main(void)
{
	//  Socket to talk to clients
	void* context = zmq_ctx_new();
	void* responder = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(responder, "tcp://*:5555");
	assert(rc == 0);
	printf("CZMQ server started...\n\n");

	while (1) {
		char buffer[10];
		zmq_recv(responder, buffer, 10, 0);
		printf("Received Hello\n");
		Sleep(1);          //  Do some 'work'
		zmq_send(responder, "World", 5, 0);
	}
	return 0;
}

// ========================================================

// Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("Connecting to Hello World server...\n");
	void *context = zmq_ctx_new();
	void *requeter = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://169.254.251.194:5555");

	int request_nbr;
	for(request_nbr = 0; request_nbr != 10; request_nbr++)
	{
		char buffer [10];
		printf("Sending Hello %d...\n", request_nbr);
		zmq_send(requester, "Hello", 5, 0);
		zmq_recv(requester, buffer, 10, 0);
		printf("Received World %d\n", request_nbr);
	}

	zmq_close(requester);
	zmq_ctx_destroy(context);
	return 0;
}