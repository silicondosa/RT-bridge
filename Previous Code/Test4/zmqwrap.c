#include "zmqwrap.h"

int keepLooping = 1;

void signalHandler(int signal_value)
{
    keepLooping = 0;
}

int isLooping()
{
    return keepLooping;
}

// Binds the publisher socket
void* bindpub(void* context, char* socket)
{
    void* publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, socket);
    assert(rc == 0);
    printf("Publisher connected\n");
    return publisher;
}

// Connects the subscriber socket
void* connectsub(void* context, char* socket)
{
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, socket);
    const char* filter = "";
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
    printf("Subscriber connected, waiting for other publisher\n");
    return subscriber;
}

// Publishes a message
void pubmsg(void* pub, float values[3]) 
{
    messagefloat currmsg;
    currmsg.x = values[0];
    currmsg.y = values[1];
    currmsg.z = values[2];
    zmq_msg_t msg;
    int framesize = sizeof(currmsg);
    zmq_msg_init_size(&msg, framesize);
    memcpy(zmq_msg_data(&msg), &currmsg, framesize);
    
    zmq_msg_send(&msg, pub, ZMQ_DONTWAIT);
}

// Subscribes and prints a received message
void submsg(void* sub) 
{
    zmq_msg_t recv_msg;
    zmq_msg_init(&recv_msg);
    int size = zmq_msg_recv(&recv_msg, sub, ZMQ_DONTWAIT);
    if (size != -1) {
        messagefloat currmsg;
        memcpy(&currmsg, zmq_msg_data(&recv_msg), sizeof(currmsg));
        printf("RX message: [1] %+6.3f\t[2] %+6.3f\t[3] %+6.3f\r", currmsg.x, currmsg.y, currmsg.z);
    }
    zmq_msg_close(&recv_msg);
}

// Clean up the socket (pub/sub)
void cleanup(void* pub, void* sub, void* context)
{
    printf("\nClosing the program\n");
    zmq_close(pub);
    zmq_close(sub);
    zmq_ctx_destroy(context);
}

// Old main function used for starting the messaging
void startmsg(void* pub, int* keepLooping)
{
    signal(SIGINT, signalHandler);
    unsigned long long i = 0;
    while (*keepLooping)
    {
        // Send message
        message currmsg;
        currmsg.x = i;
        zmq_msg_t msg;
        int framesize = sizeof(currmsg);
        zmq_msg_init_size(&msg, framesize);
        memcpy(zmq_msg_data(&msg), &currmsg, framesize);
        zmq_msg_send(&msg, pub, ZMQ_DONTWAIT);
        printf("Sending %llu\r", currmsg.x);
        i++;
    }
}