package pl.marand.aquaconnect.device;

public enum Parameter {
	
	PARAM0((byte) 0x00),
	PARAM1((byte) (0x01 << 5)),
	PARAM2((byte) (0x02 << 5)),
	PARAM3((byte) (0x03 << 5)),
	PARAM4((byte) (0x04 << 5)),
	PARAM5((byte) (0x05 << 5)),
	PARAM6((byte) (0x06 << 5)),
	PARAM7((byte) (0x07 << 5));
	
	private byte param;
	
	private Parameter(byte param){
		this.param = param;
	}
	
	public byte getParameterByte(){
		return param;
	}
}
