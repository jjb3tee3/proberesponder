/* 
	Author: jake
	Notes:
		o Setup context
		o Capture
		o Inject
		o Context cleanup
*/
#include "proberesponder.h"

ap_info_t ap_info;

void usage(char *argv[]) {
	printf("Usage:\n");
	printf("\t%s\n", argv[0]);
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

	//lorcon_loop(context, 0, probe_state, NULL);

	lorcon_close(context);
	
	lorcon_free(context);

	return 0;
} 
