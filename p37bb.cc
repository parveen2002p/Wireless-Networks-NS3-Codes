
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include <fstream>


using namespace ns3;


NS_LOG_COMPONENT_DEFINE("WN_ASSIGN_2_Q_2_B_PART");

class MyApp : public Application
{
  public:
    MyApp();
    virtual ~MyApp();

    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               uint32_t nPackets,
               DataRate dataRate);

  private:
    virtual void StartApplication(void);
    virtual void StopApplication(void);

    void ScheduleTx(void);
    void SendPacket(void);

    Ptr<Socket> m_socket;
    Address m_peer;
    uint32_t m_packetSize;
    uint32_t m_nPackets;
    DataRate m_dataRate;
    EventId m_sendEvent;
    bool m_running;
    uint32_t m_packetsSent;
};

MyApp::MyApp()
    : m_socket(0),
      m_peer(),
      m_packetSize(0),
      m_nPackets(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0)
{
}

MyApp::~MyApp()
{
    m_socket = 0;
}

void
MyApp::Setup(Ptr<Socket> socket,
             Address address,
             uint32_t packetSize,
             uint32_t nPackets,
             DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
MyApp::StartApplication(void)
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
MyApp::StopApplication(void)
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
MyApp::SendPacket(void)
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx();
    }
}

void
MyApp::ScheduleTx(void)
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &MyApp::SendPacket, this);
    }
}

static void
CwndChange(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    // NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
    *stream->GetStream() << Simulator::Now().GetSeconds() << "\t" << oldCwnd << "\t" << newCwnd
                         << std::endl;
}



int
main(int argc, char* argv[])
{
   

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer myp2pNodes;
    myp2pNodes.Create(2);

    PointToPointHelper mypointToPoint;
    mypointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    mypointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer myp2pDevices;
    myp2pDevices = mypointToPoint.Install(myp2pNodes);

    NodeContainer mycsmaNodes;
    mycsmaNodes.Add(myp2pNodes.Get(1));
    mycsmaNodes.Create(2);
    // 2
    NodeContainer mycsmaNodes2;
    mycsmaNodes2.Add(myp2pNodes.Get(0));
    mycsmaNodes2.Create(1);

    CsmaHelper mycsma;
    mycsma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    mycsma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    // 2
    CsmaHelper mycsma2;
    mycsma2.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    mycsma2.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    NetDeviceContainer mycsmaDevices;
    mycsmaDevices = mycsma.Install(mycsmaNodes);
    // 2
    NetDeviceContainer mycsmaDevices2;
    mycsmaDevices2 = mycsma2.Install(mycsmaNodes2);

    NodeContainer mywifiClientNodes;
    mywifiClientNodes.Create(4);
    NodeContainer mywifiApNode = myp2pNodes.Get(0);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiMacHelper mymac;
    Ssid myssid = Ssid("ns-3-myssid");

    WifiHelper mywifi;

    NetDeviceContainer myClientDevices;
    mymac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(myssid), "ActiveProbing", BooleanValue(false));
    myClientDevices = mywifi.Install(phy, mymac, mywifiClientNodes);

    NetDeviceContainer myapDevices;
    mymac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(myssid));
    myapDevices = mywifi.Install(phy, mymac, mywifiApNode);

    MobilityHelper mymobility;

    mymobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(10.0),
                                  "GridWidth",
                                  UintegerValue(3),
                                  "LayoutType",
                                  StringValue("RowFirst"));

    mymobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds",
                              RectangleValue(Rectangle(-50, 50, -50, 50)));
    mymobility.Install(mywifiClientNodes);

    mymobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mymobility.Install(mywifiApNode);

    InternetStackHelper mystack;
    //mystack.Install(myp2pNodes);
    mystack.Install(mycsmaNodes);
    // 2
    mystack.Install(mycsmaNodes2);

    mystack.Install(mywifiApNode);
    mystack.Install(mywifiClientNodes);

    Ipv4AddressHelper myaddress;

    myaddress.SetBase("167.7.1.0", "255.255.255.0");
    Ipv4InterfaceContainer myp2pInterfaces;
    myp2pInterfaces = myaddress.Assign(myp2pDevices);

    myaddress.SetBase("167.7.2.0", "255.255.255.0");
    Ipv4InterfaceContainer mycsmaInterfaces;
    mycsmaInterfaces = myaddress.Assign(mycsmaDevices);
    // 2
    myaddress.SetBase("167.7.4.0", "255.255.255.0");
    Ipv4InterfaceContainer mycsmaInterfaces2;
    mycsmaInterfaces2 = myaddress.Assign(mycsmaDevices2);


    myaddress.SetBase("167.7.3.0", "255.255.255.0");
    Ipv4InterfaceContainer mystaInterfaces= myaddress.Assign(myClientDevices);
    Ipv4InterfaceContainer myapInterfaces=myaddress.Assign(myapDevices);




    // --------------------------------------------------------------------------------------------

    // change these parameters for different simulations
    // std::string bandwidth = "5Mbps";
    // std::string delay = "5ms";
    //double error_rate = 0.000001;
    //int queuesize = 10;       // packets
    int simulation_time = 100; // seconds
    // TCP variant set to NewReno
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));

    // set qsize
    // Config::SetDefault ("ns3::PfifoFastQueueDisc::SetMaxSize", UintegerValue(queuesize));
    //Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("10p"));

    // mypointToPoint.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(queuesize));
    // mypointToPoint.SetQueue("ns3::DropTailQueue<Packet>",
    //                       "MaxSize",
    //                       QueueSizeValue(QueueSize("10p"))); // p in 100p stands for packets

    // mycsma2.SetQueue("ns3::DropTailQueue<Packet>","MaxSize",QueueSizeValue(QueueSize("10p"))); // p in 100p stands for packets
    // // mayy change packet size or count
    // //mywifi.SetQueue("ns3::DropTailQueue<Packet>","MaxSize",QueueSizeValue(QueueSize("10p"))); 

    // Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    // em->SetAttribute("ErrorRate", DoubleValue(error_rate));
    // //myp2pDevices.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
    // mycsmaDevices.Get(2)->SetAttribute("ReceiveErrorModel", PointerValue(em));  // server 
    // //myClientDevices.Get(3)->SetAttribute("ReceiveErrorModel", PointerValue(em));  // server 

// --------------------------------------------------------------------------------------------



//----------UDP PART--------------------------------------------
    
    std::cout << "C1: "<<mycsmaInterfaces2.GetAddress(1)<<std::endl;
    std::cout << "C3: "<<mycsmaInterfaces.GetAddress(2)<<std::endl;
    std::cout << "W4: "<<mystaInterfaces.GetAddress(3)<<std::endl;

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

//----------UDP PART--------------------------------------------

    Simulator::Stop(Seconds(100.0));

    //------------------------------------------------------------------------------------------------------
    
    // add server here
    uint16_t sinkPort = 8080;  // change interface          // server interface
    Address sinkAddress(InetSocketAddress(mycsmaInterfaces2.GetAddress(1), sinkPort));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",
                                      InetSocketAddress(Ipv4Address::GetAny(), sinkPort));  // may need insert ip correct
    ApplicationContainer sinkApps = packetSinkHelper.Install(mycsmaNodes2.Get(1));    // add your node for server

    //TIME::::::
    sinkApps.Start(Seconds(0.));
    sinkApps.Stop(Seconds(simulation_time));

    // add client here 

    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket(mycsmaNodes.Get(2), TcpSocketFactory::GetTypeId());   // add your node
    Ptr<Socket> ns3TcpSocket2 = Socket::CreateSocket(mywifiClientNodes.Get(3), TcpSocketFactory::GetTypeId());   // add your node

    Ptr<MyApp> app = CreateObject<MyApp>();
    // packet size,npackets
    app->Setup(ns3TcpSocket, sinkAddress, 2048, 100, DataRate("100Mbps"));   //?packet size,interval
    mycsmaNodes.Get(2)->AddApplication(app);   // add your node

    // W4 ADDED
    Ptr<MyApp> app2 = CreateObject<MyApp>();
    app2->Setup(ns3TcpSocket2, sinkAddress, 2048, 100, DataRate("100Mbps"));
    mywifiClientNodes.Get(3)->AddApplication(app2);   // add your node
    

    //TIME:::::::::
    app->SetStartTime(Seconds(1.));
    app->SetStopTime(Seconds(simulation_time));
    app2->SetStartTime(Seconds(1.));
    app2->SetStopTime(Seconds(simulation_time));

    // trace cwnd
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream("tcp-example.cwnd");
    ns3TcpSocket->TraceConnectWithoutContext("CongestionWindow",
                                             MakeBoundCallback(&CwndChange, stream));

    AsciiTraceHelper asciiTraceHelper2;
    Ptr<OutputStreamWrapper> stream2 = asciiTraceHelper2.CreateFileStream("tcp-example2.cwnd");
    ns3TcpSocket2->TraceConnectWithoutContext("CongestionWindow",
                                             MakeBoundCallback(&CwndChange, stream2));  ///? need change




//-------------TRACE----------------------------------------------------------------------
    
        AsciiTraceHelper ascii;
        AsciiTraceHelper ascii2;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        mycsma.EnableAsciiAll(ascii.CreateFileStream("scratch/tcp-example.tr"));  
        phy.EnableAsciiAll(ascii2.CreateFileStream("scratch/tcp-example2.tr"));  

        //mycsma.EnablePcapAll("scratch/tcp-example");
        //mypointToPoint.EnablePcapAll("scratch/ztpAll");
        //phy.EnablePcap("scratch/ztpAP", myapDevices.Get(0));
        //mycsma.EnablePcap("scratch/ztpR2", mycsmaDevices.Get(0), true);  // r2 to c2,c3
        //mycsma.EnablePcap("scratch/ztpC2", mycsmaDevices.Get(1), true);
        mycsma.EnablePcap("scratch/ztpC3", mycsmaDevices.Get(2), true);
        mycsma2.EnablePcap("scratch/ztpC1", mycsmaDevices2.Get(1), true); // c1
        phy.EnablePcap("scratch/ztpW4",  myClientDevices.Get(3), true);  // w4

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}


