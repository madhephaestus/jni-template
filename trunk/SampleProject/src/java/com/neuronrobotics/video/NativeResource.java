package com.neuronrobotics.video;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class NativeResource {
	private static native void nativeTest();
	private NativeResource(){
		load("libV4J");
	}
	private static NativeResource instance = null;
	public static NativeResource getInstance() throws NativeResourceException {
		if(!OSUtil.isLinux())
			throw new NativeResourceException("OS not yet supported");	
		if(instance == null) {
			instance = new NativeResource();
		}		
		return instance;
	}
	private boolean loaded = false;
	private synchronized void load(String libraryName) throws NativeResourceException {	
		if(loaded)
			return;
		loaded = true;
		if(System.getProperty(libraryName + ".userlib") != null) {
			try {
				if(System.getProperty(libraryName + ".userlib").equalsIgnoreCase("sys")) {
					System.loadLibrary(libraryName);
				} else {
					System.load(System.getProperty(libraryName + ".userlib"));
				}
				return;
			} catch (Exception e){
				e.printStackTrace();
				throw new NativeResourceException("Unable to load native resource from given path.\n" + e.getLocalizedMessage());
			}
		}
		loadLib(libraryName);	
	}
	
	private void inJarLoad(String name)throws UnsatisfiedLinkError, NativeResourceException{
		//start by assuming the library can be loaded from the jar
		InputStream resourceSource = locateResource(name);
		File resourceLocation = prepResourceLocation(name);
		try {
			copyResource(resourceSource, resourceLocation);
		} catch (IOException e) {
			throw new UnsatisfiedLinkError();
		}
		loadResource(resourceLocation);
		testNativeCode();
	}

	private void loadLib(String name) throws NativeResourceException {
		String libName = name.substring(name.indexOf("lib")+3);
		inJarLoad(name);
	}
	
	private void testNativeCode()throws UnsatisfiedLinkError {
		nativeTest();
	}

	private InputStream locateResource(String name) {
		name += OSUtil.getExtension();
		String file="";
		if(OSUtil.isOSX()) {
			//file="/native/osx/" + name;
			throw new NativeResourceException("OSX is not supported");
		}else if(OSUtil.isWindows()) {
//			if(OSUtil.is64Bit()){
//				file="/native/windows/x86_64/" + name;
//			}else {
//				file="/native/windows/x86_32/" + name;
//			}
			throw new NativeResourceException("Windows is not supported");
		}else if(OSUtil.isLinux()) {
			if(OSUtil.isARM()) {
				if(OSUtil.isCortexA8())
					file = "/native/linux/ARM_A8/" + name;
				else
					file = "/native/linux/ARM/" + name;
			}else {
				if(OSUtil.is64Bit()) {
					file="/native/linux/x86_64/" + name;
				}else {
					file="/native/linux/x86_32/" + name;
				}
			}
		}else{
			System.err.println("Can't load native file: "+name+" for os arch: "+OSUtil.getOsArch());
			return null;
		}
		//System.out.println("Loading "+file);
		return getClass().getResourceAsStream(file);
	}
	
	private void loadResource(File resource) {
		if(!resource.canRead())
			throw new RuntimeException("Cant open JNI file: "+resource.getAbsolutePath());
		//System.out.println("Loading: "+resource.getAbsolutePath());
		System.load(resource.getAbsolutePath());
	}

	private void copyResource(InputStream io, File file) throws IOException {
		FileOutputStream fos = new FileOutputStream(file);
		
		byte[] buf = new byte[256];
		int read = 0;
		while ((read = io.read(buf)) > 0) {
			fos.write(buf, 0, read);
		}
		fos.close();
		io.close();
	}

	private File prepResourceLocation(String fileName) throws NativeResourceException {		
		String tmpDir = System.getProperty("java.io.tmpdir");
		if ((tmpDir == null) || (tmpDir.length() == 0)) {
			tmpDir = "tmp";
		}
		
		String displayName = new File(fileName).getName().split("\\.")[0];
		
		String user = System.getProperty("user.name");
		
		File fd = null;
		File dir = null;
		
		for(int i = 0; i < 10; i++) {
			dir = new File(tmpDir, displayName + "_" + user + "_" + (i));
			if (dir.exists()) {
				if (!dir.isDirectory()) {
					continue;
				}
				
				try {
					File[] files = dir.listFiles();
					for (int j = 0; j < files.length; j++) {
						if (!files[j].delete()) {
							continue;
						}
					}
				} catch (Throwable e) {
					
				}
			}
			
			if ((!dir.exists()) && (!dir.mkdirs())) {
				continue;
			}
			
			try {
				dir.deleteOnExit();
			} catch (Throwable e) {
				// Java 1.1 or J9
			}
			
			fd = new File(dir, fileName + OSUtil.getExtension());
			if ((fd.exists()) && (!fd.delete())) {
				continue;
			}
			
			try {
				if (!fd.createNewFile()) {
					continue;
				}
			} catch (IOException e) {
				continue;
			} catch (Throwable e) {
				// Java 1.1 or J9
			}
			
			break;
		}
		
		if(fd == null || !fd.canRead()) {
			throw new NativeResourceException("Unable to deploy native resource");
		}
		//System.out.println("Local file: "+fd.getAbsolutePath());
		return fd;
	}
}
