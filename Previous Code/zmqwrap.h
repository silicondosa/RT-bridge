// Header file for wrapper library on PXI and Raspberry Pi

#if defined(_WIN32) || defined(__CYGWIN__)
    #include <czmq.h>
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
    #include <zmq.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <string.h>
#endif

#ifdef __cplusplus 
extern "C" {
#endif 

    /*!
    * Defines a message with 1 integer data value.
    */
    typedef struct _message
    {
        unsigned long long x;

    } message;

    /*!
    * Defines a message with 3 float data values.
    */
    typedef struct _messagefloat
    {
        float x;
        float y;
        float z;
        
    } messagefloat;

    // A flag that indicates whether to continue the code or not.
    extern int keepLooping;

    // Updates the continuation flag in the event of an interrupt.
    void signalHandler(int signal_value);

    // Checks the continuation flag value.
    int isLooping();

    // Binds the publisher socket.
    void* bindpub(void* context, char* socket);

    // Connects the subscriber socket.
    void* connectsub(void* context, char* socket);

    // Old function used to start the messaging in one command.
    void startmsg(void* pub, int* keepLooping);

    // Publishes the message to the publisher socket.
    void pubmsg(void* pub, float values[3]);

    // Reads from the subscriber socket and prints the received message.
    void submsg(void* sub);

    // Neatly closes the sockets and context at the end.
    void cleanup(void* pub, void* sub, void* context);

#ifdef __cplusplus 
}
#endif 

