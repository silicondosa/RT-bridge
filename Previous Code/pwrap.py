# Python wrapper library

import sys
import zmq
import struct

# Connects subscriber socket
def connectsub(context, subaddr) :
	socket = context.socket(zmq.SUB)
	socket.connect(subaddr)
	zip_filter = ""
	socket.setsockopt_string(zmq.SUBSCRIBE, zip_filter)
	print("Python Subscriber connection successful!")
	return socket

# Binds publisher socket
def bindpub(context, pubaddr) :
	socket = context.socket(zmq.PUB)
	socket.bind(pubaddr)
	print("Python Publisher connection successful!")
	return socket 

# Publishes a message
def pubmsg(pubsocket, float1, float2, float3) :
	size = struct.calcsize('fff')
	packedmsg = struct.pack('fff', float1, float2, float3)
	pubsocket.send(packedmsg)

# Subscribes, and prints if it receives a message
def submsg(subsocket) :
	msg = subsocket.recv()
	recv_tuple = struct.unpack('fff', msg)
	print(str(recv_tuple[0]) + " " + str(recv_tuple[1]) + "  " + str(recv_tuple[2]), end="\r", flush=True)
    sys.stdout.flush()

# Old main function used for testing pub sub
def startmsg(subsocket, pubsocket, csvfile_r, csvfile_s, csvwriter_r, csvwriter_s) :
	epoch = datetime(1970, 1, 1, tzinfo=timezone.utc)
	counter = 0
	try :
		while True :
			# read from the socket and unpack into a tuple
			msg = subsocket.recv()
			recv_tuple = struct.unpack('QIBd', msg) 

			# Write to CSV file
			csvwriter_r.writerow(recv_tuple)

        	# Print every 10 iterations
			if (counter % 10) == 0 :
				print(str(recv_tuple[0]) + " ; " + str(recv_tuple[1]) + " ; " + str(recv_tuple[2])
					+ " ; " + str(recv_tuple[3]), end="\r", flush=True)
        		#print(str(latency), end="\r", flush=True) 
				sys.stdout.flush()
			counter = counter + 1

        	# Calculate timestamp
			timestamp = ( datetime.now(timezone.utc) - epoch ) // timedelta(microseconds=1)
			timestamp_sec = timestamp // 1000000
			timestamp_usec = timestamp % 1000000
			timestamp_ms = timestamp_usec / 1000

        	# Format packed message to be sent over socket
			size = struct.calcsize('QIBd')
			packedmsg = struct.pack('QIBd', timestamp_sec, timestamp_usec, size, math.sin(math.pi * timestamp_ms / 180) )

        	# Send message and write to CSV file
			pubsocket.send(packedmsg)
			csvwriter_s.writerow((timestamp_sec, timestamp_usec, size, math.sin(math.pi * timestamp_ms / 180)))

        	# Sleep for 0.1 milliseconds or 100 microseconds
			sleep(0.0001)

    # Clean up
	except KeyboardInterrupt :
		print("\nCTRL-C Interrupt detected\nClosing the Python program!")
	finally :
		csvfile_r.close()
		csvfile_s.close()

# Old main function used for header file demo
def headerdemo(subsocket, csvfile_r, csvwriter_r) :
	epoch = datetime(1970, 1, 1, tzinfo=timezone.utc)
	counter = 0
	try :
		while True :
			# read from the socket and unpack into a tuple
			msg = subsocket.recv()
			recv_tuple = struct.unpack('QIBd', msg) 

			# Write to CSV file
			csvwriter_r.writerow(recv_tuple)

        	# Print every 10 iterations
			if (counter % 10) == 0 :
				print(str(recv_tuple[0]) + " ; " + str(recv_tuple[1]) + " ; " + str(recv_tuple[2])
					+ " ; " + str(recv_tuple[3]), end="\r", flush=True)
        		#print(str(latency), end="\r", flush=True) 
				sys.stdout.flush()
			counter = counter + 1

        	# Sleep for 0.1 milliseconds or 100 microseconds
			sleep(0.0001)

    # Clean up
	except KeyboardInterrupt :
		print("\nCTRL-C Interrupt detected\nClosing the Python program!")
	finally :
		csvfile_r.close()