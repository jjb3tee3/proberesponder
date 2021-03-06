/* 
	Author: jake
	Notes:
		o Setup context
		o Capture
		o Inject
		o Context cleanup
*/
#include "proberesponder.h"
#include <time.h>

ap_info_t ap_info;

void usage(char *argv[]) {
	printf("Usage:\n");
	printf("\t%s\n", argv[0]);
}

/*void send_probe_response() {
	lcpa_metapack_t *metapack;
	lorcon_packet_t *txpacket;

	metapack = lcpa_init();
	lcpf_proberesp(metapack, ap_info.dst_mac, ap_info.src_mac, ap_info.bssid, 0x00, 0x00, 0x00, 0x00, ap_info.
}*/

int is_probe_req(lorcon_packet_t *packet) {
	int ret, i;
	char ssid[256];
	char mac[MAC_LEN];
	time_t ltime;

	if((packet->packet_header[0] == 0x40) && (packet->packet_header[PSSID_OFFSET-1] < 255) && (packet->packet_header[PSSID_OFFSET-1] != 0)) {
		for(i=0; i<packet->packet_header[PSSID_OFFSET-1]; i++) {
			ssid[i] = packet->packet_header[PSSID_OFFSET + i];
		}
		ltime = time(NULL);
		printf("[!] <%s> Probe from ", asctime(localtime(&ltime)));
		// Mac address..?
		for(i=1; i<=MAC_LEN;i++)
		{
		    printf("%02x", packet->packet_header[SRC_MAC_OFFSET+i]);
		    
		    if(i < MAC_LEN)
			printf(":");
		}
		
		ssid[packet->packet_header[PSSID_OFFSET-1]] = '\0';
		printf(" SSID: %s\n", ssid);	
		/*! 
			This currently reports all probe packets it
			finds. Uncommenting will enable the -f filter
			option which will only report matches on those
			packets where the SSID matches the criteria.
		!*/
		// ret = strncmp(ssid, ap_info.ssid, packet->packet_header[PSSID_OFFSET-1]);

		// if(ret == 0) {
			//  Future: Grab mac to make sure we can filter results...
			ap_info.got_probe_req = 1;
			return 1;
		//}
	}

	return 0;
}

void probe_listen(lorcon_t *context, lorcon_packet_t *packet, u_char *user) {
	if(is_probe_req(packet) && ap_info.got_probe_req == 1) {
	//	printf("[+] Detected probe request!\n");
	//	printf("[+] Probe SSID: %s\n", ap_info.ssid);
	//	ap_info.got_probe_req = 0;
	}
}

int main(int argc, char **argv) {
	char *iface;
	int c, ret;

	pthread_t beacon_thread;

	lorcon_t *context;
	lorcon_driver_t *dlist, *driver;
	
	ap_info.src_mac = "\x00\xDE\xAD\xBE\xEF\x00";
	ap_info.bssid = "\x00\xDE\xAD\xBE\xEF\x00";

	ap_info.ssid = NULL;
	ap_info.got_probe_req = 0;
	ap_info.got_assoc_req = 0;
	ap_info.got_auth_req = 0;

	printf("[!] Running ProbeResponder\n");
		
	while((c = getopt(argc, argv, "i:s:hc:b:")) != EOF) {
		switch(c) {
			case 'i':
				iface = strdup(optarg);
				printf("[*] Interface: \"%s\"\n", iface);
				break;
			case 's':
				if(strlen(strdup(optarg)) < 255) {
					ap_info.ssid = strdup(optarg);
					ap_info.ssid_len = strlen(ap_info.ssid);
					printf("[*] SSID: \"%s\"\n", ap_info.ssid);
				} else {
					printf("[!] ERROR: SSID length > 255");
					return -1;
				}
				break;
			case 'c':
				ap_info.channel = atoi(optarg);
				printf("[*] Channel: %d\n", ap_info.channel);
				break;
			case 'h':
				usage(argv);
				break;
			default:
				usage(argv);
				break;
		}
	}	
	
	if(iface == NULL || ap_info.ssid == NULL) {
		printf("[!] ERROR: Interface, channel and SSID must be set.");
		return -1;
	}

	if((driver = lorcon_auto_driver(iface)) == NULL) {
		printf("[!] Could not detect driver for %s\n", iface);
		return -1;
	} else {
		printf("[*] Driver: \"%s\"\n", driver->name);
	}

	if((context = lorcon_create(iface, driver)) == NULL) {
		printf("[!] Failed to create context");
		return -1;
	}

	if(lorcon_open_injmon(context) < 0) {
		printf("[!] Could not create injection/monitor mode interfce.\n");
		lorcon_free_driver_list(driver);
	}	

	lorcon_set_channel(context, ap_info.channel);
	
	printf("[!] Sniffing for probe requests.\n");

	lorcon_loop(context, 0, probe_listen, NULL);

	lorcon_close(context);
	
	lorcon_free(context);

	return 0;
} 
