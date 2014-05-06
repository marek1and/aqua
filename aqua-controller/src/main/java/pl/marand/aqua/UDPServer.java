package pl.marand.aqua;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.concurrent.ArrayBlockingQueue;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import pl.marand.aquaconnect.exceptions.DeviceException;

public class UDPServer extends Thread {
	
	private static final Logger logger = LoggerFactory.getLogger(UDPServer.class);
	
	private DatagramSocket serverSocket;
	private int port;
	private boolean running;
	
	private ArrayBlockingQueue<DatagramPacket> packets;
	
	public UDPServer(int port, ArrayBlockingQueue<DatagramPacket> packets) {
		this.port = port;
		this.packets = packets;
	}
	
	public boolean isRunning() {
		return running;
	}
	
	private DatagramPacket readPacket() {
		byte[] receiveData = new byte[1024];
		DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		try {
			serverSocket.receive(receivePacket);
		} catch (IOException ex) {
			if(isRunning()){
				logger.warn("Problem with receiving UDP packet. {}", ex.getMessage());
			}
			return null;
		}
		return receivePacket;
	}
		
	public void sendResult(InetAddress ip, int port, byte[] data) {
		DatagramPacket sendPacket = new DatagramPacket(data, data.length, ip, port);
		try {
			serverSocket.send(sendPacket);
		} catch(IOException ex) {
			logger.warn("Problem with sending UDP packet. {}", ex.getMessage());
		}
	}
	
	public void shutdown() {
		logger.info("Stopping UDP Proxy Server...");
		running = false;
		if(serverSocket != null && !serverSocket.isClosed()){
			serverSocket.close();
		}
		logger.info("UDP Proxy Server stopped");
	}
		
	public void run() {
		
		logger.info("Starting UDP Proxy Server...");
		try {
			serverSocket = new DatagramSocket(port);
			running = true;
			logger.info("Proxy Server started");
			logger.info("Listening for requests on port: {}", port);
		} catch (SocketException | DeviceException e) {
			logger.error(e.getMessage());
			running = false;
		}
		
		while (isRunning()) {
			DatagramPacket packet = readPacket();
			if(packet != null) {
				packets.add(packet);
			}
		}	
	}
}
