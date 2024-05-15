import csv
import matplotlib.pyplot as plt

  
aggregate_throughput = []  

current = 0
start = 0
windowdata = 0
dict={}
c=0
count=0.01

print("1 for client c3 or 2 for client w4")
inp=int(input("Enter: "))

if(inp==1):
	with open('tcpc3.csv', 'r') as csvfile:
	    csvreader = csv.reader(csvfile)
	
	    for row in csvreader:
	    	if(c==0):
	    		c=1
	    		continue
	    	packetsize = float(row[5])
	    	packettime = float(row[1])
	    	print(packetsize)
	    	print(packettime)
	    	if packettime <= start + count:
	    		windowdata += packetsize
	    	else:
	    	  throughput = float(((windowdata * 8) / (count))/int(1e6))  # Mbps
	    	  aggregate_throughput.append(throughput)
	    	  dict[start]=throughput
	    	  start += count
	    	  windowdata = packetsize

	if start not in dict:
		dict[start]=float(((windowdata * 8) / (count))/int(1e6))  # Mbps
	avg = sum(aggregate_throughput) / len(aggregate_throughput)

	print("Average Aggregate Throughput (Mbps):", avg)
	print(len(dict))

	x=[]
	y=[]

	for k in dict:
		x.append(dict[k])
		y.append(k)

	# l1=len(x)
	# while(l1<150):
	#     x.append(0)
	#     l1+=1

	# k=2;
	# while(k<=300):
	#     y.append(k)
	#     k+=2



	#plt.figure(figsize=(60, 20))


	plt.plot(y, x, color='blue', 
			marker='o', markerfacecolor='blue', markersize=6)


	#print(x)
	#print(y)

	#plt.ylim(0,1)
	# plt.xlim(1,8)

	plt.xlabel('Time (s)')

	plt.ylabel('Average Aggregate Throughput (Mbps)')


	print("Hi")

	#plt.tight_layout()
	#plt.legend()
	plt.show()

elif(inp==2):
	with open('tcpw4.csv', 'r') as csvfile:
	    csvreader = csv.reader(csvfile)
	
	    for row in csvreader:
	    	if(c==0):
	    		c=1
	    		continue
	    	packetsize = float(row[5])
	    	packettime = float(row[1])
	    	print(packetsize)
	    	print(packettime)
	    	if packettime <= start + count:
	    		windowdata += packetsize
	    	else:
	    	  throughput = float(((windowdata * 8) / (count))/int(1e6))  # Mbps
	    	  aggregate_throughput.append(throughput)
	    	  dict[start]=throughput
	    	  start += count
	    	  windowdata = packetsize
	
	if start not in dict:
		dict[start]=float(((windowdata * 8) / (count))/int(1e6))  # Mbps
	avg = sum(aggregate_throughput) / len(aggregate_throughput)
	
	print("Average Aggregate Throughput (Mbps):", avg)
	print(len(dict))
	
	x=[]
	y=[]
	
	for k in dict:
		x.append(dict[k])
		y.append(k)
	
	# l1=len(x)
	# while(l1<150):
	#     x.append(0)
	#     l1+=1
	
	# k=2;
	# while(k<=300):
	#     y.append(k)
	#     k+=2
	
	
	
	#plt.figure(figsize=(60, 20))
	
	
	plt.plot(y, x, color='blue', 
			marker='o', markerfacecolor='blue', markersize=6)
	
	
	#print(x)
	#print(y)
	
	#plt.ylim(0,1)
	# plt.xlim(1,8)
	
	plt.xlabel('Time (s)')
	
	plt.ylabel('Average Aggregate Throughput (Mbps)')
	
	
	print("Hi")
	
	#plt.tight_layout()
	#plt.legend()
	plt.show()
	