package pl.marand.aquaconnect.device;

public enum ReturnCode {
	SUCCESS(0),
	FAILURE(1),
	INVALID_OPTION(2);
	
	private byte code;
	
	private ReturnCode(int code){
		this.code = (byte)code;
	}
	
	public static boolean isSuccess(byte code){
		
		if(getReturnCode(code) == SUCCESS){
			return true;
		}
	
		return false;
	}
	
	public static ReturnCode getReturnCode(byte code){
		if(code <= 2){
			return values()[code];
		}
		return null;
	}
	
	public byte getReturnCodeByte(){
		return code;
	}
}
