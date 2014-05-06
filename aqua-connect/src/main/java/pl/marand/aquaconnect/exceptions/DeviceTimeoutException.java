package pl.marand.aquaconnect.exceptions;

public class DeviceTimeoutException extends DeviceException {

	private static final long serialVersionUID = 214403518804331079L;

	public DeviceTimeoutException(String msg){
		super(msg);
	}
	
	public DeviceTimeoutException(Throwable th){
		super(th);
	}
}
