#include "v4j.h"

/*----------------------------------------------------------
 report_warning

   accept:      string to send to report as an message
   perform:     send the string to stderr or however it needs to be reported.
   return:      none
   exceptions:  none
   comments:
----------------------------------------------------------*/
void report_warning(char *msg)
{
	fprintf(stderr, "%s", msg);
}

/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
int memcpy_picture(unsigned char *out, unsigned char *buf, int size)
{
    unsigned char *ptcur = buf;
    memcpy(out, ptcur, size);
    return size;
}



JNIEXPORT void JNICALL NativeResource(nativeTest)( JNIEnv *env,jobject jobj,jstring jstr ){
//a test to see if the lib is loaded
}


/**
 * Bind a native camera
 */
JNIEXPORT jboolean JNICALL Camera(nativeBind)( JNIEnv *env,jobject jobj,jstring name, jint hight, jint width){
	jboolean iscopy;
	const char *mfile = (*env)->GetStringUTFChars(env, name, &iscopy);
	//report_warning("\nAttempting to initialize: ");
	//report_warning((char *)mfile);
	return initializeVideo(mfile, (int)hight,(int) width);
}

/**
 * Bind a native camera
 */
JNIEXPORT jboolean JNICALL Camera(nativeClose)( JNIEnv *env,jobject jobj,jstring name){
	jboolean iscopy;
	const char *mfile = (*env)->GetStringUTFChars(env, name, &iscopy);
	//report_warning("\nAttempting to initialize: ");
	//report_warning((char *)mfile);
	return closeVideo(mfile);
}

/**
 * get an image as an array of ints
 */
JNIEXPORT jbyteArray JNICALL Camera(nativeGetImage)( JNIEnv *env,jobject jobj,jstring jstr ){
	//report_warning("\nAttempting to get image");
	int size = captureImage();
	fprintf(stderr, "Image Size= %d \n", size);
	if (size == 0)
		return NULL;
	char array[size];
	size=getImage(array);
	jbyteArray image = (*env)->NewByteArray(env,size);
	(*env)->SetByteArrayRegion(env, image, 0,size, (jbyte *)array);
	return image;
}
