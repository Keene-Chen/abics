/**
 * Author     : KeeneChen
 * DateTime   : 2022.09.21-09:44:43
 * Description: camera
 */

#ifndef __CAMER_H
#define __CAMER_H
#include <asm/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define CLEAN(x) (memset(&(x), 0, sizeof(x)))

#define WIDTH 1280

#define HEIGHT 720

typedef struct Video_Buffer {
    void* start;
    unsigned int length;
} Video_Buffer;

int ioctl_(int fd, int request, void* arg);
void sys_exit(const char* s);
int open_device(const char* device_name);
int open_file(const char* file_name);
void start_stream(void);
void end_stream(void);
int init_device(void);
int init_mmap(void);
static int read_frame(void);
int process_frame(void);
void close_mmap(void);
void close_device(void);

#endif
