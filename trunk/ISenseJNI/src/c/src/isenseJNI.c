#include "myJNI.h"


JNIEXPORT void JNICALL Java_com_neuronrobotics_isense_NativeResource_nativeTest( JNIEnv *env,jobject jobj ){
	printf("Test OK");
}


JNIEXPORT jint JNICALL Java_com_neuronrobotics_isense_ISenseIMU_ISDOpenTracker( JNIEnv *env,jobject jobj, jint commPort,jboolean verbose){
	int handle = ISD_OpenTracker((Hwnd)NULL, commPort, FALSE, verbose);
	return handle;
}

JNIEXPORT jfloatArray JNICALL Java_com_neuronrobotics_isense_ISenseIMU_ISDGetTrackingData( JNIEnv *env,jobject jobj, jint handle){
	ISD_TRACKING_DATA_TYPE data;
	float dataTemp[8];
	jfloatArray dataJ =(*env)->NewFloatArray(env, 8);  //Array the the function returns with Euler(3), Pos(3), Magnetic(1), Timestamp(1)
	//jbyteArray image = (*env)->NewByteArray(env,size);
	//(*env)->SetByteArrayRegion(env, image, 0,size, (jbyte *)array);

	ISD_GetTrackingData(handle, &data);
	//Get the data wanted
	dataTemp[0] = data.Station[0].Euler[0];
	dataTemp[1] = data.Station[0].Euler[1];
	dataTemp[2] = data.Station[0].Euler[2];
	dataTemp[3] = data.Station[0].Position[0];
	dataTemp[4] = data.Station[0].Position[1];
	dataTemp[5] = data.Station[0].Position[2];
	dataTemp[6] = data.Station[0].CompassYaw;
	dataTemp[7] = data.Station[0].TimeStamp;

	(*env)->SetFloatArrayRegion(env, dataJ, 0, 8, (jfloat *)dataTemp);

	return dataJ;
}


