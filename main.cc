#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

// Network Topology
//
//                                      purple
//                                        |
//                         point-to-point | 10.1.6.0
//                                        |
//          10.1.1.0          10.1.2.0    |     10.1.3.0          10.1.4.0          10.1.5.0
// green -------------- n1 -------------- n2 -------------- n3 -------------- n4 -------------- blue
//       point-to-point    point-to-point    point-to-point |  point-to-point    point-to-point
//                                                          |
//                                           point-to-point | 10.1.7.0
//                                                          |
//                                                         grey

static void tracer_CWnd(uint32_t oldCwnd, uint32_t newCwnd) {
  std::cout << "TCP_Cwnd"
            << ", Time: " << Simulator::Now().GetSeconds()
            << ", old: " << oldCwnd << ", new: " << newCwnd << std::endl;
}

int main(int argc, char *argv[]) {
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);

    Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpNewReno"));
    Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(1));
    Config::SetDefault("ns3::TcpL4Protocol::RecoveryType", TypeIdValue(TypeId::LookupByName("ns3::TcpClassicRecovery")));

    NodeContainer base_nodes;
    base_nodes.Create(6); // base_nodes[0] == green, base_nodes[5] == blue

    NodeContainer purple_node;
    purple_node.Add(base_nodes.Get(2));
    purple_node.Create(1); // purple_node[1] == purple

    NodeContainer grey_node;
    grey_node.Add(base_nodes.Get(3));
    grey_node.Create(1); // grey_node[1] == grey

    PointToPointHelper p2pHelper;
    p2pHelper.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    p2pHelper.SetChannelAttribute("Delay", StringValue("5ms"));
    
    NetDeviceContainer l_0 = p2pHelper.Install(base_nodes.Get(0), base_nodes.Get(1));
    NetDeviceContainer l_1 = p2pHelper.Install(base_nodes.Get(1), base_nodes.Get(2));
    NetDeviceContainer l_2 = p2pHelper.Install(base_nodes.Get(2), base_nodes.Get(3));
    NetDeviceContainer l_3 = p2pHelper.Install(base_nodes.Get(3), base_nodes.Get(4));
    NetDeviceContainer l_4 = p2pHelper.Install(base_nodes.Get(4), base_nodes.Get(5));
    NetDeviceContainer purple_devices = p2pHelper.Install(purple_node);
    NetDeviceContainer grey_devices = p2pHelper.Install(grey_node);

    InternetStackHelper stack;
    stack.Install(base_nodes);
    stack.Install(purple_node.Get(1));
    stack.Install(grey_node.Get(1));

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer l_0_intf = address.Assign(l_0);
    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer l_1_intf = address.Assign(l_1);
    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer l_2_intf = address.Assign(l_2);
    address.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer l_3_intf = address.Assign(l_3);
    address.SetBase("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer l_4_intf = address.Assign(l_4);
    address.SetBase("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer purple_intf = address.Assign(purple_devices);  
    address.SetBase("10.1.7.0", "255.255.255.0");
    Ipv4InterfaceContainer grey_intf = address.Assign(grey_devices);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // set blue host (receiver)
    uint16_t sinkPort = 8080;
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), sinkPort));
    ApplicationContainer sinkApp = packetSinkHelper.Install(base_nodes.Get(5));
    sinkApp.Start(Seconds(0.));
    sinkApp.Stop(Seconds(20.));

    // set green host (sender)
    BulkSendHelper bulksendHelper("ns3::TcpSocketFactory", InetSocketAddress(l_4_intf.GetAddress(1), sinkPort));
    ApplicationContainer sendApp = bulksendHelper.Install(base_nodes.Get(0));
    sendApp.Start(Seconds(0.));
    sendApp.Stop(Seconds(20.));


    Simulator::Run();
    Simulator::Destroy();          
    return 0;
}