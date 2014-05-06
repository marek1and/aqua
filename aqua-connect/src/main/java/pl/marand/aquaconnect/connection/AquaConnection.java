package pl.marand.aquaconnect.connection;

import pl.marand.aquaconnect.exceptions.*;

public interface AquaConnection {
	
	public void connect() throws DeviceConnectionException;
	public void close();
	public boolean isConnected();
	public void write(byte[] data) throws DeviceWriteException;
	public byte[] read(int length) throws DeviceReadException, DeviceTimeoutException;
	
	public int getTimeout();
	public void setTimeout(int timeout);
	
	public void reboot() throws DeviceException;

}
