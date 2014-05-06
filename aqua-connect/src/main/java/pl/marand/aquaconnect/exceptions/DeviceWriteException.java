package pl.marand.aquaconnect.exceptions;

public class DeviceWriteException extends DeviceException {

	private static final long serialVersionUID = -5820894776170536866L;

	public DeviceWriteException(String msg){
		super(msg);
	}
	
	public DeviceWriteException(Throwable th){
		super(th);
	}
}
