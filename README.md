# RT-Bridge
RT-Bridge is a nearly-realtime message-passing framework built on top of the publisher-subscriber model using ZeroMQ.
It has been developed with ease of deployment and low computational overhead in mind.

# ZMQonPXI

Jaehyung Choi<br>
Valero Lab<br>
Updated November 7, 2021<br>

This repository documents the code for using ZMQ to send and receive messages between the PXI computer and the Raspberry Pi, or the hand.   
Test4 is a folder containing the code from the working Visual Studio 2019 project on the PXI computer.   


# Raspberry Pi
raspi.c, zmqwrap.c, and zmqwrap.h are the files in the Raspberry Pi titled test.c and wrap.h<br>
Raspberry Pi:<br>
Open Putty<br>
Select Raspi-test, and click Open<br>
Username: pi<br>
Password: raspberry<br>
cd zmqClient/<br>
gcc test1.c -o test1 -lzmq<br>
./test1<br>

# PXI
pxitest.c, zmqwrap.h, and zmqwrap.c are the files in Desktop > JaehyungTesting > Test2 > Test3 > (Test3.c, wrap.c, wrap.h)<br>
Open Visual Studio 2019 and open Test3.sln<br>
Run without debugging to run the code<br>

# Python
We have pprog.py, which is an example that uses the wrapper functions.<br>
We have pwrap.py, which is the Python wrapper library.<br>


