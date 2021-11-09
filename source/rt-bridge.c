#include "rt-bridge.h"
#include <stdio.h>
#include <czmq.h>

// Continuation flag
int rtbLoopFlag = 1;

/*!
 * \fn void rtb_signalHandler(int signal_value)
 * Changes the rtbLoopFlag flag to 0 in the event of a keyboard interrupt.
 * 
 * \param signal_value
 * \return Returns void.
*/
void rtb_signalHandler(int signal_value)
{
    rtbLoopFlag = 0;
}

/*!
 * \fn int rtb_isLooping()
 * Returns the value of the rtbLoopFlag flag.
 * 
 * \return Returns the value of the rtbLoopFlag flag, which is 1 for continue and 0 for stop.
*/
int rtb_isLooping()
{
    return rtbLoopFlag;
}

/*!
 * \fn void* rtb_initPub(void* context, char* socket)
 * Binds the publisher socket.
 * 
 * \param context The ZMQ context in which the publisher socket will do work.
 * \param socket The ZMQ socket which we will use as our publisher to send out messages.
 * \return Returns a pointer to the publisher socket that we set up.
*/
void* rtb_initPub(void* context, char* socket)
{
    void* publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, socket);
    assert(rc == 0);
    printf("Publisher (0x%p) initialized and online.\n", context);
    return publisher;
}

/*!
 * \fn void* rtb_connectSub(void* context, char* socket)
 * Connects the subscriber socket.
 * 
 * \param context The ZMQ context in which the subscriber socket will do work.
 * \param socket The ZMQ socket which we will use as our subscriber to receive messages.
 * \return Returns a pointer to the subscriber socket that we set up.
*/
void* rtb_connectSub(void* context, char* socket)
{
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, socket);
    const char* filter = "";
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
    printf("Subscriber (0x%p) connected to and waiting for publisher (%s).\n", context, socket);
    return subscriber;
}

/*!
 * \fn void rtb_publishMsg(void* pub, float values[3]) 
 * Publishes a message
 * 
 * \param pub A pointer to the publisher socket.
 * \param values An array of 3 float values that will be formatted into a message and sent out.
 * \return Returns void.
*/
void rtb_publishMsg(void* pub, float values[3]) 
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
 
/*!
 * \fn void rtb_receiveMsg(void* sub) 
 * Subscribes and prints a received message.
 * 
 * \param sub A pointer to the subscriber socket.
 * \return Returns void.
*/
void rtb_receiveMsg(void* sub) 
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

/*!
 * \fn void rtb_cleanup(void* pub, void* sub, void* context)
 * Neatly closes the sockets and the context at the end of our main() function.
 * 
 * \param pub A pointer to the publisher socket.
 * \param sub A pointer to the subscriber socket.
 * \param context A pointer to the ZMQ context.
 * \return Returns void.
*/
void rtb_cleanup(void* pub, void* sub, void* context)
{
    printf("\nClosing the program\n");
    zmq_close(pub);
    zmq_close(sub);
    zmq_ctx_destroy(context);
}

/*!
 * \fn void startmsg(void* pub, int* rtbLoopFlag)
 * Old main function used for starting the messaging.
 * 
 * \param pub A pointer to the publisher socket.
 * \param rtbLoopFlag A pointer to the continuation flag.
 * \return Returns void.
*/
void startmsg(void* pub, int* rtbLoopFlag)
{
    signal(SIGINT, rtbLoopFlag);
    unsigned long long i = 0;
    while (*rtbLoopFlag)
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