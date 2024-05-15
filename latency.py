import csv
import matplotlib.pyplot as plt


sta1Latency=[]
sta2Latency=[]
sta3Latency=[]
sta4Latency=[]
sta5Latency=[]
sta6Latency=[]
sta7Latency=[]
sta8Latency=[]
sta9Latency=[]
sta10Latency=[]


ultlist=[]

def myfun(list1,B):
	global ultlist
	B=str(B)
	c=0
	l1=[]
	with open("ap"+B+".csv", 'r') as csvfile:
		csvreader = csv.reader(csvfile)
		
		for row in csvreader:
			if(c==0):
				c=1
				continue
			stamp1 = float(row[1])
			l1.append(stamp1)

	c=0
	l2=[]
	with open("sta"+B+".csv", 'r') as csvfile:
		csvreader = csv.reader(csvfile)
		
		for row in csvreader:
			if(c==0):
				c=1
				continue
			stamp1 = float(row[1])
			l2.append(stamp1)


	for i in range(0,min(len(l1),len(l2))):
		latency=abs(l1[i]-l2[i])
		if(latency<=1):
			list1.append(latency)
	
	

myfun(sta1Latency,1)
#print(sta2Latency)
myfun(sta2Latency,2)
#print(sta2Latency)
myfun(sta3Latency,3)
myfun(sta4Latency,4)
myfun(sta5Latency,5)
myfun(sta6Latency,6)
myfun(sta7Latency,7)
myfun(sta8Latency,8)
myfun(sta9Latency,9)
myfun(sta10Latency,10)


sta1MeanLatency=sum(sta1Latency)/len(sta1Latency)
sta2MeanLatency=sum(sta2Latency)/len(sta2Latency)
sta3MeanLatency=sum(sta3Latency)/len(sta3Latency)
sta4MeanLatency=sum(sta4Latency)/len(sta4Latency)
sta5MeanLatency=sum(sta5Latency)/len(sta5Latency)
sta6MeanLatency=sum(sta6Latency)/len(sta6Latency)
sta7MeanLatency=sum(sta7Latency)/len(sta7Latency)
sta8MeanLatency=sum(sta8Latency)/len(sta8Latency)
sta9MeanLatency=sum(sta9Latency)/len(sta9Latency)
sta10MeanLatency=sum(sta10Latency)/len(sta10Latency)


ultlist.append(sta1Latency)
ultlist.append(sta2Latency)
ultlist.append(sta3Latency)
ultlist.append(sta4Latency)
ultlist.append(sta5Latency)
ultlist.append(sta6Latency)
ultlist.append(sta7Latency)
ultlist.append(sta8Latency)
ultlist.append(sta9Latency)
ultlist.append(sta10Latency)


x=[]

for i in range(1,11):
	x.append("STA "+str(i))

y=[]
y.append(sta1MeanLatency)
y.append(sta2MeanLatency)
y.append(sta3MeanLatency)
y.append(sta4MeanLatency)
y.append(sta5MeanLatency)
y.append(sta6MeanLatency)
y.append(sta7MeanLatency)
y.append(sta8MeanLatency)
y.append(sta9MeanLatency)
y.append(sta10MeanLatency)

print(sta1MeanLatency)
print(sta2MeanLatency)
print(sta3MeanLatency)
print(sta4MeanLatency)
print(sta5MeanLatency)
print(sta6MeanLatency)
print(sta7MeanLatency)
print(sta8MeanLatency)
print(sta9MeanLatency)
print(sta10MeanLatency)

print(ultlist)
print(x)
#print(sta1Latency)
#plt.figure(figsize=(60, 20))


# plt.plot(x, y, color='blue', 
# 		marker='o', markerfacecolor='blue', markersize=6)

# plt.bar(x, y, color ='orange', 
#         width = 0.4)

plt.boxplot(ultlist, labels=x, vert=False)

#print(x)
#print(y)

#plt.ylim(0,1)
#plt.xlim(0.00001,0.001)

plt.xlabel('Packet Latency (s)')
plt.title('Packet Latency Distribution for Stations')


print("Hi")

#plt.tight_layout()
#plt.legend()
plt.show()
