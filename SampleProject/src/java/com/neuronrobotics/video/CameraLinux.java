package com.neuronrobotics.video;

import java.io.File;
import java.util.HashSet;
import java.util.Set;

public class CameraLinux implements ICameraOSImplmentation{

	@Override
	public Set<String> getAvailibleDevices() {
		Set<String> s = new HashSet<String>();
		File dir = new File("/dev/");
		String[] devices = dir.list();
		for(int i=0;i<devices.length;i++){
			if(devices[i].contains("video"))
				s.add("/dev/"+devices[i]);
		}
		return s;
	}

}
