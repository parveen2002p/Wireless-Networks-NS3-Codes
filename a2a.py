import csv
import matplotlib.pyplot as plt

  
mylist = []
c=0
t1 = 0
end = 0

print("1 for client c3 or 2 for client w4")
inp=int(input("Enter: "))

if(inp==1):
        
    
    with open('udpc3.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        
        for row in csvreader:
            if(c==0):
                c=1
                continue
            timestamp = float(row[1])
            
            end = timestamp
    
            latency = end - t1
    
            mylist.append(latency)
            t1 = timestamp
    
    
    avg = sum(mylist) / len(mylist)
    print("Average Latency (s):", avg)
    
    # now plot
    
    
    x=[]
    # y=[]
    
    i=1
    for k in mylist:
        x.append(str(i))
        i=i+1
    
    #plt.figure(figsize=(60, 20))
    
    # print(mylist)
    # print(x)
    
    # plt.plot(mylist, x, color='blue', 
    # 		marker='o', markerfacecolor='blue', markersize=6)
    
    # change packet to discrete values
    #plt.plot(x,mylist)
    plt.scatter(x,mylist)
    
    
    
    
    #plt.ylim(0,1)
    # plt.xlim(1,8)
    
    plt.ylabel('Time (s)')
    
    plt.xlabel('Packet No. ')
    
    
    print("Hi")
    
    #plt.tight_layout()
    #plt.legend()
    plt.show()

elif(inp==2):
        
    # w4
    with open('udpw4.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        
        for row in csvreader:
            if(c==0):
                c=1
                continue
            timestamp = float(row[1])
            
            end = timestamp
    
            latency = end - t1
    
            mylist.append(latency)
            t1 = timestamp
    
    
    avg = sum(mylist) / len(mylist)
    print("Average Latency (s):", avg)
    
    # now plot
    
    
    x=[]
    # y=[]
    
    i=1
    for k in mylist:
        x.append(str(i))
        i=i+1
    
    #plt.figure(figsize=(60, 20))
    
    # print(mylist)
    # print(x)
    
    # plt.plot(mylist, x, color='blue', 
    # 		marker='o', markerfacecolor='blue', markersize=6)
    
    # change packet to discrete values
    #plt.plot(x,mylist)
    plt.scatter(x,mylist)
    
    
    
    
    #plt.ylim(0,1)
    # plt.xlim(1,8)
    
    plt.ylabel('Time (s)')
    
    plt.xlabel('Packet No. ')
    
    
    print("Hi")
    
    #plt.tight_layout()
    #plt.legend()
    plt.show()
    
    
    
    