#include <czmq.h>

struct message{
	unsigned long long x;
};

enum setups {
	cat_3_2,
	cat_12_8,
	hand_3_3,
	hand_4_4,
	hand_12_12
	//TODO: incorporate these into methods
};

struct messageFloat{
	float x;
	float y;
	float z;
	float a;
	float b;
	float c;
	float d;
	float e;
	float f;
	float g;
	float h;
	float i;
};

int keepLooping = 1;
struct messageFloat last;

void signalHandler(int signal_value)
{
	keepLooping = 0;
}

int isLooping()
{
	return keepLooping;
}

void* bindpub(void* context, char* socket){
	void* publisher = zmq_socket(context, ZMQ_PUB);
	int rc = zmq_bind(publisher, socket);
	printf("Publisher Connected\n");
	return publisher;
}

void* connectsub(void* context, char* socket){
	void* subscriber = zmq_socket(context, ZMQ_SUB);
	int rc = zmq_connect(subscriber, socket);
	const char* filter = "";
	rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
	printf("Subscriber connected, waiting for other publisher\n");
	return subscriber;
}

void pubmsg(void* pub, float values[12]){
	struct messageFloat currmsg;
	currmsg.x = values[0];
	currmsg.y = values[1];
	currmsg.z = values[2];
	currmsg.a = values[3];
	currmsg.b = values[4];
	currmsg.c = values[5];
	currmsg.d = values[6];
	currmsg.e = values[7];
	currmsg.f = values[8];
	currmsg.g = values[9];
	currmsg.h = values[10];
	currmsg.i = values[11];
	zmq_msg_t msg;
	int frameize = sizeof(currmsg);
	zmq_msg_init_size(&msg, frameize);
	memcpy(zmq_msg_data(&msg), &currmsg, frameize);
	zmq_msg_send(&msg, pub, ZMQ_DONTWAIT);
}

void submsg(void* sub){
	zmq_msg_t recv_msg;
	zmq_msg_init(&recv_msg);
	int size = zmq_msg_recv(&recv_msg, sub, ZMQ_DONTWAIT);
	if(size>=0){
		struct messageFloat currmsg;
		memcpy(&currmsg, zmq_msg_data(&recv_msg), sizeof(currmsg));
		//printf("RX MSG: [1] %+6.3f\t[2] %+6.3f\t[3] %+6.3f\r", currmsg.x, currmsg.y, currmsg.z);
		memcpy(&last, &currmsg, sizeof(currmsg));
	}
	zmq_msg_close(&recv_msg);
}

void cleanup(void* sub, void* pub, void* context){
	printf("\nClosing the program\n");
	zmq_close(pub);
	zmq_close(sub);
	zmq_ctx_destroy(context);
}

struct messageFloat getLast() {
	return last;
}