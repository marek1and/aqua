package pl.marand.aqua;

import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Date;

import org.kohsuke.args4j.Option;

import pl.marand.aqua.handlers.DateOptionHandler;

public class AquaParams {
	
	@Option(name="-d", aliases = {"--device"}, usage="Device file of connected USB device (/dev/ttyUSBx)")
	private File usbDevice;
	
	@Option(name="-h", aliases = {"--host"}, usage="Device IP address")
	private InetAddress ip;
	
	@Option(name="-p", aliases = {"--port"}, usage="Device port. Default: 10", depends="-h")
	private int port = 4120;
	
	@Option(name="-s", aliases = {"--status"}, usage="Check if device is alive")
	private boolean isAlive;
	
	@Option(name="-t", aliases = {"--time"}, usage="Get device time")
	private boolean getDatetime;
	
	@Option(name="--stime", usage="Set device time in format: DD/MM/YYYY hh:mm:ss", handler = DateOptionHandler.class)
	private Date newTime;
	
	@Option(name="--temperature", usage="Get device temperatures")
	private boolean getTemperatures;
	
	@Option(name="--illuminance", usage="Get device illuminances")
	private boolean getIlluminances;
	
	@Option(name="-b", aliases = {"--backlight"}, usage="Get device LCD backlight")
	private boolean getBacklight;
	
	@Option(name="--sbacklight", usage="Set device backlight")
	private int backlight = -1;
				
	@Option(name="-o", aliases={"--output"}, usage="Specify device output")
	private int output;
	
	@Option(name="--state", usage="Get output state", depends = {"-o", "--output"})
	private boolean getState;
	
	@Option(name="--sstate", usage="Set output state", depends = {"-o", "--output"})
	private String state;
	
	@Option(name="--device-ip", usage="Get device IP address")
	private boolean getDeviceIp;
	
	@Option(name="--sdevice-ip", usage="Set device IP address")
	private InetAddress deviceIp;
	
	@Option(name="--device-port", usage="Get device port")
	private boolean getDevicePort;
	
	@Option(name="--sdevice-port", usage="Set device port")
	private int devicePort;
	
	@Option(name="--output-mode", usage="Get output mode", depends = {"-o"})
	private boolean getOutputMode;
	
	@Option(name="--soutput-mode", usage="Set output mode", depends = {"-o"})
	private int outputMode = -1;
	
	@Option(name="--output-timer", usage="Get output timer", depends = {"-o"})
	private boolean getOutputTimer;
	
	@Option(name="--soutput-timer", usage="Set output timer", depends = {"-o"})
	private boolean setOutputTimer;
	
	@Option(name="--from", usage="Set output from time", depends = {"--soutput-timer"}, handler = DateOptionHandler.class)
	private Date fromDate;
	
	@Option(name="--to", usage="Set output from time", depends = {"--soutput-timer"}, handler = DateOptionHandler.class)
	private Date toDate;
	
	@Option(name="--output-temperature", usage="Get output temperature", depends = {"-o"})
	private boolean getOutputTemperature;
	
	@Option(name="--soutput-temperature", usage="Set output temperature", depends = {"-o"})
	private float outputTemperature;
	
	@Option(name="--output-hysteresis", usage="Get output hysteresis", depends = {"-o"})
	private boolean getOutputHysteresis;
	
	@Option(name="--soutput-hysteresis", usage="Set output hysteresis", depends = {"-o"})
	private float outputHysteresis;
	
	@Option(name="--output-luminance", usage="Get output luminance", depends = {"-o"})
	private boolean getOutputLuminance;
	
	@Option(name="--soutput-luminance", usage="Set output luminance", depends = {"-o"})
	private int outputLuminance;
		
	@Option(name="--melody", usage="Play device melody")
	private int melody;
	
	@Option(name="--melody-repeat", usage="Repeat melody x times", depends = {"--melody"})
	private int melodyRepeat = 1;
	
	@Option(name="--default-settings", usage="Load default settings on device")
	private boolean loadDefault;
	
	@Option(name="--reboot", usage="Reboot device")
	private boolean reboot;
	
	@Option(name="--help")
	private boolean showHelp;
	
	public File getUsbDevice() {
		return usbDevice;
	}

	public InetAddress getIp() {
		return ip;
	}

	public int getPort() {
		return port;
	}

	public boolean isAlive() {
		return isAlive;
	}

	public boolean isGetDatetime() {
		return getDatetime;
	}

	public Date getNewTime() {
		return newTime;
	}

	public boolean isGetTemperatures() {
		return getTemperatures;
	}

	public boolean isGetIlluminances() {
		return getIlluminances;
	}

	public boolean isGetBacklight() {
		return getBacklight;
	}

	public int getBacklight() {
		return backlight;
	}

	public int getOutput() {
		return output;
	}

	public boolean isGetState() {
		return getState;
	}

	public String getState() {
		return state;
	}

	public boolean isGetDeviceIp() {
		return getDeviceIp;
	}

	public InetAddress getDeviceIp() {
		return deviceIp;
	}

	public boolean isGetDevicePort() {
		return getDevicePort;
	}

	public int getDevicePort() {
		return devicePort;
	}

	public boolean isGetOutputMode() {
		return getOutputMode;
	}

	public int getOutputMode() {
		return outputMode;
	}

	public boolean isGetOutputTimer() {
		return getOutputTimer;
	}

	public boolean isSetOutputTimer() {
		return setOutputTimer;
	}

	public Date getFromDate() {
		return fromDate;
	}

	public Date getToDate() {
		return toDate;
	}

	public boolean isGetOutputTemperature() {
		return getOutputTemperature;
	}

	public float getOutputTemperature() {
		return outputTemperature;
	}

	public boolean isGetOutputHysteresis() {
		return getOutputHysteresis;
	}

	public float getOutputHysteresis() {
		return outputHysteresis;
	}

	public boolean isGetOutputLuminance() {
		return getOutputLuminance;
	}

	public int getOutputLuminance() {
		return outputLuminance;
	}

	public int getMelody() {
		return melody;
	}
	
	public int getMelodyRepeat() {
		return melodyRepeat;
	}

	public boolean isLoadDefault() {
		return loadDefault;
	}

	public boolean isReboot() {
		return reboot;
	}

	public boolean isShowHelp() {
		return showHelp;
	}
	
	public boolean validateConnectionParams() {
		
		boolean validation = true;
		
		if(usbDevice == null && ip == null) {
			System.out.println("Please choose connection type: -d or -h");
			validation = false;
		}
		
		if(usbDevice != null) {
			if(!usbDevice.exists()) {
				validation = false;
				System.out.printf("USB device file: %s not exists!\n", usbDevice.getAbsolutePath());
			}
		}
		
		if(ip != null) {
			try {
				validation = ip.isReachable(5);
			} catch(IOException ex) {
				System.out.printf("Host with IP: %s is not reachable\n", ip.getHostAddress());
			}
		}
		return validation;
	}
	
	public boolean validateOutput() {
		if(output < 0 || output > 5) {
			System.out.println("Output should be between 0 and 5. Option 0 for all.");
			return false;
		}
		return true;
	}
	
	public boolean validateOutputState() {
		if(state != null) {
			if(!("off".equals(state.toLowerCase())) && !("on".equals(state.toLowerCase()))) {
				System.out.println("Valid state options: on, off");
				return false;
			}
		}
		return true;
	}
	
	public boolean validateLCDBacklight() {
		if (backlight != -1 && (backlight <= 0 || backlight > 255)) {
			System.out.println("Backlight value should be between 0 and 255");
			return false;
		}
		return true;
	}
	
	
	public boolean validate() {
		
		boolean validation = validateConnectionParams();
		validation &= validateOutput();
		validation &= validateLCDBacklight();
		validation &= validateOutputState();
		
		return validation;
	}

}
