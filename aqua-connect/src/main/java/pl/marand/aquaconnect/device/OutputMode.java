package pl.marand.aquaconnect.device;

public enum OutputMode {
	       
	MANUAL(1),
	ON(2),
	OFF(3),
	TIMER(4),
	LESS_THAN_TEMPERATURE1(5),
	GREATER_THAN_TEMPERATURE1(6),
	LESS_THAN_TEMPERATURE2(7),
	GREATER_THAN_TEMPERATURE2(8),
	LESS_THAN_LUMINANCE1(9),
	GREATER_THAN_LUMINANCE1(10),
	LESS_THAN_LUMINANCE2(11),
	GREATER_THAN_LUMINANCE2(12);
	
	private int mode;
	
	private OutputMode(int mode){
		this.mode = mode;
	}
	
	public int getOutputModeCode(){
		return mode;
	}
	
	public static OutputMode getOutputModeById(int id){
		return OutputMode.values()[id - 1];
	}
}
