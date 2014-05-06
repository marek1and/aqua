package pl.marand.aquaconnect.device;

public enum Function {
	
	IS_ALIVE			((byte)0x00, 2),	
	DATETIME			((byte)0x01, 8),	
	TEMPERATURE			((byte)0x02, 5),		
	ILLUMINANCE			((byte)0x03, 3),
	BACKLIGHT			((byte)0x04, 2),
	OUT_MODE			((byte)0x05, 2),
	OUT_TIMER			((byte)0x06, 5),
	OUT_TEMPERATURE		((byte)0x07, 5),
	OUT_HYSTERESIS		((byte)0x08, 5),
	OUT_LUMINANCE		((byte)0x09, 3),
	IP_ADDRESS			((byte)0x0A, 5),
	PORT				((byte)0x0B, 2),
	CONTROL				((byte)0x0C, 2),
	DEFAULT_SETTINGS	((byte)0x0D, 2),
	PLAY_MELODY			((byte)0x0E, 2),
	REBOOT_DEVICE		((byte)0x0F, 0);
	
	private byte func;
	private int len;

	private Function(byte func, int len){
		this.func = func;
		this.len = len;
	}
	
	public byte getFunctionByte(){
		return func;
	}
	
	public int getFunctionReturnLength(){
		return len;
	}
	
	public static Function getFunction(byte functionByte){
		
		Function function = null;
		
		for(Function f: Function.values()){
			if(f.getFunctionByte() == functionByte){
				function = f;
			}
		}
		return function;
	}
}
