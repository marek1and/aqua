package pl.marand.aquaconnect.device;

public enum Photoresistor {
	
	PHOTORESISTOR1((byte) (0x01 << 5)),
	PHOTORESISTOR2((byte) (0x02 << 5));
	
	private byte res;
	
	private Photoresistor(byte res){
		this.res = res;
	}
	
	public byte getPhotoresistorByte(){
		return res;
	}
	
}
