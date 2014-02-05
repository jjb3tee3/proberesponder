#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/time.h>

#include <lorcon2/lorcon.h>
#include <lorcon2/lorcon_packasm.h>

/* Field positions in 802.11 packets */
#define MAC_LEN 6
#define SRC_MAC_OFFSET 9
#define DST_MAC_OFFSET 3
#define BSSID_OFFSET 15

#define SSID_LEN_OFFSET 37
#define SSID_OFFSET 38

#define PSSID_OFFSET 26
#define PSRC_OFFSET 10

#define TYPE_OFFSET 0

/* Virtual Access Point information */
struct AP_INFO {
	char *ssid;
	uint8_t ssid_len;

	uint8_t channel;

	uint8_t *src_mac;
	uint8_t *dst_mac;
	uint8_t *bssid;

	int beacon_int;
	int capabilities;
	uint8_t *rates;

	struct timeval time;
	uint64_t timestamp;
	
	int got_probe_req;
	int got_assoc_req;
	int got_auth_req;

};

typedef struct AP_INFO ap_info_t;
