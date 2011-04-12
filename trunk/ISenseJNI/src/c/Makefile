CCLINA8=~/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MMD -MP
LINKLINA8=~/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-g++ -shared


LINOBJ=build/isenseJNI.o build/isense.o build/main.o
LININCLUDE=-I"./include" -I"./include/isense" -I/usr/src/linux-headers-2.6.32-26-generic/include/ -I/usr/lib/jvm/java-6-openjdk/include/

CCLIN32=gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -m32 -MMD
LINKLIN32=g++ -m32 -shared


CCLIN64=gcc $(LININCLUDE) -O3 -Wall -c -fmessage-length=0 -fPIC -m64 -MMD
LINKLIN64=g++ -m64  

dirs:
	if (test -d build); then rm -rf build;mkdir build;else mkdir build; fi
	if (test -d jni/native);\
	 then echo dirs ok;else\
	 mkdir -p jni/native/linux/x86_32/;\
	 mkdir -p jni/native/linux/x86_64/;\
	 mkdir -p jni/native/linux/ARM_A8/; \
	 mkdir -p jni/native/windows/x86_32/; \
	 mkdir -p jni/native/windows/x86_64/; \
	 mkdir -p jni/native/osx/; \
	 fi

linux:dirs
	$(CCLIN32) src/isenseJNI.c -o build/isenseJNI.o
	$(CCLIN32) src/libCode/main.c -o build/main.o
	$(CCLIN32) src/libCode/isense.c -o build/isense.o
	$(LINKLIN32) -o"jni/native/linux/x86_32/libISenseJNI.so" $(LINOBJ) 
	rm build/*
