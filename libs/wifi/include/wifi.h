#ifndef __WIFI_H__
#define __WIFI_H__

#include <stdbool.h>

void wifi_init_sta(void);
void wifi_stop(void);
void wifi_start(void);
bool wifi_is_connected(void);

#endif // __WIFI_H__