package pl.marand.aquaconnect.connection;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

import pl.marand.aquaconnect.device.Function;
import pl.marand.aquaconnect.exceptions.DeviceConnectionException;
import pl.marand.aquaconnect.exceptions.DeviceException;
import pl.marand.aquaconnect.exceptions.DeviceReadException;
import pl.marand.aquaconnect.exceptions.DeviceTimeoutException;
import pl.marand.aquaconnect.exceptions.DeviceWriteException;
import pl.marand.aquaconnect.utils.WakeOnLanPacket;

public class AquaUDPConnection implements AquaConnection {
	
	private DatagramSocket socket;
	
	private byte[] macAddress;
	private InetAddress ip;
	private int port;
	private boolean proxy;
	
	public AquaUDPConnection(String ip, int port) throws DeviceException {
		try {
			this.ip = InetAddress.getByName(ip);
		} catch (UnknownHostException e) {
			throw new DeviceConnectionException(e);
		}
		this.port = port;
		macAddress = new byte[]{0x30, 0x41, 0x51, 0x55, 0x41, 0x30};
	}
	
	public AquaUDPConnection(String ip, int port, boolean proxy) throws DeviceException {
		this(ip, port);
		this.proxy = proxy;
	}
	
	@Override
	public void connect() throws DeviceConnectionException {
		try {
			socket = new DatagramSocket();
			setTimeout(5000);
		} catch (SocketException e) {
			throw new DeviceConnectionException(e.getMessage());
		}
	}

	@Override
	public void close() {
		if(socket != null){
			if(socket.isConnected()){
				socket.close();
			}
		}
	}

	@Override
	public boolean isConnected() {
		boolean connected = false;
		if(socket != null) {
			connected = socket.isConnected();
		}
		return connected;
	}
	
	@Override
	public void write(byte[] data) throws DeviceWriteException {
		DatagramPacket packet = new DatagramPacket(data, data.length, ip, port);
		if(socket != null && socket.isBound()) {
			try {
				socket.send(packet);
			} catch (IOException e) {
				throw new DeviceWriteException(e.getMessage());
			}
		}
	}

	@Override
	public byte[] read(int length) throws DeviceReadException, DeviceTimeoutException{
		byte[] data = new byte[length];
		DatagramPacket packet = new DatagramPacket(data, length);
		try {
			socket.receive(packet);
		} catch(SocketTimeoutException e){
			throw new DeviceTimeoutException("Wait for response timeout!");
		} catch (IOException e) {
			throw new DeviceReadException(e.getMessage());
		}
		return data;
	}

	public byte[] getMacAddress() {
		return macAddress;
	}

	public void setMacAddress(byte[] macAddress) {
		this.macAddress = macAddress;
	}

	public InetAddress getIp() {
		return ip;
	}

	public void setIp(InetAddress ip) {
		this.ip = ip;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}
	
	@Override
	public int getTimeout() {
		try {
			return socket.getSoTimeout();
		} catch (SocketException e) {
			return -1;
		}
	}
	
	@Override
	public void setTimeout(int timeout) {
		try {
			socket.setSoTimeout(timeout);
		} catch (SocketException e) {
			// Do nothing
		}
	}

	@Override
	public void reboot() throws DeviceException{
		if(proxy) {
			write(new byte[]{(byte)Function.REBOOT_DEVICE.getFunctionByte()});
		} else {
			boolean state = WakeOnLanPacket.send(macAddress, ip);
			if(!state){
				throw new DeviceConnectionException("Can't send reboot packet");
			}
		}
	}
}
