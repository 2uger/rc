#pragma once

#define EXAMPLE_ESP_WIFI_SSID "CIT"
#define EXAMPLE_ESP_WIFI_PASS "0296574742"
#define EXAMPLE_ESP_MAXIMUM_RETRY 10

#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void wifi_init_sta(void);
