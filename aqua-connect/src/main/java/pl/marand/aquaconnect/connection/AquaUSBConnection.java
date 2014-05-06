package pl.marand.aquaconnect.connection;

import jssc.SerialPort;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import jssc.SerialPortTimeoutException;
import pl.marand.aquaconnect.device.Function;
import pl.marand.aquaconnect.exceptions.DeviceConnectionException;
import pl.marand.aquaconnect.exceptions.DeviceException;
import pl.marand.aquaconnect.exceptions.DeviceReadException;
import pl.marand.aquaconnect.exceptions.DeviceTimeoutException;
import pl.marand.aquaconnect.exceptions.DeviceWriteException;

public class AquaUSBConnection implements AquaConnection {
	
	private SerialPort port;
	private int timeout;
	
	public AquaUSBConnection(String usbPort){
		this.port = new SerialPort(usbPort);
		timeout = 5000;
	}
	
	@Override
	public void connect() throws DeviceConnectionException {
		try {
			if(port != null && !port.isOpened()){
				port.openPort();
				port.setParams(SerialPort.BAUDRATE_9600, 
                               SerialPort.DATABITS_8,
                               SerialPort.STOPBITS_1,
                               SerialPort.PARITY_NONE);
			}
        }
        catch (SerialPortException ex) {
            throw new DeviceConnectionException(ex.getMessage());
        }
	}

	@Override
	public void close() {
		if(port != null && port.isOpened()){
			try {
				port.closePort();
			} catch (SerialPortException e) {
				// Do nothing
			}
		}
	}
	
	@Override
	public boolean isConnected() {
		boolean connected = false;
		if(port != null) {
			connected = port.isOpened();
		}
		return connected;
	}

	@Override
	public void write(byte[] data) throws DeviceWriteException {
		if(port != null && port.isOpened()){
			try {
				port.writeByte((byte)0xEE);
				port.writeBytes(data);
				port.writeByte((byte)0xEF);
			} catch (SerialPortException e) {
				throw new DeviceWriteException(e.getMessage());
			}
		}
	}

	@Override
	public byte[] read(int length) throws DeviceReadException, DeviceTimeoutException {
		byte[] packet = new byte[length + 2];
		byte[] data = new byte[length];
		if(port != null && port.isOpened()){
			try {
				packet = port.readBytes(length + 2, timeout);
				
				for(int i = 0; i < packet.length; i++){
					
					if(i == 0 && packet[i] != (byte)0xEE){
						throw new DeviceReadException("Device packet error! Wrong start byte!");
					} else if(i == packet.length - 1 && packet[i] != (byte)0xEF){
						throw new DeviceReadException("Device packet error! Wrong stop byte!");
					} else if( i != 0 && i != packet.length - 1){
						data[i - 1] = packet[i];
					}
				}
				
			} catch (SerialPortException e) {
				throw new DeviceReadException(e.getMessage());
			} catch (SerialPortTimeoutException e){
				throw new DeviceTimeoutException("Wait for response timeout!");
			} 
			
		}
		return data;
	}

	@Override
	public int getTimeout() {
		return timeout;
	}

	@Override
	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}
	
	public void setSerialPortReader(SerialPortEventListener reader) throws DeviceException{
		if(reader != null){
			if(port != null){
				int mask = SerialPort.MASK_RXCHAR + SerialPort.MASK_CTS + SerialPort.MASK_DSR;
	            try {
	            	port.setEventsMask(mask);
					port.removeEventListener();
	            	port.addEventListener(reader);
				} catch (SerialPortException e) {
					throw new DeviceException(e.getMessage());
				}
			}
		}
	}

	@Override
	public void reboot() throws DeviceException{
		write(new byte[]{(byte)Function.REBOOT_DEVICE.getFunctionByte()});
	}
}
