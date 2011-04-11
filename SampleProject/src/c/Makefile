CCLINA8=~/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MMD -MP
LINKLINA8=~/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-g++ -shared


LINOBJ=build/LinuxImp.o build/v4jCommon.o build/v4l2uvc.o 
LININCLUDE=-I"./include" -I/usr/src/linux-headers-2.6.32-26-generic/include/ -I/usr/lib/jvm/java-6-openjdk/include/

CCLIN32=gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -m32 -MMD
LINKLIN32=g++ -m32 -shared 


CCLIN64=gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -m64 -MMD
LINKLIN64=g++ -m64 -shared 

dirs:
	if (test -d build); then rm -rf build;mkdir build;else mkdir build; fi
	if (test -d resources/native);\
	 then echo dirs ok;else\
	 mkdir -p resources/native/linux/x86_32/;\
	 mkdir -p resources/native/linux/x86_64/;\
	 mkdir -p resources/native/linux/ARM/; \
	 mkdir -p resources/native/linux/ARM_A8/; \
	 mkdir -p resources/native/linux/ARM_ANDROID/; \
	 mkdir -p resources/native/windows/x86_32/; \
	 mkdir -p resources/native/windows/x86_64/; \
	 mkdir -p resources/native/osx/; \
	 fi

linux:dirs

	$(CCLIN32) src/v4jCommon.c -o build/v4jCommon.o
	$(CCLIN32) src/linux/LinuxImp.c -o build/LinuxImp.o
	$(CCLIN32) src/linux/v4l2uvc.c -o build/v4l2uvc.o
	#$(CCLIN32) src/linux/jpeg_utils.c -o build/jpeg_utils.o
	$(LINKLIN32) -ljpeg -o"jni/native/linux/x86_32/libV4J.so" $(LINOBJ)
	rm build/*
	
	$(CCLINA8) src/v4jCommon.c -o build/v4jCommon.o
	$(CCLINA8) src/linux/LinuxImp.c -o build/LinuxImp.o
	$(CCLINA8) src/linux/v4l2uvc.c -o build/v4l2uvc.o
	$(LINKLINA8) -o"jni/native/linux/ARM_A8/libV4J.so" $(LINOBJ)
	rm  build/*
	
	$(CCLIN64) src/v4jCommon.c -o build/v4jCommon.o
	$(CCLIN64) src/linux/LinuxImp.c -o build/LinuxImp.o
	$(CCLIN64) src/linux/v4l2uvc.c -o build/v4l2uvc.o
	#$(CCLIN64) src/linux/jpeg_utils.c -o build/jpeg_utils.o
	$(LINKLIN64) -ljpeg -o"jni/native/linux/x86_64/libV4J.so" $(LINOBJ)
	rm -rf build