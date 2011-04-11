#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <jni.h>
#define NativeResource(foo) Java_com_neuronrobotics_video_NativeResource_ ## foo
#define Camera(foo) Java_com_neuronrobotics_video_Camera_ ## foo

void report_warning(char *msg);

int initializeVideo(const char * device, int hight, int width);

int closeVideo(const char * device);

int  captureImage();

int getImage(char * imageArray);


int memcpy_picture(unsigned char *out, unsigned char *buf, int size);
