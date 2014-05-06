package pl.marand.aquaconnect.device;

public enum Melody {
	MELODY_STOP	((byte)0x00),	
	MELODY_1	((byte)0x20),	
	MELODY_2	((byte)0x40),	
	MELODY_3	((byte)0x60),	
	MELODY_4	((byte)0x80),	
	MELODY_5	((byte)0xA0),	
	MELODY_6	((byte)0xC0),	
	MELODY_7	((byte)0xE0);	
	
	private byte melody;

	private Melody(byte melody){
		this.melody = melody;
	}
	
	public byte getMelodyByte(){
		return melody;
	}
	
}
