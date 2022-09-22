/**
 * Author     : KeeneChen
 * DateTime   : 2022.09.21-09:46:10
 * Description: camera_test
 */

#include "camera.h"
#include <stdio.h>

#define DEVICE_NAME "/dev/video1"

#define FILE_NAME "./out.mjpeg"
int main(int argc, char* argv[])
{
    int ret = open_device(DEVICE_NAME);
    if (ret == -1)
        exit(EXIT_FAILURE);
    open_file(FILE_NAME);
    init_device();
    init_mmap();
    start_stream();

    for (int i = 0; i < 400; i++) {
        process_frame();
        printf("frame:%d\n", i);
    }

    end_stream();
    close_mmap();
    close_device();

    return 0;
}
