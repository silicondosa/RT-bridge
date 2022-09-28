#include "zmqwrap.h"

static int keepLooping = 1;

void signalHandler(int signal_value) 
{
	keepLooping = 0;
}

int main(void) 
{
	void* context = zmq_ctx_new();
	void* pub = bindpub(context, "tcp://*:5556");
	void* sub = connectsub(context, "tcp://169.254.251.194:5555");

	signal(SIGINT, signalHandler);

	float temp[] = { 0.0,0.0,0.0 };
	
	while (keepLooping) 
	{
		submsg(sub);
		temp[0] -= 0.5;
		temp[1] -= 0.5;
		temp[2] -= 0.5;
		pubmsg(pub, temp);
	}

	cleanup(pub, sub, context);
	return 0;
}