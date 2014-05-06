package pl.marand.aquaconnect.exceptions;

public class DeviceException extends RuntimeException {

	private static final long serialVersionUID = 7710129890217778043L;
	
	public DeviceException(String msg){
		super(msg);
	}
	
	public DeviceException(Throwable th){
		super(th);
	}
	
}
