#include "myJNI.h"


JNIEXPORT void JNICALL Java_com_neuronrobotics_isense_NativeResource_nativeTest( JNIEnv *env,jobject jobj ){
	printf("Test OK");
}


JNIEXPORT jint JNICALL Java_com_neuronrobotics_isense_ISenseIMU_ISDOpenTracker( JNIEnv *env,jobject jobj, jint commPort,jboolean verbose){
	int handle = ISD_OpenTracker( (Hwnd)NULL, commPort, FALSE, verbose);
	return handle;
}
