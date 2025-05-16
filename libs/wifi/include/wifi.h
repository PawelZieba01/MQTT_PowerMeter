#ifndef __WIFI_H__
#define __WIFI_H__

#include <stdbool.h>
#include <stddef.h>

void wifi_init_sta(void);
void wifi_stop(void);
void wifi_start(void);
bool wifi_is_connected(void);
bool wifi_get_ip_str(char *buf, size_t buflen);

#endif // __WIFI_H__