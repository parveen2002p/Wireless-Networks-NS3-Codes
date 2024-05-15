import csv
import matplotlib.pyplot as plt
import statistics 

c=0
l1=[]
with open("TCPall.csv", 'r') as csvfile:
	csvreader = csv.reader(csvfile)
	for row in csvreader:
		if(c==0):
			c=1
			continue
		str1 = row[8]
		if(str1==""):
			continue
		#print(str1)
		tplist=str1.split(",")
		#print("L : "+str(tplist))
		if(len(tplist)>0):
			#print(tplist[0])
			l1.append(float(tplist[0]))
		# stamp1 = float(row[1])
		# l1.append(stamp1)


print("Median TCP RTT for OFDMA ")
#print(sum(l1)/len(l1))
print(statistics.median(l1))



c=0
l1=[]
with open("TCPall2.csv", 'r') as csvfile:
	csvreader = csv.reader(csvfile)
	for row in csvreader:
		if(c==0):
			c=1
			continue
		str1 = row[8]
		if(str1==""):
			continue
		#print(str1)
		tplist=str1.split(",")
		#print("L : "+str(tplist))
		if(len(tplist)>0):
			#print(tplist[0])
			l1.append(float(tplist[0]))
		# stamp1 = float(row[1])
		# l1.append(stamp1)


print("Median TCP RTT for OFDM ")
print(statistics.median(l1))
#print(sum(l1)/len(l1))

#0.0307915