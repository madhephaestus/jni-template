package test;

import com.neuronrobotics.isense.ISenseIMU;
import com.neuronrobotics.isense.NativeResource;

public class SampleJNITest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Starting...");
		new ISenseIMU(0,false);
		System.out.println("Loaded!");
	}

}
