package test;

import java.awt.Frame;
import java.awt.Image;
import java.util.Set;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;


import com.neuronrobotics.video.Camera;

public class v4jTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try{
			System.out.println("Starting...");
			
			Set<String> devices = Camera.getAvailibleDevices();
			System.out.println("Devices: "+devices);
			for(String dev:devices){
				try{
					Camera c = new Camera(dev,320,240);
					Image im;
					JPanel directPanel = new JPanel();
					JFrame frame = new JFrame();
					frame.add(directPanel);
					frame.setLocationRelativeTo(null);
					frame.setVisible(true);
					for(int i=0;i<50;i++){
						im=c.getImage();
						if(im!= null){
							directPanel.removeAll();
							JLabel l = new JLabel();
							l.setIcon(new ImageIcon(im));
							directPanel.add(l);
							directPanel.invalidate();
							frame.pack();
						}else{
							System.err.println("Image is null");
						}
					}
					c.close();
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}catch(Exception ex){
			System.err.println("Failed!");
			ex.printStackTrace();
		}
		System.exit(0);
		
	}

}
