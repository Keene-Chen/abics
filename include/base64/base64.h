#ifndef __BASE64_H__
#define __BASE64_H__

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* base64_encode(const unsigned char* bindata, int binlength, char* base64);
int base64_decode(const char* base64, unsigned char* bindata);
#endif // __BASE64_H__