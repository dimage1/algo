#include <signal.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>

//Setup memory buffers for memory pool.
#define NUM_MBUFS (4096-1)

#define BURST_SIZE	32

// setup NIC number
static int gDpdkPortId = 0;
static uint16_t listenUdpPort = 7777;
static struct rte_mempool *mbufPool;

//
static const struct rte_eth_conf port_conf_default = {
	.rxmode = {.mtu= 1420 }
};

//initiate the NIC with allocated memory pool.
static void init_port(struct rte_mempool *mbuf_pool) {

	// check if NIC has been bound with DPDK
	uint16_t nb_sys_ports= rte_eth_dev_count_avail(); 
	if (nb_sys_ports == 0) {
		rte_exit(EXIT_FAILURE, "No Supported eth found\n");
	}

	//NIC info struct
	struct rte_eth_dev_info dev_info;
    mbufPool = mbuf_pool;

	// Get raw NIC info via a function provided by DPDK
	rte_eth_dev_info_get(gDpdkPortId, &dev_info); 
	printf("Dev info\n driver_name : %s\n if_index: %d\n mtu: %d %d\n Queue: %d %d\n", dev_info.driver_name,dev_info.if_index,dev_info.max_mtu,dev_info.min_mtu,dev_info.nb_rx_queues,dev_info.nb_tx_queues);


	// Setuo rx & tx queue for NIC
	const int num_rx_queues = 1;
	const int num_tx_queues = 1;

	//NIC configuration struct
	struct rte_eth_conf port_conf = port_conf_default;
	//Configure the NIC with queue parameters
	rte_eth_dev_configure(gDpdkPortId, num_rx_queues, num_tx_queues, &port_conf);


	//1024 is uint16_t nb_rx_desc , it stands for the length of the rx queue. 1024 packets
	if (rte_eth_rx_queue_setup(gDpdkPortId, 0 , 1024, 
		//When receiving data from interface, the data will be stored at this mbuf_pool
		rte_eth_dev_socket_id(gDpdkPortId),NULL, mbuf_pool) < 0) {
		rte_exit(EXIT_FAILURE, "Could not setup RX queue\n");
	}

	//set up configuration for tx
	struct rte_eth_txconf tx_conf = dev_info.default_txconf;
	//set up tx offload which is size of data transfer
	tx_conf.offloads = port_conf_default.rxmode.offloads;
	printf("Offload: %d\n", (int)tx_conf.offloads);
	if (rte_eth_tx_queue_setup(gDpdkPortId, 0 , 1024, 
		//TX queue
		rte_eth_dev_socket_id(gDpdkPortId), &tx_conf) < 0) {
		rte_exit(EXIT_FAILURE, "Could not setup TX queue\n");
	}

	//Start the NIC
	if (rte_eth_dev_start(gDpdkPortId) < 0 ) {
		rte_exit(EXIT_FAILURE, "Could not start\n");
	}
}

int e_init(void)
{
    /* 
    RunTime Environment RTE
    Environment Abstraction Layer EAL
    initiate the environment. EAL is a interface that connect application and hardware.
    verify hugepages
    */
    const char *args[] = {"app"};
    	if (rte_eal_init(1, (char**)args) < 0) {
		rte_exit(EXIT_FAILURE, "Error with EAL init\n");
		
	}
    /* 
    Memory pool. It is used to store data received on the NIC.
    one process -> on pool
    Create memory pool by call rte_pktmbuf_pool_create
    */
	struct rte_mempool *mbuf_pool = rte_pktmbuf_pool_create("mbuf pool", NUM_MBUFS,
		0, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
	if (mbuf_pool == NULL) {
		rte_exit(EXIT_FAILURE, "Could not create mbuf pool\n");
	}

    //initiate the NIC
	init_port(mbuf_pool);

    return 0;
}

int e_socket_send(ESocket& socket, const EAddress* address, const EBuffer * buffers, size_t bufferCount)
{
    if (bufferCount == 0 || !buffers)
    {
        return 0;
    }

    struct sockaddr_in dest;
    e_address_to_sockaddr(*address, dest);

    // Allocate one mbuf for the packet (will be freed when effectively sent)
    struct rte_mbuf *pkt = rte_pktmbuf_alloc(mbufPool);
    if (!pkt) {
        errno = ENOMEM;
        return -1;
    }
        // Initialize the Ethernet header
    struct rte_ether_hdr *eth_hdr = rte_pktmbuf_mtod(pkt, struct rte_ether_hdr *);
    static const uint8_t dstMac[] = {0x42, 0x01, 0x0a, 0x00, 0x00, 0x06};
    static const uint8_t srcMac[] = {0x42, 0x01, 0x0a, 0x00, 0x00, 0x05};
    rte_memcpy(eth_hdr->src_addr.addr_bytes, srcMac, RTE_ETHER_ADDR_LEN);
    rte_memcpy(eth_hdr->dst_addr.addr_bytes, dstMac, RTE_ETHER_ADDR_LEN);
    //rte_ether_addr_copy(srcMac, &eth_hdr->src_addr);
    //rte_ether_addr_copy(dstMac, &eth_hdr->dst_addr);
    eth_hdr->ether_type = rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4);

    // Initialize the IP header
    struct rte_ipv4_hdr *ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);
    memset(ip_hdr, 0, sizeof(*ip_hdr));
    ip_hdr->version_ihl = 0x45; // TODO: no magic
    ip_hdr->type_of_service = 0;
    ip_hdr->fragment_offset = 0;
    ip_hdr->time_to_live = 64; // TODO: no magic
    ip_hdr->next_proto_id = IPPROTO_UDP;
    ip_hdr->packet_id = 0;

    const uint32_t len = buffers[0].dataLength;
    const void *data = buffers[0].data;

    ip_hdr->src_addr = E_NET_TO_HOST_32(0x0A000005);

    ip_hdr->dst_addr = dest.sin_addr.s_addr;
    ip_hdr->total_length = rte_cpu_to_be_16(len + sizeof(*ip_hdr) + sizeof(rte_udp_hdr));
    ip_hdr->hdr_checksum = rte_ipv4_cksum(ip_hdr);

    // Initialize the UDP header
    struct rte_udp_hdr *udp_hdr = (struct rte_udp_hdr *)(ip_hdr + 1);
    udp_hdr->src_port = rte_cpu_to_be_16(listenUdpPort);
    udp_hdr->dst_port = dest.sin_port;
    udp_hdr->dgram_cksum = 0;   // UDP checksum is optional
    udp_hdr->dgram_len = rte_cpu_to_be_16(len + sizeof(*udp_hdr));

    // Fill other DPDK metadata
    pkt->packet_type = RTE_PTYPE_L2_ETHER | RTE_PTYPE_L3_IPV4 | RTE_PTYPE_L4_UDP;
    pkt->pkt_len = len + sizeof(*eth_hdr) + sizeof(*ip_hdr) + sizeof(*udp_hdr);
    pkt->data_len = pkt->pkt_len;
    pkt->l2_len = sizeof(struct rte_ether_hdr);
    pkt->l3_len = sizeof(struct rte_ipv4_hdr);
    pkt->l4_len = sizeof(struct rte_udp_hdr);

    // Write payload
    void *udp_data = (void *)(udp_hdr + 1);
    rte_memcpy(udp_data, data, len);

     rte_eth_tx_burst(gDpdkPortId, 0, &pkt, 1);
	rte_pktmbuf_free(pkt);
    return len;
}

//**********************************************************************************************************************
int e_socket_receive(ESocket& socket, EAddress* address, EBuffer * buffer)
{
     struct rte_mbuf *mbufs[1];
    // get data from interface / queue / memory buffer / receive size
    //mbufs -> mbuf_pool in setup rx 
    unsigned num_recvd = rte_eth_rx_burst(gDpdkPortId, 0, mbufs, 1);
    if (num_recvd > 1) {
        rte_exit(EXIT_FAILURE, "Error receiving from eth\n");
    }
    if (num_recvd != 1) {
        return -1;
    }

    
    // Convert the buffer into Ethernet header
    struct rte_ether_hdr *ethhdr = rte_pktmbuf_mtod(mbufs[0], struct rte_ether_hdr*);
    
    // if the next header is not IPv4.
    if (ethhdr->ether_type != rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4)) {
        return -1;
    }

    /* Get next IP header.
    The offset means start from offset. Not the lenght.
    */
    struct rte_ipv4_hdr *iphdr =  rte_pktmbuf_mtod_offset(mbufs[0], struct rte_ipv4_hdr *, 
        sizeof(struct rte_ether_hdr));

    /* Get next ICMP header.
    The offset means start from offset. Not the lenght.
    */
    if (iphdr->next_proto_id != IPPROTO_UDP ){
        return -1;
    }


    struct sockaddr_in sin;
	uint32_t src_ip;
	uint32_t dst_ip;
    uint16_t dst_port;

    struct rte_udp_hdr *udpHdr = (struct rte_udp_hdr *)(iphdr + 1);
    rte_memcpy(&sin.sin_addr, &iphdr->src_addr, sizeof(uint32_t));
    rte_memcpy(&dst_ip, &iphdr->src_addr, sizeof(uint32_t));
    rte_memcpy(&sin.sin_port, &udpHdr->src_port, sizeof(uint16_t));
    rte_memcpy(&dst_port, &udpHdr->dst_port, sizeof(uint16_t));
    uint32_t recvLength = E_NET_TO_HOST_16(udpHdr->dgram_len) - sizeof(rte_udp_hdr);


    if (listenUdpPort == E_NET_TO_HOST_16(dst_port) && (int)recvLength > 0 && buffer && (recvLength <= buffer->dataLength))
    {
        rte_memcpy(buffer->data, (udpHdr + 1), recvLength);
        if (address != nullptr)
        {
            e_address_from_sockaddr(sin, *address);
        }
    }
    else
    {
        recvLength = 0;
    }
    rte_pktmbuf_free(mbufs[0]);

    return recvLength;
}