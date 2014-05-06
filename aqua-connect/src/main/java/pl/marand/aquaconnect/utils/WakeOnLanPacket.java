package pl.marand.aquaconnect.utils;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class WakeOnLanPacket {
	
	public static final int PORT = 9;
	
	public static boolean send(byte[] macAddress, InetAddress ipAddress){
		try {
			byte[] bytes = new byte[6 + 16 * macAddress.length];
	        for (int i = 0; i < 6; i++) {
	            bytes[i] = (byte) 0xff;
	        }
	        for (int i = 6; i < bytes.length; i += macAddress.length) {
	            System.arraycopy(macAddress, 0, bytes, i, macAddress.length);
	        }
	        
	        DatagramPacket packet = new DatagramPacket(bytes, bytes.length, ipAddress, PORT);
	        DatagramSocket socket = new DatagramSocket();
	        socket.send(packet);
	        socket.close();
	        
	        return true;
	    }
	    catch (Exception e) {
	        return false;
	    }
	}
}
