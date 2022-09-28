//#include "zmqwrap.h"
#include "zwrap.h"
#include <stdio.h>
#include <cLinkedList.h>
#include <quickDAQ.h>

// This is testing if the PXI can publish a trio of floats to the Raspberry Pi
// This code works in Visual Studio projects, but not the quickDAQ project

int main(void)
{
    void* context = zmq_ctx_new();
    void* pub = bindpub(context, (char*)"tcp://*:5555");
    //void* sub = connectsub(context, (char *)"tcp://169.254.121.192:5556");
    void* sub = connectsub(context, (char*)"tcp://169.254.115.243:5557");
    struct messageFloat received;

    quickDAQinit();
    float LCV[12] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
    const unsigned aiAddr[3] = { 1, 9, 2 };
    float64 MTR[3] = { 0.0,0.0,0.0 };
    const float64 muscleTone = 0.1;
    uInt32 DO1 = 0x000000ff;

    float64 LCoffset[7] = { 0,0,0.1396, -0.0595, 0.078, 0, 0 };
    float64 lcMult[7] = { 0,0,37.21, 43.006, 35.44, 0, 0 };
    float64 AIoffset[7] = { 0,0,.0072,.0329,.0232, 0, 0 };
    float64 mtrMult[16] = { 0.1829, 0.1582, 0.2025, 0,0,0,0,0,0,0,0,0,0,0,0,0 };
    float64 mtrOffset[16] = { 0.0585, 0.0686, 0.0519,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    float forces[3] = { 0.0, 0.0, 0.0 };
    int maxNewt = 25;

    /*int loadCells_plus[9] = { 0,8,1,9,2,10,11,3, 23 };
    for (int k = 0; k < 9; k++) {
        pinMode(5, ANALOG_IN, loadCells_plus[k]);
    }*/
    int i = 0;
    for (i = 0; i < 3; i++) {
        pinMode(5, ANALOG_IN, aiAddr[i]);
        pinMode(2, ANALOG_OUT, i);
    }
    pinMode(2, DIGITAL_OUT, 0);

    setSampleClockTiming((samplingModes)HW_CLOCKED/*DAQmxSampleMode*/, DAQmxSamplingRate, DAQmxClockSource, (triggerModes)DAQmxTriggerEdge, DAQmxNumDataPointsPerSample, TRUE);
    printf("\nIO timeout is %f\n", DAQmxDefaults.IOtimeout);


    signal(SIGINT, signalHandler);
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    //float temp[] = { x,y,z };
    //float temp[] = { 1.0, 2.0, 3.0};

    float64 AI[3] = { 0,0,0 };

    printf("\n\nPress enter to start DAQ\n\n");
    getchar();
    quickDAQstart();
    syncSampling();

    writeDigital(2, &DO1);
    writeAnalog(2, &(MTR[0]));
    printf("Motor Enabled\n");

    for (int i = 0; i < 3; i++) {
        MTR[i] = muscleTone;
    }
    writeAnalog(2, &(MTR[0]));
    printf("Motor Wound up\n");

    printf("\n\n\n");
    //int count = 0;
    while (isLooping() == 1)
    {
        //if (count == 0) {
            submsg(sub);
            received = getLast();
            MTR[0] = received.x;
            MTR[1] = received.y;
            MTR[2] = received.z;
            //if (count % 1000==0) {
            for (int j = 0; j < 3; j++) {
                forces[j] = (MTR[j] * maxNewt);
                MTR[j] = (forces[j] * mtrMult[j]) + (mtrOffset[j] * 1.1) + muscleTone;
            }

            writeAnalog(2, &(MTR[0]));
            //printf("Motors: %2.2f %2.2f %2.2f\r", MTR[0], MTR[1], MTR[2]);

            //count = 0;
            //}

            //printf("To Confirm returning struct: %+6.3f\t%+6.3f\t%+6.3f\r", received.x, received.y, received.z);
        //}

        readAnalog(5, AI);
        for (i = 0; i < 3; i++) {
            //LCV[i] = AI[i] ;
            LCV[i] = (float)(((AI[i] - LCoffset[i + 2]) * lcMult[i + 2]) + AIoffset[i + 2]);
        }
        printf("LOAD CELLS:: [1] %+6.3f\t[2] %+6.3f\t[3]%+6.3f\r", LCV[0], LCV[1], LCV[2]);
        pubmsg(pub, LCV);

        //count++;
        syncSampling();
    }

    printf("\nClosing out\n");
    for (int i = 0; i < 3; i++) {
        MTR[i] = 0;
    }
    writeAnalog(2, &(MTR[0]));
    printf("\nMotor wound down\n");


    DO1 = 0x00000000;
    writeDigital(2, &(DO1));
    printf("Motor disabled\n\n");

    syncSampling();
    quickDAQstop();

    cleanup(pub, sub, context);

    quickDAQTerminate();

    return 0;
}
