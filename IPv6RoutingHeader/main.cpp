/*
* IPv6RoutingHeader
*
* Create/Parse packets with an IPv6 Routing Extension Header
* */
#include <iostream>
#include <string>
#include <crafter.h>

using namespace std;
using namespace Crafter;

void test_packet(const char *test_title, Packet &packet) {
    cout << "Testing: " << test_title << endl;
    packet.PreCraft();
    cout << "Original packet:" << endl;
    packet.Print();
    packet.HexDump(cout);
    cout << endl;

    /* Decode it */
    Packet decoded;
    decoded.Decode(packet.GetRawPtr(), packet.GetSize(), IPv6::PROTO);
    cout << "Decoded packet:" << endl;
    decoded.Print();
    decoded.HexDump(cout);
    cout << endl;

    cout << endl;
}

int main() {
    /* Create an IP header */
    IPv6 ip_header;
    /* Set the Source and Destination IP address */
    ip_header.SetSourceIP("2001:db8:dead:beef:cafe::0");
    ip_header.SetDestinationIP("2001:db8:1234::1");
    /* Create a segment routing header */
    IPv6SegmentRoutingHeader sr_header;
    sr_header.PushIPv6Segment("2001:db8:1234::2");
    sr_header.PushIPv6Segment("2001:db8:1234::3");
    sr_header.PushIPv6Segment("2001:db8:1234::4");
    sr_header.PushIPv6Segment("2001:db8:1234::5");
    /* Setting a policy in a _nice_ way */
    sr_header.SetPolicy(3, "dead:beef::", IPv6SegmentRoutingHeader::POLICY_EGRESS);
    /* Or by acessing to the byte arrays directly */
    sr_header.SetPolicyFlag1(IPv6SegmentRoutingHeader::POLICY_SOURCE_ADDRESS);
    sr_header.PolicyList[0][2] = 0xff;
    sr_header.SetPFlag(1);
    sr_header.SetHMACKeyID(5);
    sr_header.HMAC[15] = 0xff;

    /* Dummy TCP header */
    TCP tcp_header;
    /* Dummy Payload */
    RawLayer payload("Hello World!");

    /* Create a packet... */
    Packet packet = ip_header / sr_header / tcp_header / payload;
    test_packet("Segment Routing header", packet);

    /* Same test for the mobile routing header */
    IPv6MobileRoutingHeader mr_header;
    mr_header.SetHomeAddress("2001:db8::1");

    packet = ip_header / mr_header / tcp_header / payload;
    test_packet("Mobile Routing header", packet);

    return 0;
}
