package pl.marand.aqua;

import java.net.DatagramPacket;
import java.util.concurrent.ArrayBlockingQueue;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import pl.marand.aquaconnect.connection.AquaConnection;
import pl.marand.aquaconnect.device.Function;
import pl.marand.aquaconnect.device.OperationMode;
import pl.marand.aquaconnect.exceptions.DeviceException;

public class ProxyServer extends Thread {
	
	private static final Logger logger = LoggerFactory.getLogger(ProxyServer.class);
	
	private static final DatagramPacket STOP_PACKET = new DatagramPacket(new byte[]{}, 0);
	
	private AquaConnection connection;
	private UDPServer udpServer;
	
	private ArrayBlockingQueue<DatagramPacket> packets;
	
	public ProxyServer(AquaConnection connection, int port) {
		this.connection = connection;
		packets= new ArrayBlockingQueue<DatagramPacket>(10, true);	
		udpServer = new UDPServer(port, packets);
	}
	
	@Override
	public void start() {
		if(udpServer != null) {
			udpServer.start();
		}
		if(connection != null) {
			connection.connect();
		}
		super.start();
	}
	
	public void shutdown() {
		packets.add(STOP_PACKET);
		if(udpServer != null) {
			udpServer.shutdown();
		}
		if(connection != null) {
			connection.close();
		}
	}
	
	public boolean isRunning() {
		boolean running = false;
		if(udpServer != null) {
			running = udpServer.isRunning();
		}
		return running;
	}
	
	private byte[] getPacketData(DatagramPacket packet) {
		
		byte data[] = new byte[packet.getLength()];
		
		for(int i = 0; i < packet.getLength(); i++) {
			data[i] = packet.getData()[i];
		}
		
		return data;
	}
	
	private int getResponseLength(byte[] data) {
		int length = 0;
		byte firstByte = data[0];
		byte operationMode = (byte)(firstByte & 0x10);
		Function function = getFunction(data);
		
		if(operationMode == OperationMode.WRITE.getModeByte()) {
			length = 2;
		} else {
			length = function.getFunctionReturnLength();
		}
		
		return length;
	}
	
	private Function getFunction(byte[] data) {
		byte firstByte = data[0];
		return Function.getFunction((byte)(firstByte & 0x0F));
	}
	
	private String getFormattedData(byte[] data) {
		StringBuilder output = new StringBuilder();
		for(int i = 0; i < data.length; i++) {
			output.append(String.format("%02x ", (data[i] & 0xFF )));
		}
		return output.toString();
	}
	
	private boolean checkConnectionState() {
		boolean state = false;
		if(connection != null) {
			state = connection.isConnected();
		}
		return state;
	}
	
	@Override
	public void run() {
		
		DatagramPacket packet = null;
		try {
			while((packet = packets.take()) != STOP_PACKET) {
				byte data[] = getPacketData(packet);
				if(data != null && data.length > 0){
					try {
						logger.debug("Packet received: {}", getFormattedData(data));
						Function function = getFunction(data);
						
						if(!checkConnectionState()) {
							logger.error("Connection is not estaablished or it was lost!");
						}
						
						if(function != null && function.equals(Function.REBOOT_DEVICE)){
							connection.reboot();
						} else {
							connection.write(data);
							int length = getResponseLength(data);
							byte[] result = connection.read(length);
							udpServer.sendResult(packet.getAddress(), packet.getPort(), result);
						}
						
					} catch(DeviceException ex) {
						logger.error(ex.getMessage());
					}
				}
			}
		} catch (InterruptedException ex) {
			logger.warn("Getting packets from queue was interrupted.");
		}
		
	}
	
}
