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

    typedef struct _message
    {
        unsigned long long x;

    } message;

    typedef struct _messagefloat
    {
        float x;
        float y;
        float z;
        
    } messagefloat;

    extern int keepLooping;

    void signalHandler(int signal_value);

    int isLooping();

    void signalHandler(int signal_value);

    void* bindpub(void* context, char* socket);

    void* connectsub(void* context, char* socket);

    void startmsg(void* pub, int* keepLooping);

    void pubmsg(void* pub, float values[3]);

    void submsg(void* sub);

    void cleanup(void* pub, void* sub, void* context);

#ifdef __cplusplus 
}
#endif 

