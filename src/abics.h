#ifndef __ABICS_H__
#define __ABICS_H__

#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "camera.h"
#include "mqtt_config.h"
#include "mqtt_log.h"
#include "mqttclient.h"
#include "serial_port.h"

#define LEN 5
int serial_fd;
bool flag[LEN] = { false, false, false, false, true };

char open_light[LEN]  = { 0xdd, 0x05, 0x24, 0x00, 0x00 };
char close_light[LEN] = { 0xdd, 0x05, 0x24, 0x00, 0x01 };
char open_alarm[LEN]  = { 0xdd, 0x05, 0x24, 0x00, 0x02 };
char close_alarm[LEN] = { 0xdd, 0x05, 0x24, 0x00, 0x03 };
char open_fan[LEN]    = { 0xdd, 0x05, 0x24, 0x00, 0x04 };
char close_fan[LEN]   = { 0xdd, 0x05, 0x24, 0x00, 0x08 };
char open_door[LEN]   = { 0xdd, 0x05, 0x24, 0x00, 0x09 };
char close_door[LEN]  = { 0xdd, 0x05, 0x24, 0x00, 0x0a };

#define CMD 0xdd, 0x05, 0x24, 0x00
char cmd[][LEN] = { { CMD, 0x00 }, { CMD, 0x01 }, { CMD, 0x02 }, { CMD, 0x03 }, { CMD, 0x04 }, { CMD, 0x08 }, { CMD, 0x09 }, { CMD, 0x0a } };

void abics_mqtt_back(void* client, const char* topic, char* str);
void abics_get_image(void);
void abics_push_image(void);

#endif // __ABICS_H__