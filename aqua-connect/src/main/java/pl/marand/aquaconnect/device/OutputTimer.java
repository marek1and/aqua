package pl.marand.aquaconnect.device;

public class OutputTimer {
	
	private int fromHour;
	private int fromMinute;
	private int toHour;
	private int toMinute;
	
	public OutputTimer(){
		fromHour = 0;
		fromMinute = 0;
		toHour = 0;
		toMinute = 0;
	}
	
	public OutputTimer(int fromHour, int fromMinute, int toHour, int toMinute){
		this.fromHour = fromHour;
		this.fromMinute = fromMinute;
		this.toHour = toHour;
		this.toMinute = toMinute;
	}

	public int getFromHour() {
		return fromHour;
	}

	public void setFromHour(int fromHour) {
		this.fromHour = fromHour;
	}

	public int getFromMinute() {
		return fromMinute;
	}

	public void setFromMinute(int fromMinute) {
		this.fromMinute = fromMinute;
	}

	public int getToHour() {
		return toHour;
	}

	public void setToHour(int toHour) {
		this.toHour = toHour;
	}

	public int getToMinute() {
		return toMinute;
	}

	public void setToMinute(int toMinute) {
		this.toMinute = toMinute;
	}
}
