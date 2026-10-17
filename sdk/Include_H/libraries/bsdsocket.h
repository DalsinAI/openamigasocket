#ifndef LIBRARIES_BSDSOCKET_H
#define LIBRARIES_BSDSOCKET_H
/*
 * OpenSocket SDK: bsdsocket.library's tags, codes and structures.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif
#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif
#ifndef SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifndef NETINET_IN_H
#include <netinet/in.h>
#endif
#ifndef NET_IF_H
#include <net/if.h>
#endif

/* SocketBaseTagList()'s tags: TAG_USER, then whether the tag passes a
 * value or a reference, its code, and whether it gets or sets */
#define SBTF_VAL                0x0
#define SBTF_REF                0x8000
#define SBTB_CODE               0x1
#define SBTS_CODE               0x3FFF
#define SBTF_GET                0x0
#define SBTF_SET                0x1
#define SBTM_CODE(td)		(((td) >> SBTB_CODE) & SBTS_CODE)
#define SBTM_GETREF(code)	(TAG_USER | SBTF_REF | (((code) & SBTS_CODE) << SBTB_CODE) | SBTF_GET)
#define SBTM_GETVAL(code)	(TAG_USER | SBTF_VAL | (((code) & SBTS_CODE) << SBTB_CODE) | SBTF_GET)
#define SBTM_SETREF(code)	(TAG_USER | SBTF_REF | (((code) & SBTS_CODE) << SBTB_CODE) | SBTF_SET)
#define SBTM_SETVAL(code)	(TAG_USER | SBTF_VAL | (((code) & SBTS_CODE) << SBTB_CODE) | SBTF_SET)

/* the codes */
#define SBTC_BREAKMASK                    1
#define SBTC_SIGIOMASK                    2
#define SBTC_SIGURGMASK                   3
#define SBTC_SIGEVENTMASK                 4
#define SBTC_ERRNO                        6
#define SBTC_HERRNO                       7
#define SBTC_DTABLESIZE                   8
#define SBTC_FDCALLBACK                   9
#define SBTC_LOGSTAT                      10
#define SBTC_LOGTAGPTR                    11
#define SBTC_LOGFACILITY                  12
#define SBTC_LOGMASK                      13
#define SBTC_ERRNOSTRPTR                  14
#define SBTC_HERRNOSTRPTR                 15
#define SBTC_IOERRNOSTRPTR                16
#define SBTC_S2ERRNOSTRPTR                17
#define SBTC_S2WERRNOSTRPTR               18
#define SBTC_ERRNOBYTEPTR                 21
#define SBTC_ERRNOWORDPTR                 22
#define SBTC_ERRNOLONGPTR                 24
#define SBTC_HERRNOLONGPTR                25
#define SBTC_RELEASESTRPTR                29
#define SBTC_COMPAT43			SBTC_RELEASESTRPTR
/* the errno pointer code for an errno variable of a given size */
#define SBTC_ERRNOPTR(size)	((size) == sizeof(LONG) ? SBTC_ERRNOLONGPTR : \
				 (size) == sizeof(WORD) ? SBTC_ERRNOWORDPTR : \
				 (size) == sizeof(BYTE) ? SBTC_ERRNOBYTEPTR : 0)
#define SBTC_NUM_PACKET_FILTER_CHANNELS   40
#define SBTC_HAVE_ROUTING_API             41
#define SBTC_UDP_CHECKSUM                 42
#define SBTC_IP_FORWARDING                43
#define SBTC_IP_DEFAULT_TTL               44
#define SBTC_ICMP_MASK_REPLY              45
#define SBTC_ICMP_SEND_REDIRECTS          46
#define SBTC_HAVE_INTERFACE_API           47
#define SBTC_ICMP_PROCESS_ECHO            48
#define SBTC_ICMP_PROCESS_TSTAMP          49
#define SBTC_HAVE_MONITORING_API          50
#define SBTC_CAN_SHARE_LIBRARY_BASES      51
#define SBTC_LOG_FILE_NAME                52
#define SBTC_HAVE_STATUS_API              53
#define SBTC_HAVE_DNS_API                 54
#define SBTC_LOG_HOOK                     55
#define SBTC_SYSTEM_STATUS                56
#define SBTC_SIG_ADDRESS_CHANGE_MASK      57
#define SBTC_IPF_API_VERSION              58
#define SBTC_HAVE_LOCAL_DATABASE_API      59
#define SBTC_HAVE_ADDRESS_CONVERSION_API  60
#define SBTC_HAVE_KERNEL_MEMORY_API       61
#define SBTC_IP_FILTER_HOOK               62
#define SBTC_HAVE_SERVER_API              63
#define SBTC_GET_BYTES_RECEIVED           64
#define SBTC_GET_BYTES_SENT               65
#define SBTC_IDN_DEFAULT_CHARACTER_SET    66
#define SBTC_HAVE_ROADSHOWDATA_API        67
#define SBTC_ERROR_HOOK                   68
#define SBTC_HAVE_GETHOSTADDR_R_API       69

/* SBTC_FDCALLBACK's actions */
#define FDCB_FREE               0
#define FDCB_ALLOC              1
#define FDCB_CHECK              2

/* SBTC_IDN_DEFAULT_CHARACTER_SET's values */
#define IDNCS_ASCII             0
#define IDNCS_ISO_8859_LATIN_1  1

/* SBTC_SYSTEM_STATUS's bits */
#define SBSYSSTAT_Interfaces    0x1
#define SBSYSSTAT_PTP_Interfaces 0x2
#define SBSYSSTAT_BCast_Interfaces 0x4
#define SBSYSSTAT_Resolver      0x8
#define SBSYSSTAT_Routes        0x10
#define SBSYSSTAT_DefaultRoute  0x20

/* an IP filter hook's answers */
#define IR_Process              0
#define IR_Ignore               1
#define IR_Drop                 2

/* ObtainSocket()'s "any id" */
#define UNIQUE_ID               (-1)

/* the socket events SO_EVENTMASK asks for */
#define FD_ACCEPT               0x1
#define FD_CONNECT              0x2
#define FD_OOB                  0x4
#define FD_READ                 0x8
#define FD_WRITE                0x10
#define FD_ERROR                0x20
#define FD_CLOSE                0x40

/* the routing calls' tags */
#define RTA_BASE                0x80000640UL
#define RTA_Destination         0x80000641UL
#define RTA_Gateway             0x80000642UL
#define RTA_DefaultGateway      0x80000643UL
#define RTA_DestinationHost     0x80000644UL
#define RTA_DestinationNet      0x80000645UL

/* AddInterfaceTagList()'s tags and their values */
#define IFA_BASE                0x800006A4UL
#define IFA_IPType              0x800006A5UL
#define IFA_ARPType             0x800006A6UL
#define IFA_NumReadRequests     0x800006A7UL
#define IFA_NumWriteRequests    0x800006A8UL
#define IFA_NumARPRequests      0x800006A9UL
#define IFA_PacketFilterMode    0x800006AAUL
#define IFA_PointToPoint        0x800006ABUL
#define IFA_Reserved0           0x800006ACUL
#define IFA_Multicast           0x800006ADUL
#define IFA_DownGoesOffline     0x800006AEUL
#define IFA_ReportOffline       0x800006AFUL
#define IFA_RequiresInitDelay   0x800006B0UL
#define IFA_CopyMode            0x800006B1UL
#define IFA_HardwareAddress     0x800006B2UL
#define IFA_SetDebugMode        0x800006B3UL
#define IFA_LimitMTU            0x800006B4UL
#define PFM_Nothing             0
#define PFM_Local               1
#define PFM_IPandARP            2
#define PFM_Everything          3
#define CM_SlowWordCopy         0
#define CM_FastWordCopy         1

/* ConfigureInterfaceTagList()'s tags */
#define IFC_BASE                0x80000708UL
#define IFC_Address             0x80000709UL
#define IFC_NetMask             0x8000070AUL
#define IFC_DestinationAddress  0x8000070BUL
#define IFC_BroadcastAddress    0x8000070CUL
#define IFC_Metric              0x8000070DUL
#define IFC_AddAliasAddress     0x8000070EUL
#define IFC_DeleteAliasAddress  0x8000070FUL
#define IFC_State               0x80000710UL
#define IFC_GetPeerAddress      0x80000711UL
#define IFC_GetDNS              0x80000712UL
#define IFC_AssociatedRoute     0x80000714UL
#define IFC_AssociatedDNS       0x80000715UL
#define IFC_ReleaseAddress      0x80000716UL
#define IFC_SetDebugMode        0x80000717UL
#define IFC_Complete            0x80000718UL
#define IFC_LimitMTU            0x80000719UL

/* QueryInterfaceTagList()'s tags */
#define IFQ_BASE                      0x8000076CUL
#define IFQ_DeviceName                0x8000076DUL
#define IFQ_DeviceUnit                0x8000076EUL
#define IFQ_HardwareAddressSize       0x8000076FUL
#define IFQ_HardwareAddress           0x80000770UL
#define IFQ_MTU                       0x80000771UL
#define IFQ_BPS                       0x80000772UL
#define IFQ_HardwareType              0x80000773UL
#define IFQ_PacketsReceived           0x80000774UL
#define IFQ_PacketsSent               0x80000775UL
#define IFQ_BadData                   0x80000776UL
#define IFQ_Overruns                  0x80000777UL
#define IFQ_UnknownTypes              0x80000778UL
#define IFQ_LastStart                 0x80000779UL
#define IFQ_Address                   0x8000077AUL
#define IFQ_DestinationAddress        0x8000077BUL
#define IFQ_BroadcastAddress          0x8000077CUL
#define IFQ_NetMask                   0x8000077DUL
#define IFQ_Metric                    0x8000077EUL
#define IFQ_State                     0x8000077FUL
#define IFQ_AddressBindType           0x80000780UL
#define IFQ_AddressLeaseExpires       0x80000781UL
#define IFQ_PrimaryDNSAddress         0x80000782UL
#define IFQ_SecondaryDNSAddress       0x80000783UL
#define IFQ_NumReadRequests           0x80000784UL
#define IFQ_MaxReadRequests           0x80000785UL
#define IFQ_NumWriteRequests          0x80000786UL
#define IFQ_MaxWriteRequests          0x80000787UL
#define IFQ_GetBytesIn                0x80000788UL
#define IFQ_GetBytesOut               0x80000789UL
#define IFQ_GetDebugMode              0x8000078AUL
#define IFQ_GetSANA2CopyStats         0x8000078BUL
#define IFQ_NumReadRequestsPending    0x8000078CUL
#define IFQ_NumWriteRequestsPending   0x8000078DUL
#define IFQ_HardwareMTU               0x8000078EUL
#define IFQ_OutputDrops               0x8000078FUL
#define IFQ_InputDrops                0x80000790UL
#define IFQ_OutputErrors              0x80000791UL
#define IFQ_InputErrors               0x80000792UL
#define IFQ_OutputMulticasts          0x80000793UL
#define IFQ_InputMulticasts           0x80000794UL
#define IFQ_IPDrops                   0x80000795UL
#define IFQ_ARPDrops                  0x80000796UL

/* IFQ_AddressBindType's and IFQ_State's values */
#define IFABT_Unknown           0
#define IFABT_Static            1
#define IFABT_Dynamic           2
#define SM_Offline              0
#define SM_Online               1
#define SM_Down                 2
#define SM_Up                   3

/* IFQ_GetSANA2CopyStats's answer */
struct SANA2CopyStats
{
    ULONG s2cs_DMAIn;
    ULONG s2cs_DMAOut;
    ULONG s2cs_ByteIn;
    ULONG s2cs_ByteOut;
    ULONG s2cs_WordOut;
};

/* network monitor hooks: the message types and answers */
#define MHT_ICMP                0
#define MHT_UDP                 1
#define MHT_TCP_Connect         2
#define MHT_Connect             3
#define MHT_Send                4
#define MHT_Packet              5
#define MHT_Bind                6
#define PMMD_Receive            0
#define PMMD_Send               1
#define MA_Continue             0
#define MA_Ignore               (-1)
#define MA_Drop                 (-2)
#define MA_DropWithReset        (-3)

/* GetNetworkStatistics()'s version and kinds */
#define NETWORKSTATUS_VERSION   1
#define NETSTATUS_icmp          0
#define NETSTATUS_igmp          1
#define NETSTATUS_ip            2
#define NETSTATUS_mb            3
#define NETSTATUS_mrt           4
#define NETSTATUS_rt            5
#define NETSTATUS_tcp           6
#define NETSTATUS_udp           7
#define NETSTATUS_tcp_sockets   9
#define NETSTATUS_udp_sockets   10

/* one connection, as NETSTATUS_tcp_sockets and NETSTATUS_udp_sockets list it */
struct protocol_connection_data
{
    struct in_addr pcd_foreign_address;
    UWORD pcd_foreign_port;
    struct in_addr pcd_local_address;
    UWORD pcd_local_port;
    ULONG pcd_receive_queue_size;
    ULONG pcd_send_queue_size;
    LONG pcd_tcp_state;
};

/* address allocation (BOOTP, DHCP, automatic) */
#define AAM_VERSION                   2
#define AAM_VERSION_MINIMUM           1
#define AAMR_Success                  0
#define AAMR_Aborted                  1
#define AAMR_InterfaceNotKnown        2
#define AAMR_InterfaceWrongType       3
#define AAMR_AddressKnown             4
#define AAMR_VersionUnknown           5
#define AAMR_NoMemory                 6
#define AAMR_Timeout                  7
#define AAMR_AddressInUse             8
#define AAMR_AddrChangeFailed         9
#define AAMR_MaskChangeFailed         10
#define AAMR_Busy                     11
#define AAMR_Ignored                  (-1)
#define AAM_TIMEOUT_MIN               10
#define DHCP_DEFAULT_LEASE_TIME       0
#define DHCP_INFINITE_LEASE_TIME      0xFFFFFFFFUL
#define AAMP_BOOTP                    0
#define AAMP_DHCP                     1
#define AAMP_SLOWAUTO                 2
#define AAMP_FASTAUTO                 3
#define CAAMTA_BASE                   0x800007D0UL
#define CAAMTA_Timeout                0x800007D1UL
#define CAAMTA_LeaseTime              0x800007D2UL
#define CAAMTA_RequestedAddress       0x800007D3UL
#define CAAMTA_ClientIdentifier       0x800007D4UL
#define CAAMTA_NAKMessageSize         0x800007D5UL
#define CAAMTA_RouterTableSize        0x800007D6UL
#define CAAMTA_DNSTableSize           0x800007D7UL
#define CAAMTA_StaticRouteTableSize   0x800007D8UL
#define CAAMTA_HostNameSize           0x800007D9UL
#define CAAMTA_DomainNameSize         0x800007DAUL
#define CAAMTA_BOOTPMessageSize       0x800007DBUL
#define CAAMTA_RecordLeaseExpiration  0x800007DCUL
#define CAAMTA_ReplyPort              0x800007DDUL
#define CAAMTA_RequestUnicast         0x800007DEUL
#define CAAME_Success                     0
#define CAAME_Invalid_result_ptr          1
#define CAAME_Not_enough_memory           2
#define CAAME_Invalid_version             3
#define CAAME_Invalid_protocol            4
#define CAAME_Invalid_interface_name      5
#define CAAME_Interface_not_found         6
#define CAAME_Invalid_client_identifier   7
#define CAAME_Client_identifier_too_short 8
#define CAAME_Client_identifier_too_long  9

/* interface monitoring directions */
#define IFMD_Incoming           0
#define IFMD_Outgoing           1

/* the network controller: its port, commands and errors */
#define NETWORK_CONTROLLER_PORT_NAME	"TCP/IP Control"
#define NSMC_Shutdown           1
#define NSMC_Cancel             2
#define NSME_Success            0
#define NSME_Aborted            1
#define NSME_InUse              2
#define NSME_Ignored            3
#define NSME_NotFound           4
#define NCPM_Cookie             0x20040306

/* the Roadshow data calls */
#define RDNT_Integer            0
#define RDNF_ReadOnly           1
#define ORD_ReadAccess          0
#define ORD_WriteAccess         1

/* SBTC_ERROR_HOOK's actions */
#define EHMA_Set_errno          1
#define EHMA_Set_h_errno        2

/* one name server, as ObtainDomainNameServerList() lists them */
struct DomainNameServerNode
{
    struct MinNode dnsn_MinNode;
    LONG dnsn_Size;			/* this node's size */
    STRPTR dnsn_Address;		/* the address as text */
    LONG dnsn_UseCount;
};

/* structures some calls take pointers to, whose layout is not here */
struct rt_msghdr;			/* the routing calls' messages */
struct mbuf;				/* the kernel memory calls' buffers */
struct AddressAllocationMessage;	/* CreateAddrAllocMessageA()'s message */

#endif /* LIBRARIES_BSDSOCKET_H */
