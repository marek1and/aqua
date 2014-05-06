package pl.marand.aquaconnect.exceptions;

public class DeviceConnectionException extends DeviceException {

	private static final long serialVersionUID = -3391240305743136081L;
	
	public DeviceConnectionException(String msg){
		super(msg);
	}
	
	public DeviceConnectionException(Throwable th){
		super(th);
	}
}
