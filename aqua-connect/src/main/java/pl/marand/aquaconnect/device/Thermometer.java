package pl.marand.aquaconnect.device;

public enum Thermometer {
	
	THERMOMETER1((byte) (0x01 << 5)),
	THERMOMETER2((byte) (0x02 << 5));
	
	private byte therm;
	
	private Thermometer(byte therm){
		this.therm = therm;
	}
	
	public byte getThermometerByte(){
		return therm;
	}
}
