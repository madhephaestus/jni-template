package com.neuronrobotics.isense;

public class ISenseIMU {
	
	static{
		NativeResource.getInstance();
	}
	
	private float[] isdTrackerData;
	
	private static native int ISDOpenTracker(int commPort,boolean verbose);
	private static native float[] ISDGetTrackingData(int handle);

	public ISenseIMU(int commPort,boolean verbose){
		NativeResource.getInstance();
		int back =ISDOpenTracker(commPort,verbose);
		System.out.println("Initialized..." + back);
		while(true){
			isdTrackerData = ISDGetTrackingData(back);
			printTrackerData(isdTrackerData);
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private void printTrackerData(float[] trackerData){
		System.out.print("\nAz: " + trackerData[0]);
		System.out.print(" E1: " + trackerData[1]);
		System.out.print(" R1: " + trackerData[2]);
		System.out.print(" X: " + trackerData[3]);
		System.out.print(" Y: " + trackerData[4]);
		System.out.print(" Z: " + trackerData[5]);
		System.out.print(" Compass: " + trackerData[6]);
		System.out.print(" TimeStamp: " + trackerData[7]);
		
	}
}
