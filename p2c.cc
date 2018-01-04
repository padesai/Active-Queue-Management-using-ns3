#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/sequence-number.h"

using namespace ns3;

// i hate typing _t so I'm making shortcuts
using i32 = int32_t;
using u32 = uint32_t;
//if you're not familiar with the above lines, it's like an oldschool typedef

//i can now do things like this:
//	i32 myInt = 5 

//instead of this: 
//	int32_t myInt = 5

// yes, i'm that lazy


std::stringstream filePlotQueue;
std::stringstream filePlotQueueAvg;
std::stringstream filePlotQueueDropped;
std::stringstream filePlotQueueEnqueue;
std::stringstream filePlotQueueDequeue;
std::stringstream filePlotQueue2;
std::stringstream filePlotQueue2Avg;
std::stringstream filePlotQueue2Dropped;
std::stringstream filePlotQueue2Enqueue;
std::stringstream filePlotQueue2Dequeue;


u32 checkTimes;
double avgQueueSize;

u32 checkTimes2;
double avgQueue2Size;

u32 port = 5000;

constexpr u32 packetSize = 1000 - 42;



//REMEMBER:  This is not an exact replica of what you need to create.  It just shows something relatively similar.
//NOTE: What it is I do in this example may not be the only way to achieve what you're trying to do for this assignment

void EnqueueAtRedNew(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueue2Enqueue(filePlotQueue2Enqueue.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueue2Enqueue << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueue2Enqueue.close();

	//TODO:  Need to figure out how to print the time this packet arrived and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph

}

void DequeueAtRedNew(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueue2Dequeue(filePlotQueue2Dequeue.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueue2Dequeue << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueue2Dequeue.close();
	
	//TODO:  Need to figure out how to print the time this packet left and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph

}

void DroppedAtRedNew(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueue2Dropped(filePlotQueue2Dropped.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueue2Dropped << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueue2Dropped.close();
	
	//TODO:  Need to figure out how to print the time this packet was dropped and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph
}


void EnqueueAtRed(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueueEnqueue(filePlotQueueEnqueue.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueueEnqueue << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueueEnqueue.close();

	//TODO:  Need to figure out how to print the time this packet arrived and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph

}

void DequeueAtRed(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueueDequeue(filePlotQueueDequeue.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueueDequeue << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueueDequeue.close();
	
	//TODO:  Need to figure out how to print the time this packet left and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph

}

void DroppedAtRed(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	
	std::ofstream fPlotQueueDropped(filePlotQueueDropped.str().c_str(), std::ios::out | std::ios::app);	
	fPlotQueueDropped << Simulator::Now().GetSeconds() << "," << pktNumber << std::endl;		
	fPlotQueueDropped.close();
	
	//TODO:  Need to figure out how to print the time this packet was dropped and which flow it belongs to.  Hint below in app setup.
	//REMEMBER:  The sequence number is actually in bytes not packets so to make it graph nice you'll need to manipulate to get 
	//           nice consequtive sequence numbers to graph
}


//This code is fine for printing average and actual queue size
void CheckQueueSize(Ptr<QueueDisc> queue) {

	u32 qsize = StaticCast<RedQueueDisc>(queue)->GetQueueSize();
	avgQueueSize += qsize;
	checkTimes++;
	
	Simulator::Schedule(Seconds(0.01), &CheckQueueSize, queue);	

	std::ofstream fPlotQueue(filePlotQueue.str().c_str(), std::ios::out | std::ios::app);
	fPlotQueue << Simulator::Now().GetSeconds() << "," << qsize << std::endl;
	fPlotQueue.close();

	std::ofstream fPlotQueueAvg(filePlotQueueAvg.str().c_str(), std::ios::out | std::ios::app);
	fPlotQueueAvg << Simulator::Now().GetSeconds() << "," << avgQueueSize / checkTimes << std::endl;
	fPlotQueueAvg.close();
}

void CheckQueue2Size(Ptr<QueueDisc> queue) {

	u32 qsize2 = StaticCast<RedQueueDisc>(queue)->GetQueueSize();
	avgQueue2Size += qsize2;
	checkTimes2++;
	
	Simulator::Schedule(Seconds(0.01), &CheckQueue2Size, queue);	

	std::ofstream fPlotQueue2(filePlotQueue2.str().c_str(), std::ios::out | std::ios::app);
	fPlotQueue2 << Simulator::Now().GetSeconds() << "," << qsize2 << std::endl;
	fPlotQueue2.close();

	std::ofstream fPlotQueue2Avg(filePlotQueue2Avg.str().c_str(), std::ios::out | std::ios::app);
	fPlotQueue2Avg << Simulator::Now().GetSeconds() << "," << avgQueue2Size / checkTimes2 << std::endl;
	fPlotQueue2Avg.close();
}


int main(int argc, char* argv[]) {

	std::string pathOut = ".";
	bool writeForPlot = true;
	bool createVis = false;
	bool useFlowMon = true;

	//allow these to be varied via command line
	u32 runNumber = 0;
	u32 numberOfLeaves = 10;
	u32 maxPackets = 100;
	

	std::string leafLinkBW = "100Mbps";
	std::string leafLinkDelay = "0.5ms";
	std::string leafLinkDelay1 = "1ms";
	std::string leafLinkDelay5 = "5ms";
	std::string leafLinkDelay3 = "3ms";
	std::string leafLinkDelay2 = "2ms";

	std::string bottleneckLinkBW = "45Mbps";
	std::string bottleneckLinkDelay = "2ms";

	std::string animationFile = "demo.xml";


	double stopTime = 3.0;

	double minTh = 5;
	double maxTh = 15;


	CommandLine cmd;
	cmd.AddValue("runNumber", "run number for random variable generation", runNumber);
	cmd.AddValue("numberOfLeaves", "number of leaves on each side of bottleneck", numberOfLeaves);
	cmd.AddValue("animationFile", "File name for animation output", animationFile);
	cmd.AddValue("createVis", "<0/1> to create animation output", createVis);
	cmd.AddValue("writeForPlot", "<0/1> to write results for queue plot", writeForPlot);
	cmd.AddValue("useFlowMon", "<0/1> to use the flowmonitor", useFlowMon);
	cmd.AddValue("maxPackets", "Max packets allowed in the device queue", maxPackets);

	cmd.Parse(argc, argv);

	Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(packetSize));
	Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(1));

	//RED setup
	Config::SetDefault("ns3::RedQueueDisc::Mode", StringValue("QUEUE_MODE_PACKETS"));
	Config::SetDefault("ns3::RedQueueDisc::MeanPktSize", UintegerValue(packetSize));
  	Config::SetDefault ("ns3::RedQueueDisc::MinTh", DoubleValue (minTh));
  	Config::SetDefault ("ns3::RedQueueDisc::MaxTh", DoubleValue (maxTh));
  	Config::SetDefault ("ns3::RedQueueDisc::LinkBandwidth", StringValue (bottleneckLinkBW));
  	Config::SetDefault ("ns3::RedQueueDisc::LinkDelay", StringValue (bottleneckLinkDelay));
	Config::SetDefault ("ns3::RedQueueDisc::QueueLimit", UintegerValue (maxPackets));
	Config::SetDefault ("ns3::RedQueueDisc::MeanPktSize", UintegerValue (packetSize));
	Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue (0.002));

	SeedManager::SetSeed(1);
	SeedManager::SetRun(runNumber);

	NodeContainer nodes;
	nodes.Create(10);	
	
	PointToPointHelper bottleneckLink1;
	bottleneckLink1.SetDeviceAttribute ("DataRate", StringValue(bottleneckLinkBW));
	bottleneckLink1.SetChannelAttribute ("Delay", StringValue(bottleneckLinkDelay));

	PointToPointHelper bottleneckLink2;
	bottleneckLink2.SetDeviceAttribute ("DataRate", StringValue(bottleneckLinkBW));
	bottleneckLink2.SetChannelAttribute ("Delay", StringValue(bottleneckLinkDelay));

	PointToPointHelper pointToPointLeaf1;
	pointToPointLeaf1.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf1.SetChannelAttribute ("Delay", StringValue(leafLinkDelay));

	PointToPointHelper pointToPointLeaf2;
	pointToPointLeaf2.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf2.SetChannelAttribute ("Delay", StringValue(leafLinkDelay1));
	
	PointToPointHelper pointToPointLeaf3;
	pointToPointLeaf3.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf3.SetChannelAttribute ("Delay", StringValue(leafLinkDelay3));

	PointToPointHelper pointToPointLeaf4;
	pointToPointLeaf4.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf4.SetChannelAttribute ("Delay", StringValue(leafLinkDelay5));

	PointToPointHelper pointToPointLeaf5;
	pointToPointLeaf5.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf5.SetChannelAttribute ("Delay", StringValue(leafLinkDelay));

	PointToPointHelper pointToPointLeaf6;
	pointToPointLeaf6.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf6.SetChannelAttribute ("Delay", StringValue(leafLinkDelay1));
	
	PointToPointHelper pointToPointLeaf7;
	pointToPointLeaf7.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf7.SetChannelAttribute ("Delay", StringValue(leafLinkDelay5));

	PointToPointHelper pointToPointLeaf8;
	pointToPointLeaf8.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf8.SetChannelAttribute ("Delay", StringValue(leafLinkDelay2));

	PointToPointHelper pointToPointLeaf9;
	pointToPointLeaf9.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf9.SetChannelAttribute ("Delay", StringValue(leafLinkDelay));

	PointToPointHelper pointToPointLeaf10;
	pointToPointLeaf10.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf10.SetChannelAttribute ("Delay", StringValue(leafLinkDelay1));
	
	PointToPointHelper pointToPointLeaf11;
	pointToPointLeaf11.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf11.SetChannelAttribute ("Delay", StringValue(leafLinkDelay5));

	PointToPointHelper pointToPointLeaf12;
	pointToPointLeaf12.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf12.SetChannelAttribute ("Delay", StringValue(leafLinkDelay2));

	PointToPointHelper pointToPointLeaf13;
	pointToPointLeaf13.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf13.SetChannelAttribute ("Delay", StringValue(leafLinkDelay));

	PointToPointHelper pointToPointLeaf14;
	pointToPointLeaf14.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf14.SetChannelAttribute ("Delay", StringValue(leafLinkDelay1));
	
	PointToPointHelper pointToPointLeaf15;
	pointToPointLeaf15.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf15.SetChannelAttribute ("Delay", StringValue(leafLinkDelay3));

	PointToPointHelper pointToPointLeaf16;
	pointToPointLeaf16.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf16.SetChannelAttribute ("Delay", StringValue(leafLinkDelay5));	

	NetDeviceContainer devices;
    	devices = pointToPointLeaf1.Install(nodes.Get(0), nodes.Get(4));
	
	NetDeviceContainer devices2;
	devices2 = pointToPointLeaf2.Install(nodes.Get(1), nodes.Get(4));

	NetDeviceContainer devices3;
	devices3 = pointToPointLeaf3.Install(nodes.Get(2), nodes.Get(4));
	
	NetDeviceContainer devices4;
	devices4 = pointToPointLeaf4.Install(nodes.Get(3), nodes.Get(4));

	NetDeviceContainer devices5;
	devices5 = bottleneckLink1.Install(nodes.Get(4), nodes.Get(5));

	NetDeviceContainer devices6;
    	devices6 = pointToPointLeaf5.Install(nodes.Get(5), nodes.Get(6));
	
	NetDeviceContainer devices7;
	devices7 = pointToPointLeaf6.Install(nodes.Get(5), nodes.Get(7));

	NetDeviceContainer devices8;
	devices8 = pointToPointLeaf7.Install(nodes.Get(5), nodes.Get(8));
	
	NetDeviceContainer devices9;
	devices9 = pointToPointLeaf8.Install(nodes.Get(5), nodes.Get(9));

	NetDeviceContainer devices10;
	devices10 = bottleneckLink2.Install(nodes.Get(5), nodes.Get(4));

	NetDeviceContainer devices11;
    	devices11 = pointToPointLeaf9.Install(nodes.Get(6), nodes.Get(5));
	
	NetDeviceContainer devices12;
	devices12 = pointToPointLeaf10.Install(nodes.Get(7), nodes.Get(5));

	NetDeviceContainer devices13;
	devices13 = pointToPointLeaf11.Install(nodes.Get(8), nodes.Get(5));
	
	NetDeviceContainer devices14;
	devices14 = pointToPointLeaf12.Install(nodes.Get(9), nodes.Get(5));

	NetDeviceContainer devices15;
    	devices15 = pointToPointLeaf13.Install(nodes.Get(4), nodes.Get(0));
	
	NetDeviceContainer devices16;
	devices16 = pointToPointLeaf14.Install(nodes.Get(4), nodes.Get(1));

	NetDeviceContainer devices17;
	devices17 = pointToPointLeaf15.Install(nodes.Get(4), nodes.Get(2));
	
	NetDeviceContainer devices18;
	devices18 = pointToPointLeaf16.Install(nodes.Get(4), nodes.Get(3));

	InternetStackHelper stack;
	
	stack.Install(nodes);
	
	TrafficControlHelper tchBottleneck1;
	tchBottleneck1.SetRootQueueDisc("ns3::RedQueueDisc");
	//this `install` below returns a QueueDiscContainer.  Since I'm only installing one queue, I'm just going to grab the first one
	Ptr<QueueDisc> redQueue = (tchBottleneck1.Install(devices5.Get(0))).Get(0);
	Ptr<QueueDisc> redQueue2 = (tchBottleneck1.Install(devices5.Get(1))).Get(0);

	// Ptr<QueueDisc> redQueue = (tchBottleneck.Install(d.GetLeft()->GetDevice(0))).Get(0);
	// tchBottleneck.Install(d.GetRight()->GetDevice(0));
	
	//NOTE:  If I don't do the above with the RedQueueDisc, I'll end up with default queues on the bottleneck link.

	//setup traces
	
	if (redQueue2 == nullptr) {
		std::cout << "it's null!" << std::endl;
	}
		
	redQueue->TraceConnectWithoutContext("Enqueue", MakeCallback(&EnqueueAtRed));
	redQueue->TraceConnectWithoutContext("Dequeue", MakeCallback(&DequeueAtRed));
	redQueue->TraceConnectWithoutContext("Drop", MakeCallback(&DroppedAtRed));

	redQueue2->TraceConnectWithoutContext("Enqueue", MakeCallback(&EnqueueAtRedNew));
	redQueue2->TraceConnectWithoutContext("Dequeue", MakeCallback(&DequeueAtRedNew));
	redQueue2->TraceConnectWithoutContext("Drop", MakeCallback(&DroppedAtRedNew));	

	//ASSIGN IP
	Ipv4AddressHelper address;
	Ipv4AddressHelper address1;
	Ipv4AddressHelper address2;
	Ipv4AddressHelper address3;
	Ipv4AddressHelper address4;
	Ipv4AddressHelper address5;
	Ipv4AddressHelper address6;
	Ipv4AddressHelper address7;
	Ipv4AddressHelper address8;
	Ipv4AddressHelper address9;
	Ipv4AddressHelper address10;
	Ipv4AddressHelper address11;
	Ipv4AddressHelper address12;
	Ipv4AddressHelper address13;
	Ipv4AddressHelper address14;
	Ipv4AddressHelper address15;
	Ipv4AddressHelper address16;
	Ipv4AddressHelper address17;
	address.SetBase("10.1.1.0", "255.255.255.0");
	address1.SetBase("10.2.1.0", "255.255.255.0");
	address2.SetBase("10.3.1.0", "255.255.255.0");
	address3.SetBase("10.4.1.0","255.255.255.0");
	address4.SetBase("10.5.1.0","255.255.255.0");
	address5.SetBase("10.6.1.0","255.255.255.0");
	address6.SetBase("10.7.1.0","255.255.255.0");
	address7.SetBase("10.8.1.0","255.255.255.0");
	address8.SetBase("10.9.1.0","255.255.255.0");
	address9.SetBase("10.10.1.0","255.255.255.0");
	address10.SetBase("10.11.1.0","255.255.255.0");
	address11.SetBase("10.12.1.0","255.255.255.0");
	address12.SetBase("10.13.1.0","255.255.255.0");
	address13.SetBase("10.14.1.0","255.255.255.0");
	address14.SetBase("10.15.1.0","255.255.255.0");
	address15.SetBase("10.16.1.0","255.255.255.0");
	address16.SetBase("10.17.1.0","255.255.255.0");
	address17.SetBase("10.18.1.0","255.255.255.0");

	Ipv4InterfaceContainer interfaces = address.Assign(devices);
	Ipv4InterfaceContainer interfaces1 = address1.Assign(devices2);
	Ipv4InterfaceContainer interfaces2 = address2.Assign(devices3);
	Ipv4InterfaceContainer interfaces3 = address3.Assign(devices4);
	Ipv4InterfaceContainer interfaces4 = address4.Assign(devices5);
	Ipv4InterfaceContainer interfaces5 = address5.Assign(devices6);
	Ipv4InterfaceContainer interfaces6 = address6.Assign(devices7);
	Ipv4InterfaceContainer interfaces7 = address7.Assign(devices8);
	Ipv4InterfaceContainer interfaces8 = address8.Assign(devices9);
	Ipv4InterfaceContainer interfaces9 = address9.Assign(devices10);
	Ipv4InterfaceContainer interfaces10 = address10.Assign(devices11);
	Ipv4InterfaceContainer interfaces11 = address11.Assign(devices12);
	Ipv4InterfaceContainer interfaces12 = address12.Assign(devices13);
	Ipv4InterfaceContainer interfaces13 = address13.Assign(devices14);
	Ipv4InterfaceContainer interfaces14 = address14.Assign(devices15);
	Ipv4InterfaceContainer interfaces15 = address15.Assign(devices16);
	Ipv4InterfaceContainer interfaces16 = address16.Assign(devices17);
	Ipv4InterfaceContainer interfaces17 = address17.Assign(devices18);

	Ipv4GlobalRoutingHelper::PopulateRoutingTables();	

	//APPLICATION

	//Configure Sources
	ApplicationContainer sources;

	//Install Sources
	OnOffHelper sourceHelper("ns3::TcpSocketFactory", Address());
	sourceHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	sourceHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	sourceHelper.SetAttribute("DataRate", DataRateValue(DataRate(leafLinkBW)));		

	//NOTE:  How can you make it so you can determine which flow of traffic a packet belongs to by only 
	//       examinining the TCP header which is what is available when the traces fire?  It's something that
	//		 you set when you setup your applications

	//
	
	//NOTE:  here I'm going to create one source on each leaf node and configure it to send to the gateway.
	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress(InetSocketAddress(interfaces.GetAddress(1), 5000+i*25));
		sourceHelper.SetAttribute("Remote", remoteAddress);

		if (i == 0) { 
			sourceHelper.SetAttribute("PacketSize", UintegerValue(390.625));
		}		
		if (i==1) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(781.25));
		}
		if (i==2) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(2343.75));
		}
		if (i==3) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(3906.25));
		}

		sources.Add(sourceHelper.Install(nodes.Get(i)));
	}
	
	for(u32 i = 6; i < 10; ++i) {
		AddressValue remoteAddress(InetSocketAddress(interfaces10.GetAddress(1), 5000+(i-6)*25));
		sourceHelper.SetAttribute("Remote", remoteAddress);

		if (i == 6) { 
			sourceHelper.SetAttribute("PacketSize", UintegerValue(390.625));
		}		
		if (i==7) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(781.25));
		}
		if (i==8) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(3906.25));
		}
		if (i==9) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(1562.5));
			//sourceHelper.SetAttribute("MaxBytes", UintegerValue(1562.5*400));
		}

		sources.Add(sourceHelper.Install(nodes.Get(i)));
	}
	
	//Configure Sinks

	//NOTE:  Keep in mind I could install multiple sinks on a single node.  
	//HINT:  That will probably be useful for the first experiment if you solve the problem the way I did

	ApplicationContainer sinks;

	Address sinkLocalAddress1(InetSocketAddress(Ipv4Address::GetAny(), 5000));
	Address sinkLocalAddress2(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress3(InetSocketAddress(Ipv4Address::GetAny(), 5050));
	Address sinkLocalAddress4(InetSocketAddress(Ipv4Address::GetAny(), 5075));
	Address sinkLocalAddress5(InetSocketAddress(Ipv4Address::GetAny(), 5001));
	Address sinkLocalAddress6(InetSocketAddress(Ipv4Address::GetAny(), 5026));
	Address sinkLocalAddress7(InetSocketAddress(Ipv4Address::GetAny(), 5051));
	Address sinkLocalAddress8(InetSocketAddress(Ipv4Address::GetAny(), 5076));
	
	Address sinkLocalAddress9(InetSocketAddress(Ipv4Address::GetAny(), 5000));
	Address sinkLocalAddress10(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress11(InetSocketAddress(Ipv4Address::GetAny(), 5050));
	Address sinkLocalAddress12(InetSocketAddress(Ipv4Address::GetAny(), 5075));
	Address sinkLocalAddress13(InetSocketAddress(Ipv4Address::GetAny(), 5001));
	Address sinkLocalAddress14(InetSocketAddress(Ipv4Address::GetAny(), 5026));
	Address sinkLocalAddress15(InetSocketAddress(Ipv4Address::GetAny(), 5051));
	Address sinkLocalAddress16(InetSocketAddress(Ipv4Address::GetAny(), 5076));

	Address sinkLocalAddress17(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress18(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress19(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress20(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress21(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress22(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress23(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	Address sinkLocalAddress24(InetSocketAddress(Ipv4Address::GetAny(), 5025));
	
	PacketSinkHelper sinkHelper1 ("ns3::TcpSocketFactory", sinkLocalAddress1);
	PacketSinkHelper sinkHelper2 ("ns3::TcpSocketFactory", sinkLocalAddress2);	
	PacketSinkHelper sinkHelper3 ("ns3::TcpSocketFactory", sinkLocalAddress3);	
	PacketSinkHelper sinkHelper4 ("ns3::TcpSocketFactory", sinkLocalAddress4);
	PacketSinkHelper sinkHelper5 ("ns3::TcpSocketFactory", sinkLocalAddress5);
	PacketSinkHelper sinkHelper6 ("ns3::TcpSocketFactory", sinkLocalAddress6);	
	PacketSinkHelper sinkHelper7 ("ns3::TcpSocketFactory", sinkLocalAddress7);	
	PacketSinkHelper sinkHelper8 ("ns3::TcpSocketFactory", sinkLocalAddress8);

	PacketSinkHelper sinkHelper9 ("ns3::TcpSocketFactory", sinkLocalAddress17);
	PacketSinkHelper sinkHelper10 ("ns3::TcpSocketFactory", sinkLocalAddress18);	
	PacketSinkHelper sinkHelper11 ("ns3::TcpSocketFactory", sinkLocalAddress19);	
	PacketSinkHelper sinkHelper12 ("ns3::TcpSocketFactory", sinkLocalAddress20);
	PacketSinkHelper sinkHelper13 ("ns3::TcpSocketFactory", sinkLocalAddress21);
	PacketSinkHelper sinkHelper14 ("ns3::TcpSocketFactory", sinkLocalAddress22);	
	PacketSinkHelper sinkHelper15 ("ns3::TcpSocketFactory", sinkLocalAddress23);
	PacketSinkHelper sinkHelper16 ("ns3::TcpSocketFactory", sinkLocalAddress24);
	
	PacketSinkHelper sinkHelper17 ("ns3::TcpSocketFactory", sinkLocalAddress9);
	PacketSinkHelper sinkHelper18 ("ns3::TcpSocketFactory", sinkLocalAddress10);	
	PacketSinkHelper sinkHelper19 ("ns3::TcpSocketFactory", sinkLocalAddress11);	
	PacketSinkHelper sinkHelper20 ("ns3::TcpSocketFactory", sinkLocalAddress12);
	PacketSinkHelper sinkHelper21 ("ns3::TcpSocketFactory", sinkLocalAddress13);
	PacketSinkHelper sinkHelper22 ("ns3::TcpSocketFactory", sinkLocalAddress14);	
	PacketSinkHelper sinkHelper23 ("ns3::TcpSocketFactory", sinkLocalAddress15);	
	PacketSinkHelper sinkHelper24 ("ns3::TcpSocketFactory", sinkLocalAddress16);

	//Install Sinks for traffic from left to rigtht on Gateways A and B
	sinks.Add(sinkHelper1.Install(nodes.Get(4)));
	sinks.Add(sinkHelper2.Install(nodes.Get(4)));
	sinks.Add(sinkHelper3.Install(nodes.Get(4)));
	sinks.Add(sinkHelper4.Install(nodes.Get(4)));
	sinks.Add(sinkHelper5.Install(nodes.Get(5)));
	sinks.Add(sinkHelper6.Install(nodes.Get(5)));
	sinks.Add(sinkHelper7.Install(nodes.Get(5)));
	sinks.Add(sinkHelper8.Install(nodes.Get(5)));

	//Install Sinks for all Nodes that aren't gateways
	sinks.Add(sinkHelper9.Install(nodes.Get(0)));
	sinks.Add(sinkHelper10.Install(nodes.Get(1)));
	sinks.Add(sinkHelper11.Install(nodes.Get(2)));
	sinks.Add(sinkHelper12.Install(nodes.Get(3)));
	sinks.Add(sinkHelper13.Install(nodes.Get(6)));
	sinks.Add(sinkHelper14.Install(nodes.Get(7)));
	sinks.Add(sinkHelper15.Install(nodes.Get(8)));
	sinks.Add(sinkHelper16.Install(nodes.Get(9)));

	//Install Sinks for Gateway B
	sinks.Add(sinkHelper17.Install(nodes.Get(5)));
	sinks.Add(sinkHelper18.Install(nodes.Get(5)));
	sinks.Add(sinkHelper19.Install(nodes.Get(5)));
	sinks.Add(sinkHelper20.Install(nodes.Get(5)));
	sinks.Add(sinkHelper21.Install(nodes.Get(4)));
	sinks.Add(sinkHelper22.Install(nodes.Get(4)));
	sinks.Add(sinkHelper23.Install(nodes.Get(4)));
	sinks.Add(sinkHelper24.Install(nodes.Get(4)));

	
	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress1;
		remoteAddress1.Set(InetSocketAddress(interfaces4.GetAddress(1), 5000+i*25+1));
		sourceHelper.SetAttribute("Remote", remoteAddress1);
		sources.Add(sourceHelper.Install(nodes.Get(4)));
	}

	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress2;
		remoteAddress2.Set(InetSocketAddress(interfaces.GetAddress(1), 5000+i*25+1));
		sourceHelper.SetAttribute("Remote", remoteAddress2);
		sources.Add(sourceHelper.Install(nodes.Get(5)));
	}

	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress3;
		if (i == 0) {
			remoteAddress3.Set(InetSocketAddress(interfaces.GetAddress(0), 5025));
		}
		if (i == 1) {
			remoteAddress3.Set(InetSocketAddress(interfaces1.GetAddress(0), 5025));
		}			
		if (i == 2) {
			remoteAddress3.Set(InetSocketAddress(interfaces2.GetAddress(0), 5025));
		}			
		if (i == 3) {
			remoteAddress3.Set(InetSocketAddress(interfaces3.GetAddress(0), 5025));
		}		
		sourceHelper.SetAttribute("Remote", remoteAddress3);
		sources.Add(sourceHelper.Install(nodes.Get(4)));
	}

	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress4;
		if (i == 0) {
			remoteAddress4.Set(InetSocketAddress(interfaces5.GetAddress(1), 5025));
		}
		if (i == 1) {
			remoteAddress4.Set(InetSocketAddress(interfaces6.GetAddress(1), 5025));
		}			
		if (i == 2) {
			remoteAddress4.Set(InetSocketAddress(interfaces7.GetAddress(1), 5025));
		}			
		if (i == 3) {
			remoteAddress4.Set(InetSocketAddress(interfaces8.GetAddress(1), 5025));
		}		
		sourceHelper.SetAttribute("Remote", remoteAddress4);
		sources.Add(sourceHelper.Install(nodes.Get(5)));
	}

	sources.Get(0)->SetStartTime(Seconds(0));
	sources.Get(1)->SetStartTime(Seconds(0.2));
	sources.Get(2)->SetStartTime(Seconds(0.2));
	sources.Get(3)->SetStartTime(Seconds(0));
	sources.Get(4)->SetStartTime(Seconds(0.2));
	sources.Get(5)->SetStartTime(Seconds(0));
	sources.Get(6)->SetStartTime(Seconds(0.2));
	sources.Get(7)->SetStartTime(Seconds(0.2));
	sources.Get(8)->SetStartTime(Seconds(0));
	sources.Get(9)->SetStartTime(Seconds(0.2));
	sources.Get(10)->SetStartTime(Seconds(0));
	sources.Get(11)->SetStartTime(Seconds(0));
	sources.Get(12)->SetStartTime(Seconds(0.2));
	sources.Get(13)->SetStartTime(Seconds(0));
	sources.Get(14)->SetStartTime(Seconds(0.2));
	sources.Get(15)->SetStartTime(Seconds(0));
	sources.Get(16)->SetStartTime(Seconds(0.2));
	sources.Get(17)->SetStartTime(Seconds(0));
	sources.Get(18)->SetStartTime(Seconds(0.2));
	sources.Get(19)->SetStartTime(Seconds(0));
	sources.Get(20)->SetStartTime(Seconds(0.2));
	sources.Get(21)->SetStartTime(Seconds(0));
	sources.Get(22)->SetStartTime(Seconds(0.2));
	sources.Get(23)->SetStartTime(Seconds(0));
	sinks.Start(Seconds(0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables();

	std::cout << "'Ere we go" << std::endl;

	AnimationInterface* anim = nullptr;
	FlowMonitorHelper flowMonHelper;
	Ptr<FlowMonitor> flowmon;

	if(createVis) {
		anim = new AnimationInterface(animationFile);
		anim->EnablePacketMetadata();
		anim->EnableIpv4L3ProtocolCounters(Seconds(0), Seconds(stopTime));
	}

	if(writeForPlot) {
		filePlotQueue << pathOut << "/" << "redQueue.plot";
		filePlotQueueAvg << pathOut << "/" << "redQueueAvg.plot";
		filePlotQueueDropped << pathOut << "/" << "redQueueDropped.plot";
		filePlotQueueDequeue << pathOut << "/" << "redQueueDequeue.plot";
		filePlotQueueEnqueue << pathOut << "/" << "redQueueEnqueue.plot"; 

		remove(filePlotQueue.str().c_str());
		remove(filePlotQueueAvg.str().c_str());
		remove(filePlotQueueDropped.str().c_str());
		remove(filePlotQueueDequeue.str().c_str());
		remove(filePlotQueueEnqueue.str().c_str());

		filePlotQueue2 << pathOut << "/" << "redQueue2.plot";
		filePlotQueue2Avg << pathOut << "/" << "redQueue2Avg.plot";
		filePlotQueue2Dropped << pathOut << "/" << "redQueue2Dropped.plot";
		filePlotQueue2Dequeue << pathOut << "/" << "redQueue2Dequeue.plot";
		filePlotQueue2Enqueue << pathOut << "/" << "redQueue2Enqueue.plot"; 

		remove(filePlotQueue2.str().c_str());
		remove(filePlotQueue2Avg.str().c_str());
		remove(filePlotQueue2Dropped.str().c_str());
		remove(filePlotQueue2Dequeue.str().c_str());
		remove(filePlotQueue2Enqueue.str().c_str());

		Simulator::ScheduleNow(&CheckQueueSize, redQueue);
		Simulator::ScheduleNow(&CheckQueue2Size, redQueue2);
		
	}

	if(useFlowMon) {
		flowmon = flowMonHelper.InstallAll();	
	}

	Simulator::Stop(Seconds(stopTime));
	Simulator::Run();

	u32 totalBytes = 0;
	
	if(useFlowMon) {
		std::stringstream flowOut;
		flowOut << pathOut << "/" << "red.flowmon";
		remove(flowOut.str().c_str());
		flowmon->SerializeToXmlFile(flowOut.str().c_str(), true, true);
	}

	for(u32 i = 0; i < sinks.GetN(); ++i) {
		Ptr<Application> app = sinks.Get(i);
		Ptr<PacketSink> pktSink = DynamicCast<PacketSink>(app);
		u32 recieved = pktSink->GetTotalRx();
		std::cout << "\tSink\t" << i << "\tBytes\t" << recieved << std::endl;
		totalBytes += recieved;
	}

	std::cout << std::endl << "\tTotal\t\tBytes\t" << totalBytes << std::endl;

	std::cout << "Done" << std::endl;

	if(anim)
		delete anim;

	Simulator::Destroy();
}

