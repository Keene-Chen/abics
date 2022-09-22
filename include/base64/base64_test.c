#include "base64.h"

int main()
{

    //打开图片
    FILE* fp = NULL;
    unsigned int size; //图片字节数
    char* buffer;
    char* buffer1;
    size_t result;
    char* ret1;
    unsigned int length;

    fp = fopen("out.mjpeg", "rb");
    if (NULL == fp) {
        printf("open_error");
        exit(1);
    }

    //获取图片大小
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //分配内存存储整个图片
    buffer = (char*)malloc((size / 4 + 1) * 4);
    if (NULL == buffer) {
        printf("memory_error");
        exit(2);
    }

    //将图片拷贝到buffer
    result = fread(buffer, 1, size, fp);
    if (result != size) {
        printf("reading_error");
        exit(3);
    }
    fclose(fp);

    // base64编码
    buffer1 = (char*)malloc((size / 4 + 1) * 4);
    if (NULL == buffer1) {
        printf("memory_error");
        exit(2);
    }
    ret1                 = base64_encode(buffer, size, buffer1);
    length               = strlen(buffer1);

    // const char* filepath = "./1.log";
    // FILE* fd             = fopen(filepath, "w+");
    // if (fd == NULL) {
    //     printf("failed to open file\n");
    //     return -1;
    // }
    // // fprintf(fd, "%s", buffer1);
    // fclose(fd);

    // free(buffer);
    free(buffer1);

    return 0;
}