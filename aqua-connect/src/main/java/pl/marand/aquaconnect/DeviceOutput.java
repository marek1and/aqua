package pl.marand.aquaconnect;

import pl.marand.aquaconnect.connection.AquaConnection;
import pl.marand.aquaconnect.device.Function;
import pl.marand.aquaconnect.device.OperationMode;
import pl.marand.aquaconnect.device.OutputMode;
import pl.marand.aquaconnect.device.OutputState;
import pl.marand.aquaconnect.device.OutputTimer;
import pl.marand.aquaconnect.device.Outputs;

public class DeviceOutput {
	
	private AquaConnection connection;
	private AquaDevice device;
	private Outputs output;
	
	public DeviceOutput(Outputs output, AquaDevice device) {
		this.device = device;
		this.output = output;
		connection = device.getConnection();
	}
	
	public OutputMode getMode() {
		
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.OUT_MODE.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.OUT_MODE.getFunctionReturnLength());
		
		int id = out[1] & 0xff;
		
		return OutputMode.getOutputModeById(id);
	}
	
	public boolean setMode(OutputMode mode) {
		byte command[] = new byte[Function.OUT_MODE.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.OUT_MODE.getFunctionByte());
		command[1] = (byte)mode.getOutputModeCode();
		connection.write(command);
		
		return device.verifyOperation();
	}
	
	public OutputTimer getTimer() {
		
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.OUT_TIMER.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.OUT_TIMER.getFunctionReturnLength());
		
		OutputTimer timer = new OutputTimer();
		timer.setFromHour(out[1]);
		timer.setFromMinute(out[2]);
		timer.setToHour(out[3]);
		timer.setToMinute(out[4]);
		
		return timer;
	}
	
	public boolean setTimer(OutputTimer timer) {
		byte command[] = new byte[Function.OUT_TIMER.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.OUT_TIMER.getFunctionByte());
		command[1] = (byte)timer.getFromHour();
		command[2] = (byte)timer.getFromMinute();
		command[3] = (byte)timer.getToHour();
		command[4] = (byte)timer.getToMinute();
		
		connection.write(command);
		
		return device.verifyOperation();
	}
	
	public float getTemperature() {
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.OUT_TEMPERATURE.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.OUT_TEMPERATURE.getFunctionReturnLength());
		
		int value = ((out[4] << 24) & 0xFF000000) | ((out[3] << 16) & 0x00FF0000) | ((out[2] << 8) & 0x0000FF00) | (out[1] & 0x000000FF);
		return Float.intBitsToFloat(value);
	}
	
	public boolean setTemperature(float temperature) {
		byte command[] = new byte[Function.OUT_TEMPERATURE.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.OUT_TEMPERATURE.getFunctionByte());
		int value = Float.floatToIntBits(temperature);
		command[1] = (byte)(value & 0xFF);
		command[2] = (byte)((value >> 8) & 0xFF);
		command[3] = (byte)((value >> 16) & 0xFF);
		command[4] = (byte)((value >> 24) & 0xFF);
		connection.write(command);
		
		return device.verifyOperation();
	}
	
	public float getHysteresis() {
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.OUT_HYSTERESIS.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.OUT_HYSTERESIS.getFunctionReturnLength());
		
		int value = ((out[4] << 24) & 0xFF000000) | ((out[3] << 16) & 0x00FF0000) | ((out[2] << 8) & 0x0000FF00) | (out[1] & 0x000000FF);
		return Float.intBitsToFloat(value);
	}
	
	public boolean setHysteresis(float hysteresis) {
		byte command[] = new byte[Function.OUT_HYSTERESIS.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.OUT_HYSTERESIS.getFunctionByte());
		int value = Float.floatToIntBits(hysteresis);
		command[1] = (byte)(value & 0xFF);
		command[2] = (byte)((value >> 8) & 0xFF);
		command[3] = (byte)((value >> 16) & 0xFF);
		command[4] = (byte)((value >> 24) & 0xFF);
		connection.write(command);
		
		return device.verifyOperation();
	}

	public int getLuminance() {
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.OUT_LUMINANCE.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.OUT_LUMINANCE.getFunctionReturnLength());
		
		int value = ((out[1] << 8) & 0xff00) | (out[2] & 0xff);
		return value;
	}
	
	public boolean setLuminance(int illuminance) {
		byte command[] = new byte[Function.OUT_LUMINANCE.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.OUT_LUMINANCE.getFunctionByte());
		command[1] = (byte)((illuminance >> 8) & 0xFF);
		command[2] = (byte)(illuminance & 0xFF);
		connection.write(command);
		
		return device.verifyOperation();
	}
	
	public OutputState getState() {
		byte command[] = new byte[]{(byte)(output.getOutputByte() | OperationMode.READ.getModeByte() | Function.CONTROL.getFunctionByte())};
		connection.write(command);
		byte out[] = connection.read(Function.CONTROL.getFunctionReturnLength());
		return OutputState.getOutputState(out[1]);
	}
	
	public boolean setState(OutputState state) {
		byte command[] = new byte[Function.CONTROL.getFunctionReturnLength()];
		command[0] = (byte)(output.getOutputByte() | OperationMode.WRITE.getModeByte() | Function.CONTROL.getFunctionByte());
		command[1] = state.getStateByte();
		connection.write(command);
		
		return device.verifyOperation();
	}
}
