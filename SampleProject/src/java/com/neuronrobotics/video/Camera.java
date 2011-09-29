package com.neuronrobotics.video;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.Set;

import javax.imageio.ImageIO;

public class Camera {
	private static final String mjpg="MJPG";
	private static final String yuyv="YUYV";
	static ICameraOSImplmentation imp = null;
	private static native byte [] nativeGetImage();
	private static native boolean nativeBind(String device, int hight, int width);
	private static native boolean nativeClose(String device);
	private static String dev;
	static{
		NativeResource.getInstance();
		if(imp == null){
			if(OSUtil.isLinux())
				imp = new CameraLinux();
			if(OSUtil.isWindows()){
				imp = new CameraWindows();
				throw new NativeResourceException("OS not yet supported");	
			}
			if(OSUtil.isOSX()){
				imp = new CameraOSX();
				throw new NativeResourceException("OS not yet supported");	
			}
		}
	}
	
	public Camera(String device, int hight, int width){
		if(OSUtil.isWindows() || OSUtil.isOSX()){
			throw new NativeResourceException("OS not yet supported");	
		}
		nativeClose(device);
		Set<String> s = getAvailibleDevices();
		boolean ok=false;
		for(String str:s){
			if(str.contains(device))
				ok=true;
		}
		if(!ok)
			throw new RuntimeException("Device not availible: "+device);
		if(!nativeBind( device, hight, width)){
			throw new RuntimeException("Failed to bind "+device);
		}
		dev=device;
	}
	public byte [] getImageArray(){
		return nativeGetImage();
	}
	public BufferedImage getImage(){
		byte [] byteArray = nativeGetImage();
		if(byteArray == null)
			return null;
		try {
			return ByteArrayToImage(byteArray);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}
	}
	static public Set<String> getAvailibleDevices(){
		
		return imp.getAvailibleDevices();
	}
	public static BufferedImage ByteArrayToImage(byte [] array) throws IOException{
		//System.out.println("Image: "+new ByteList(array));
		BufferedImage image = null;
		image = ImageIO.read(new ByteArrayInputStream(array));
		return image;
	}
	public void close() {
		nativeClose(dev);
	}
}
