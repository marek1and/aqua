package pl.marand.aquaconnect.device;

public enum OperationMode {
	READ((byte) 0x10),
	WRITE((byte) 0x00);
	
	private byte mode;
	
	private OperationMode(byte mode){
		this.mode = mode;
	}
	
	public byte getModeByte(){
		return mode;
	}
}
