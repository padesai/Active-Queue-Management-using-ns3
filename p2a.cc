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


u32 checkTimes;
double avgQueueSize;
u32 port = 5000;

constexpr u32 packetSize = 1000 - 42;



//REMEMBER:  This is not an exact replica of what you need to create.  It just shows something relatively similar.
//NOTE: What it is I do in this example may not be the only way to achieve what you're trying to do for this assignment



void EnqueueAtRed(Ptr<const QueueItem> item) {
	TcpHeader tcp;
	Ptr<Packet> pkt = item->GetPacket();
	pkt->PeekHeader(tcp);
	double sn32 = tcp.GetSequenceNumber().GetValue();
	int pktNumber = sn32/(pkt->GetSize());
	uint16_t destPort = tcp.GetDestinationPort();
	
	std::ofstream fPlotQueueEnqueue(filePlotQueueEnqueue.str().c_str(), std::ios::out | std::ios::app);	

	if (destPort == 5001) {
		fPlotQueueEnqueue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 << std::endl;
	}
	if (destPort == 5026) {
		fPlotQueueEnqueue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 100 << std::endl;
	}
	if (destPort == 5051) {
		fPlotQueueEnqueue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 200 << std::endl;
	} 
	if (destPort == 5076) {
		fPlotQueueEnqueue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 300 << std::endl;
	}		
	
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
	uint16_t destPort = tcp.GetDestinationPort();
	
	std::ofstream fPlotQueueDequeue(filePlotQueueDequeue.str().c_str(), std::ios::out | std::ios::app);	

	if (destPort == 5001) {
		fPlotQueueDequeue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 << std::endl;
	}
	if (destPort == 5026) {
		fPlotQueueDequeue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 100 << std::endl;
	}
	if (destPort == 5051) {
		fPlotQueueDequeue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 200 << std::endl;
	}
	if (destPort == 5076) {
		fPlotQueueDequeue << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 300 << std::endl;
	}		
	
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
	uint16_t destPort = tcp.GetDestinationPort();
	
	std::ofstream fPlotQueueDropped(filePlotQueueDropped.str().c_str(), std::ios::out | std::ios::app);	

	if (destPort == 5001) {
		fPlotQueueDropped << Simulator::Now().GetSeconds() << "," << pktNumber % 90 << std::endl;
	}
	if (destPort == 5026) {
		fPlotQueueDropped << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 100 << std::endl;
	}
	if (destPort == 5051) {
		fPlotQueueDropped << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 200 << std::endl;
	}
	if (destPort == 5076) {
		fPlotQueueDropped << Simulator::Now().GetSeconds() << "," << pktNumber % 90 + 300 << std::endl;
	}		
	
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
	std::string leafLinkDelay1 = "1ms";
	std::string leafLinkDelay4 = "4ms";
	std::string leafLinkDelay5 = "5ms";
	std::string leafLinkDelay8 = "8ms";

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
	Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue(0.002));

	SeedManager::SetSeed(1);
	SeedManager::SetRun(runNumber);

	NodeContainer nodes;
	nodes.Create(6);


	PointToPointHelper bottleneckLink;
	bottleneckLink.SetDeviceAttribute ("DataRate", StringValue(bottleneckLinkBW));
	bottleneckLink.SetChannelAttribute ("Delay", StringValue(bottleneckLinkDelay));

	PointToPointHelper pointToPointLeaf1;
	pointToPointLeaf1.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf1.SetChannelAttribute ("Delay", StringValue(leafLinkDelay1));

	PointToPointHelper pointToPointLeaf2;
	pointToPointLeaf2.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf2.SetChannelAttribute ("Delay", StringValue(leafLinkDelay4));
	
	PointToPointHelper pointToPointLeaf3;
	pointToPointLeaf3.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf3.SetChannelAttribute ("Delay", StringValue(leafLinkDelay8));

	PointToPointHelper pointToPointLeaf4;
	pointToPointLeaf4.SetDeviceAttribute ("DataRate", StringValue(leafLinkBW));
	pointToPointLeaf4.SetChannelAttribute ("Delay", StringValue(leafLinkDelay5));

	
	NetDeviceContainer devices;
    	devices = pointToPointLeaf1.Install(nodes.Get(0), nodes.Get(4));
	
	NetDeviceContainer devices2;
	devices2 = pointToPointLeaf2.Install(nodes.Get(1), nodes.Get(4));

	NetDeviceContainer devices3;
	devices3 = pointToPointLeaf3.Install(nodes.Get(2), nodes.Get(4));
	
	NetDeviceContainer devices4;
	devices4 = pointToPointLeaf4.Install(nodes.Get(3), nodes.Get(4));

	NetDeviceContainer devices5;
	devices5 = bottleneckLink.Install(nodes.Get(4), nodes.Get(5));

	InternetStackHelper stack;
	
	stack.Install(nodes);
	
	TrafficControlHelper tchBottleneck;
	tchBottleneck.SetRootQueueDisc("ns3::RedQueueDisc");
	//this `install` below returns a QueueDiscContainer.  Since I'm only installing one queue, I'm just going to grab the first one
	Ptr<QueueDisc> redQueue = (tchBottleneck.Install(devices5.Get(0))).Get(0);
	tchBottleneck.Install(devices5.Get(1));


		
	// Ptr<QueueDisc> redQueue = (tchBottleneck.Install(d.GetLeft()->GetDevice(0))).Get(0);
	// tchBottleneck.Install(d.GetRight()->GetDevice(0));
	
	//NOTE:  If I don't do the above with the RedQueueDisc, I'll end up with default queues on the bottleneck link.

	//setup traces
	redQueue->TraceConnectWithoutContext("Enqueue", MakeCallback(&EnqueueAtRed));
	redQueue->TraceConnectWithoutContext("Dequeue", MakeCallback(&DequeueAtRed));
	redQueue->TraceConnectWithoutContext("Drop", MakeCallback(&DroppedAtRed));

	//ASSIGN IP
	Ipv4AddressHelper address;
	Ipv4AddressHelper address1;
	Ipv4AddressHelper address2;
	Ipv4AddressHelper address3;
	Ipv4AddressHelper address4;
	address.SetBase( "10.1.1.0", "255.255.255.0");
	address1.SetBase("10.2.1.0", "255.255.255.0");
	address2.SetBase("10.3.1.0", "255.255.255.0");
	address3.SetBase("10.4.1.0", "255.255.255.0");
	address4.SetBase("10.5.1.0","255.255.255.0");

	Ipv4InterfaceContainer interfaces = address.Assign(devices);
	Ipv4InterfaceContainer interfaces1 = address1.Assign(devices2);
	Ipv4InterfaceContainer interfaces2 = address2.Assign(devices3);
	Ipv4InterfaceContainer interfaces3 = address3.Assign(devices4);
	Ipv4InterfaceContainer interfaces4 = address4.Assign(devices5);

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
			sourceHelper.SetAttribute("PacketSize", UintegerValue(52));
		}		
		if (i==1) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(200));
		}
		if (i==2) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(250));
		}
		if (i==3) {
			sourceHelper.SetAttribute("PacketSize", UintegerValue(400));
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
	PacketSinkHelper sinkHelper1 ("ns3::TcpSocketFactory", sinkLocalAddress1);
	PacketSinkHelper sinkHelper2 ("ns3::TcpSocketFactory", sinkLocalAddress2);	
	PacketSinkHelper sinkHelper3 ("ns3::TcpSocketFactory", sinkLocalAddress3);	
	PacketSinkHelper sinkHelper4 ("ns3::TcpSocketFactory", sinkLocalAddress4);
	PacketSinkHelper sinkHelper5 ("ns3::TcpSocketFactory", sinkLocalAddress5);
	PacketSinkHelper sinkHelper6 ("ns3::TcpSocketFactory", sinkLocalAddress6);	
	PacketSinkHelper sinkHelper7 ("ns3::TcpSocketFactory", sinkLocalAddress7);	
	PacketSinkHelper sinkHelper8 ("ns3::TcpSocketFactory", sinkLocalAddress8);


	//Install Sinks
	sinks.Add(sinkHelper1.Install(nodes.Get(4)));
	sinks.Add(sinkHelper2.Install(nodes.Get(4)));
	sinks.Add(sinkHelper3.Install(nodes.Get(4)));
	sinks.Add(sinkHelper4.Install(nodes.Get(4)));
	sinks.Add(sinkHelper5.Install(nodes.Get(5)));
	sinks.Add(sinkHelper6.Install(nodes.Get(5)));
	sinks.Add(sinkHelper7.Install(nodes.Get(5)));
	sinks.Add(sinkHelper8.Install(nodes.Get(5)));
	
	for(u32 i = 0; i < 4; ++i) {
		AddressValue remoteAddress1;
		remoteAddress1.Set(InetSocketAddress(interfaces4.GetAddress(1), 5000+i*25+1));
		sourceHelper.SetAttribute("Remote", remoteAddress1);
		sources.Add(sourceHelper.Install(nodes.Get(4)));
	}

	sources.Get(0)->SetStartTime(Seconds(0));
	sources.Get(1)->SetStartTime(Seconds(0.2));
	sources.Get(2)->SetStartTime(Seconds(0.4));
	sources.Get(3)->SetStartTime(Seconds(0.6));
	sources.Get(4)->SetStartTime(Seconds(0));
	sources.Get(5)->SetStartTime(Seconds(0));
	sources.Get(6)->SetStartTime(Seconds(0));
	sources.Get(7)->SetStartTime(Seconds(0));
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
		filePlotQueue << pathOut << "/" << "redQueuep2a.plot";
		filePlotQueueAvg << pathOut << "/" << "redQueueAvgp2a.plot";
		filePlotQueueDropped << pathOut << "/" << "redQueueDroppedp2a.plot";
		filePlotQueueDequeue << pathOut << "/" << "redQueueDequeuep2a.plot";
		filePlotQueueEnqueue << pathOut << "/" << "redQueueEnqueuep2a.plot"; 

		remove(filePlotQueue.str().c_str());
		remove(filePlotQueueAvg.str().c_str());
		remove(filePlotQueueDropped.str().c_str());
		remove(filePlotQueueDequeue.str().c_str());
		remove(filePlotQueueEnqueue.str().c_str());

		Simulator::ScheduleNow(&CheckQueueSize, redQueue);
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

