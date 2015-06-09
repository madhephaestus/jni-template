
#depends on sudo apt-get install libjpeg62 build-essential g++-multilib libv4l-dev

all:linux
	ant
	
ubuntu:
	sudo apt-get install libjpeg62 build-essential g++-multilib libv4l-dev libjpeg-dev
	#sudo ln -s /usr/include/libv4l1-videodev.h   /usr/include/linux/videodev.h
linux:ubuntu
	make -C src/c linux
	ant
