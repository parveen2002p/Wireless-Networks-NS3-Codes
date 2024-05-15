#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include <iostream>

// Network Topology AS Per Assignment-2
//
//        Wifi
//             
//  *   *    *    *            (point-to-point)
//  |   |    |    |   (AP)       
//  w4  w3   w2   w1   R1 ---------------------  R2   C2   C3
//                     |                          |    |    |
//                     |                        ================
//                    C1 (wired)                    Ethernet
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WN_ASSIGN_2_Q_2_A_PART");

int
main(int argc, char* argv[])
{
    // can change name?
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);


    NodeContainer wiredR1_R2nodes;
    wiredR1_R2nodes.Create(2);

    PointToPointHelper wiredR1_R2helper;
    wiredR1_R2helper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    wiredR1_R2helper.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer wiredR1_R2devices;
    wiredR1_R2devices = wiredR1_R2helper.Install(wiredR1_R2nodes);

    NodeContainer csmaNodes_R2linked;
    csmaNodes_R2linked.Add(wiredR1_R2nodes.Get(1));
    csmaNodes_R2linked.Create(2);
    // 2
    NodeContainer csmaNodes2_R1linked;
    csmaNodes2_R1linked.Add(wiredR1_R2nodes.Get(0));
    csmaNodes2_R1linked.Create(1);

    CsmaHelper csmaR2;
    csmaR2.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csmaR2.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    // 2
    CsmaHelper csmaR1;
    csmaR1.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csmaR1.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    NetDeviceContainer csma_R2Devices;
    csma_R2Devices = csmaR2.Install(csmaNodes_R2linked);
    // 2
    NetDeviceContainer csma_R1Devices2;
    csma_R1Devices2 = csmaR1.Install(csmaNodes2_R1linked);

    NodeContainer wifi_w_nodes;
    wifi_w_nodes.Create(4);
    NodeContainer wifi_apR1_node = wiredR1_R2nodes.Get(0);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    Ssid ssid = Ssid("mySSID"); 

    WifiHelper wifi;

    NetDeviceContainer wifi_w_devices;
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    wifi_w_devices = wifi.Install(phy, mac, wifi_w_nodes);

    NetDeviceContainer wifi_apR1_devices;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    wifi_apR1_devices = wifi.Install(phy, mac, wifi_apR1_node);

    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
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

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds",
                              RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility.Install(wifi_w_nodes);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifi_apR1_node);

    InternetStackHelper stack;
    //stack.Install(wiredR1_R2nodes);
    stack.Install(csmaNodes_R2linked);
    // 2
    stack.Install(csmaNodes2_R1linked);

    stack.Install(wifi_apR1_node);
    stack.Install(wifi_w_nodes);

    Ipv4AddressHelper address;
    

    address.SetBase("167.7.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(wiredR1_R2devices);

    address.SetBase("167.7.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csma_R2Devices);
    // 2
    address.SetBase("167.7.4.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces2;
    csmaInterfaces2 = address.Assign(csma_R1Devices2);

    address.SetBase("167.7.3.0", "255.255.255.0");
    Ipv4InterfaceContainer staInterfaces= address.Assign(wifi_w_devices);
    Ipv4InterfaceContainer apInterfaces=address.Assign(wifi_apR1_devices);
   
    // UDP PART---------------------------------------------------------------

    // UdpEchoServerHelper echoServer(9);  // random

    // ApplicationContainer serverApps =echoServer.Install(csmaNodes2_R1linked.Get(1)); // C1 as server 
    // serverApps.Start(Seconds(1.0));
    // serverApps.Stop(Seconds(100.0));

    // // Create UDP echo client on C3 and W4 (clients)
    // UdpEchoClientHelper echoClientC3(csmaInterfaces2.GetAddress(1), 9); // C3 as client
    // //UdpEchoClientHelper echoClientC3(csmaInterfaces.GetAddress(2), 9); // C3 as client   3 r1 c2 c3
    // echoClientC3.SetAttribute("MaxPackets", UintegerValue(100));
    // echoClientC3.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    // echoClientC3.SetAttribute("PacketSize", UintegerValue(2048));

    // UdpEchoClientHelper echoClientW4(csmaInterfaces2.GetAddress(1), 9); // W4 as client
    // //UdpEchoClientHelper echoClientW4(staInterfaces.GetAddress(3), 9); // W4 as client sta: w1 w2 w3 w4
    // echoClientW4.SetAttribute("MaxPackets", UintegerValue(100));
    // echoClientW4.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    // echoClientW4.SetAttribute("PacketSize", UintegerValue(2048));

    // ApplicationContainer clientAppsC3 = echoClientC3.Install(csmaNodes_R2linked.Get(2)); // C3 as client
    // ApplicationContainer clientAppsW4 = echoClientW4.Install(wifi_w_nodes.Get(3)); // W4 as client

    // clientAppsC3.Start(Seconds(2.0));
    // clientAppsC3.Stop(Seconds(100.0));

    // clientAppsW4.Start(Seconds(2.0));
    // clientAppsW4.Stop(Seconds(100.0));

    // std::cout << "C1: "<<csmaInterfaces2.GetAddress(1)<<std::endl;
    // std::cout << "C3: "<<csmaInterfaces.GetAddress(2)<<std::endl;
    // std::cout << "W4: "<<staInterfaces.GetAddress(3)<<std::endl;

    //Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Stop(Seconds(100.0));

  
        // phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        // //wiredR1_R2helper.EnablePcapAll("scratch/zpAll");
        // //phy.EnablePcap("scratch/zpAP", wifi_apR1_devices.Get(0));
        // //csmaR2.EnablePcap("scratch/zpR2", csma_R2Devices.Get(0), true);  // r2 to c2,c3
        // //csmaR2.EnablePcap("scratch/zpC2", csma_R2Devices.Get(1), true);
        // csmaR2.EnablePcap("scratch/zpC3", csma_R2Devices.Get(2), true);
        // csmaR1.EnablePcap("scratch/zpC1", csma_R1Devices2.Get(1), true); // c1
        // phy.EnablePcap("scratch/zpW4",  wifi_w_devices.Get(3), true);  // w4

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
