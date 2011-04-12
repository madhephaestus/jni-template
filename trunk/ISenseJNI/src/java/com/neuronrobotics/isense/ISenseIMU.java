package com.neuronrobotics.isense;

public class ISenseIMU {
	static{
		NativeResource.getInstance();
	}
	private static native int ISDOpenTracker(int commPort,boolean verbose);
	public ISenseIMU(int commPort,boolean verbose){
		NativeResource.getInstance();
		int back =ISDOpenTracker(commPort,verbose);
		System.out.println("Initialized..."+back);
	}
}
