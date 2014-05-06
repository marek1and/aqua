package pl.marand.aquaconnect;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;

import pl.marand.aquaconnect.connection.AquaConnection;
import pl.marand.aquaconnect.connection.AquaUDPConnection;
import pl.marand.aquaconnect.device.*;
import pl.marand.aquaconnect.exceptions.DeviceReadException;
import pl.marand.aquaconnect.exceptions.DeviceTimeoutException;

public class AquaDevice {
	
	public static final int STATE_MESSAGE_LENGTH = 2;
	
	private AquaConnection connection;
	private HashMap<Outputs, DeviceOutput> outputs;
	
	public AquaDevice(AquaConnection connection) {
		this.connection = connection;
		prepareDeviceOutputsMap();
	}
	
	private void prepareDeviceOutputsMap() {
		outputs = new HashMap<Outputs, DeviceOutput>();
		outputs.put(Outputs.OUTPUT1, new DeviceOutput(Outputs.OUTPUT1, this));
		outputs.put(Outputs.OUTPUT2, new DeviceOutput(Outputs.OUTPUT2, this));
		outputs.put(Outputs.OUTPUT3, new DeviceOutput(Outputs.OUTPUT3, this));
		outputs.put(Outputs.OUTPUT4, new DeviceOutput(Outputs.OUTPUT4, this));
		outputs.put(Outputs.OUTPUT5, new DeviceOutput(Outputs.OUTPUT5, this));
	}
	
	public void connect() {
		if(connection != null){
			connection.connect();
		}
	}
	
	public void close() {
		if(connection != null){
			connection.close();
		}
	}
	
	public boolean isAlive() {
		byte command[] = new byte[]{(byte)(OperationMode.READ.getModeByte() | Function.IS_ALIVE.getFunctionByte())};
		connection.write(command);
		return verifyOperation();
	}

	public AquaConnection getConnection() {
		return connection;
	}

	public void setConnection(AquaConnection connection) {
		this.connection = connection;
	}
	
	public DeviceOutput getOutput(Outputs output) {
		return outputs.get(output);
	}
	
	public void addOutput(Outputs output, DeviceOutput devOutput) {
		outputs.put(output, devOutput);
	}
	
	public void removeOutput(Outputs output) {
		outputs.remove(output);
	}
	
	public void removeAllOutputs(){
		outputs.clear();
	}

	public boolean verifyOperation() {
		try {
			byte out[] = connection.read(2);
			if(out.length >= STATE_MESSAGE_LENGTH && ReturnCode.isSuccess(out[1])){
				return true;
			}
		} catch (DeviceReadException e) {
			throw new DeviceReadException("Can't read operation status!\n" + e.getMessage());
		} catch (DeviceTimeoutException e) {
			throw new DeviceTimeoutException("Read operation timeout!\n" + e.getMessage());
		}
		return false;
	}
	
	public Date getTime() {
		
		byte command[] = new byte[]{(byte)(OperationMode.READ.getModeByte() | Function.DATETIME.getFunctionByte())};
		
		connection.write(command);
		byte out[] = connection.read(Function.DATETIME.getFunctionReturnLength());
		
		int hour 	= (int)out[1];
		int minute 	= (int)out[2];
		int second 	= (int)out[3];
		int day 	= (int)out[4];
		int month 	= (int)out[5];
		int year 	= ((out[6] << 8) & 0xFF00) | (out[7] & 0x00FF);
		
		Calendar cal = Calendar.getInstance();
		cal.set(year, month - 1, day, hour, minute, second);
		
		return cal.getTime();
	}
	
	public boolean setTime(Date time) {
		
		Calendar cal = Calendar.getInstance();
		cal.setTime(time);
		
		byte command[] = new byte[Function.DATETIME.getFunctionReturnLength()];
		command[0] = (byte)(OperationMode.WRITE.getModeByte() | Function.DATETIME.getFunctionByte());
		command[1] = (byte)cal.get(Calendar.HOUR_OF_DAY);
		command[2] = (byte)cal.get(Calendar.MINUTE);
		command[3] = (byte)cal.get(Calendar.SECOND);
		command[4] = (byte)cal.get(Calendar.DAY_OF_MONTH);
		command[5] = (byte)(cal.get(Calendar.MONTH) + 1);
		
		int year 	= cal.get(Calendar.YEAR);
		command[6] = (byte)((year >> 8) & 0x00FF);
		command[7] = (byte)(year & 0x00FF);
		
		connection.write(command);
		return verifyOperation();
	}
	
	public float getTemparature(Thermometer thermometer) {
		byte command[] = new byte[]{(byte)(thermometer.getThermometerByte() | OperationMode.READ.getModeByte() | Function.TEMPERATURE.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.TEMPERATURE.getFunctionReturnLength());
		
		int value = ((out[4] << 24) & 0xFF000000) | ((out[3] << 16) & 0x00FF0000) | ((out[2] << 8) & 0x0000FF00) | (out[1] & 0x000000FF);
		return Float.intBitsToFloat(value);
	}
	
	public int getIlluminance(Photoresistor photoresistor) {
		
		byte command[] = new byte[]{(byte)(photoresistor.getPhotoresistorByte() | OperationMode.READ.getModeByte() | Function.ILLUMINANCE.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.ILLUMINANCE.getFunctionReturnLength());
		
		int value = ((out[1] << 8) & 0xFF00) | (out[2] & 0x00FF);
		return value;
	}
	
	public int getBacklight() {
		byte command[] = new byte[]{(byte)(OperationMode.READ.getModeByte() | Function.BACKLIGHT.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.BACKLIGHT.getFunctionReturnLength());
		
		int value = out[1] & 0xff;
		return value;
	}
	
	public boolean setBacklight(int backlight) {
		
		byte command[] = new byte[Function.BACKLIGHT.getFunctionReturnLength()];
		command[0] = (byte)(OperationMode.WRITE.getModeByte() | Function.BACKLIGHT.getFunctionByte());
		command[1] = (byte)backlight;
		connection.write(command);
		
		return verifyOperation();
	}
	
	public String getIpAddress() {
		byte command[] = new byte[]{(byte)(OperationMode.READ.getModeByte() | Function.IP_ADDRESS.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.IP_ADDRESS.getFunctionReturnLength());
		int a = out[1] & 0xFF;
		int b = out[2] & 0xFF;
		int c = out[3] & 0xFF;
		int d = out[4] & 0xFF;
		
		return String.format("%d.%d.%d.%d", a, b, c, d);
	}
	
	public boolean setIpAddress(String ip) {
		
		String ipGroups[] = ip.split("\\.");
		if(ipGroups.length < 4){
			return false;
		}
		
		byte command[] = new byte[Function.IP_ADDRESS.getFunctionReturnLength()];
		command[0] = (byte)(OperationMode.WRITE.getModeByte() | Function.IP_ADDRESS.getFunctionByte());
		
		command[1] = (byte)Integer.valueOf(ipGroups[0]).intValue();
		command[2] = (byte)Integer.valueOf(ipGroups[1]).intValue();
		command[3] = (byte)Integer.valueOf(ipGroups[2]).intValue();
		command[4] = (byte)Integer.valueOf(ipGroups[3]).intValue();
		
		connection.write(command);
		
		if(connection instanceof AquaUDPConnection) {
			try {
				((AquaUDPConnection) connection).setIp(InetAddress.getByName(ip));
			} catch (UnknownHostException e) {
				System.err.println("Can't set connection IP to: " + ip);
			}
		}
		
		return verifyOperation();
	}
	
	public int getPort() {
		byte command[] = new byte[]{(byte)(OperationMode.READ.getModeByte() | Function.PORT.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.PORT.getFunctionReturnLength());
		
		int port = out[1] & 0xff;
		return port;
	}
	
	public boolean setPort(int port) {
		byte command[] = new byte[Function.PORT.getFunctionReturnLength()];
		command[0] = (byte)(OperationMode.WRITE.getModeByte() | Function.PORT.getFunctionByte());
		command[1] = (byte)(port & 0xFF);
		connection.write(command);
		
		if(connection instanceof AquaUDPConnection){
			((AquaUDPConnection) connection).setPort(port);
		}
		
		return verifyOperation();
	}
	
	public byte getOutputsState() {
		byte command[] = new byte[]{(byte)(Outputs.OUTPUT0.getOutputByte() | OperationMode.READ.getModeByte() | Function.CONTROL.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.CONTROL.getFunctionReturnLength());
		return out[1];
	}
	
	public boolean setOutputsState(OutputState state) {
		byte command[] = new byte[Function.CONTROL.getFunctionReturnLength()];
		command[0] = (byte)(Outputs.OUTPUT0.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.CONTROL.getFunctionByte());
		if(state.equals(OutputState.ON)){
			command[1] = (byte)0xFF;
		} else {
			command[1] = (byte)0x00;	
		}
		connection.write(command);

		return verifyOperation();
	}
	
	public boolean loadDefaultSettings() {
		byte command[] = new byte[Function.CONTROL.getFunctionReturnLength()];
		command[0] = (byte)(OperationMode.WRITE.getModeByte() | Function.DEFAULT_SETTINGS.getFunctionByte());
		connection.write(command);
		
		return verifyOperation();
	}
	
	public boolean playMelody(Melody melody, int repeat) {
		
		byte command[] = new byte[2];
		command[0] = (byte)(melody.getMelodyByte() | OperationMode.WRITE.getModeByte() | Function.PLAY_MELODY.getFunctionByte());
		command[1] = (byte)(repeat & 0xFF);
		connection.write(command);
		
		return verifyOperation();
	}
	
	public void reboot() {
		connection.reboot();
	}
}
