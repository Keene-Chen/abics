### 视频推流
ffmpeg -f v4l2 -framerate 30 -video_size 1280x720 -i /dev/video1 -vcodec libx264 -acodec libfaac -f flv rtmp://192.168.10.9:1935/myapp/a

### 编译参数
clang -std=gnu11 abics.c ../include/serial_port/serial_port.c ../include/camera/camera.c -I../include/libmqttclient -I../include/serial_port -I../include/camera -lmqttclient -L../lib/libmqttclient.so -lpthread -o abics && ./abics.out