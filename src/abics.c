#include "abics.h"

void abics_mqtt_back(void* client, const char* topic, char* str)
{
    mqtt_message_t msg = { 0 };
    msg.payload        = str;
    mqtt_publish(client, topic, &msg);
}

void abics_get_image(void)
{
    int ret = open_device("/dev/video0");
    if (ret == -1)
        exit(EXIT_FAILURE);
    open_file("../res/out.jpeg");
    init_device();
    init_mmap();
    start_stream();
    process_frame();
    end_stream();
    close_mmap();
    close_device();
}

void abics_push_image(void)
{
    system("scp -r ../res root@hello-chen.cn:/www");
}

static void iot_handler(void* client, message_data_t* msg)
{
    (void)client;
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char*)msg->message->payload);
    // 自动报警
    char buf[3] = { 0 };
    strncpy(buf, (char*)msg->message->payload + 8, 2);
    int num = atoi(buf);
    if (num > 50 && flag[4] == true) {
        flag[4] = false;
        abics_mqtt_back(client, "back", "oa");
    }
    else {
        if (flag[4] == false)
            abics_mqtt_back(client, "back", "ca");
        flag[4] = true;
    }
}

static void cmd_handler(void* client, message_data_t* msg)
{
    (void)client;
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s\n", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char*)msg->message->payload);
    switch (*(char*)msg->message->payload) {
    case 'l': {
        if (flag[0]) {
            //开灯
            serial_port_send(serial_fd, open_light, LEN);
            abics_mqtt_back(client, "back", "open_light");
        }
        else {
            //关灯
            serial_port_send(serial_fd, close_light, LEN);
            abics_mqtt_back(client, "back", "close_light");
        }
        flag[0] = !flag[0];
    } break;
    case 'f': {
        if (flag[1]) {
            serial_port_send(serial_fd, open_fan, LEN);
            abics_mqtt_back(client, "back", "open_fan");
        }
        else {
            serial_port_send(serial_fd, close_fan, LEN);
            abics_mqtt_back(client, "back", "close_fan");
        }
        flag[1] = !flag[1];
    } break;
    case 'd': {
        if (flag[2]) {
            serial_port_send(serial_fd, open_door, LEN);
            abics_mqtt_back(client, "back", "open_door");
        }
        else {
            serial_port_send(serial_fd, close_door, LEN);
            abics_mqtt_back(client, "back", "close_door");
        }
        flag[2] = !flag[2];
    } break;
    case 'a': {
        if (flag[3]) {
            serial_port_send(serial_fd, open_alarm, LEN);
            abics_mqtt_back(client, "back", "open_alarm");
        }
        else {
            serial_port_send(serial_fd, close_alarm, LEN);
            abics_mqtt_back(client, "back", "close_alarm");
        }
        flag[3] = !flag[3];
    } break;
    case 'c': {
        abics_mqtt_back(client, "back", "connected");
    } break;
    case 'i': {
        abics_get_image();
        abics_push_image();
    } break;
    default:
        break;
    }
}

void* mqtt_publish_thread(void* arg)
{
    char buf[64]          = { 0 };
    char recv_buf[36]     = { 0 };
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;

    msg.payload = (void*)buf;

    while (1) {
        memset(recv_buf, 0, sizeof(recv_buf));
        int len = serial_port_recv(serial_fd, recv_buf, 36);
        if (len > 0) {
            sprintf(buf, "{\"temp\":%d.%d,\"humi\":%d.%d,\"light\":%d.%d}", recv_buf[5], random_number_range(1, 99), recv_buf[7], random_number_range(1, 99), recv_buf[20], random_number_range(1, 99));
            msg.qos = 0;
            mqtt_publish(client, "iot", &msg);
        }
        else {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "cannot receive data");
            msg.qos = 0;
            mqtt_publish(client, "iot", &msg);
            MQTT_LOG_E("cannot receive data");
        }
        sleep(1);
    }
}

int main(void)
{
    // 日志初始化
    mqtt_log_init();
    MQTT_LOG_I("welcome to abics...");

    serial_fd = serial_port_open(serial_fd, "/dev/ttyS0");
    serial_port_init(serial_fd, 115200, 0, 8, 1, 'N');
    serial_port_send(serial_fd, open_fan, 36);
    MQTT_LOG_I("serial_port init success");

    // 申请mqtt客户端
    mqtt_client_t* client = NULL;
    client                = mqtt_lease();

    // 客户端配置
    char client_id[32];
    char user_name[32];
    char password[32];
    sprintf(client_id, "abics_%s", random_string(6));
    sprintf(user_name, "abics_%s", random_string(6));
    sprintf(password, "abics_%s", random_string(6));
    mqtt_set_port(client, PORT);
    mqtt_set_host(client, HOST);
    mqtt_set_client_id(client, client_id);
    mqtt_set_user_name(client, user_name);
    mqtt_set_password(client, password);
    mqtt_set_clean_session(client, 1);

    // 连接服务器
    mqtt_connect(client);

    // 订阅主题,使用回调函数处理消息
    mqtt_subscribe(client, "iot", QOS2, iot_handler);
    mqtt_subscribe(client, "cmd", QOS2, cmd_handler);
    mqtt_subscribe(client, "back", QOS2, NULL);
    sleep(1);
    mqtt_list_subscribe_topic(client);

    // 客户端上线发送消息
    mqtt_message_t msg = { 0 };
    msg.payload        = "connected";
    msg.qos            = 1;
    mqtt_publish(client, "back", &msg);

    // 创建客户端发送线程
    pthread_t thread1;
    int res = pthread_create(&thread1, NULL, mqtt_publish_thread, client);
    if (res != 0) {
        MQTT_LOG_E("create mqtt publish thread fail");
        exit(res);
    }

    while (1) {
        sleep(100);
    }

    // 释放资源
    mqtt_release(client);
    serial_port_close(serial_fd);
}
