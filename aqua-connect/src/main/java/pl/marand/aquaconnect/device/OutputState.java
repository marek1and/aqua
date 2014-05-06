package pl.marand.aquaconnect.device;

public enum OutputState {
	OFF((byte) 0x00),
	ON ((byte) 0x01);
	
	private byte state;
	
	private OutputState(byte state){
		this.state = state;
	}
	
	public byte getStateByte(){
		return state;
	}
	
	public static OutputState getOutputState(byte st){
		return values()[st];
	}
}
