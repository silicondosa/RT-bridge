# Python program

import pwrap

# Set up context
context = zmq.Context()

# Set up subscriber
sub = pwrap.connectsub(context, "tcp://localhost:5556")

# Set up publisher
pub = pwrap.bindpub(context, "tcp://*:5557")
float1 = 1.0
float2 = 1.0
float3 = 1.0

try :
	while True :
		pubmsg(pub, float1, float2, float3)
		float1 = float1 + 0.1
		float2 = float2 + 0.1
		float3 = float3 + 0.1
		submsg(sub)
except KeyboardInterrupt :
	print("\nCTRL-C Interrupt detected")
finally :
	print("\nClosing the Python program!")