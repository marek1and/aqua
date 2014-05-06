package pl.marand.aquaconnect.device;

public enum Outputs {
	
	OUTPUT0((byte) 0x00),
	OUTPUT1((byte) (0x01 << 5)),
	OUTPUT2((byte) (0x02 << 5)),
	OUTPUT3((byte) (0x03 << 5)),
	OUTPUT4((byte) (0x04 << 5)),
	OUTPUT5((byte) (0x05 << 5)),
	OUTPUT6((byte) (0x06 << 5)),
	OUTPUT7((byte) (0x07 << 5));
	
	private byte out;
	
	private Outputs(byte out){
		this.out = out;
	}
	
	public byte getOutputByte(){
		return out;
	}
}