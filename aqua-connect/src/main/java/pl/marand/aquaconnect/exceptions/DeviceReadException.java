package pl.marand.aquaconnect.exceptions;

public class DeviceReadException extends DeviceException{

	private static final long serialVersionUID = 2112407892746237206L;

	public DeviceReadException(String msg){
		super(msg);
	}
	
	public DeviceReadException(Throwable th){
		super(th);
	}
}
