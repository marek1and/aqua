package pl.marand.aqua;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;

import pl.marand.aquaconnect.AquaDevice;
import pl.marand.aquaconnect.DeviceOutput;
import pl.marand.aquaconnect.connection.AquaConnection;
import pl.marand.aquaconnect.connection.AquaUDPConnection;
import pl.marand.aquaconnect.connection.AquaUSBConnection;
import pl.marand.aquaconnect.device.Melody;
import pl.marand.aquaconnect.device.OutputMode;
import pl.marand.aquaconnect.device.OutputState;
import pl.marand.aquaconnect.device.OutputTimer;
import pl.marand.aquaconnect.device.Outputs;
import pl.marand.aquaconnect.device.Photoresistor;
import pl.marand.aquaconnect.device.Thermometer;
import pl.marand.aquaconnect.exceptions.DeviceException;

public class Aqua {
	
	private AquaParams params;
	private AquaConnection connection;
	private AquaDevice device;
	
	private SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
	
	public Aqua(AquaParams params) {
		this.params = params;
	}
	
	public void parseArguments(String[] args) {
		
		CmdLineParser parser = new CmdLineParser(params);
		
		try {
			parser.parseArgument(args);
		} catch (CmdLineException e) {
			System.out.println(e.getMessage());
			System.out.println("Use option '--help' to show possible arguments!");
		}
		
		if(params.isShowHelp()) {
			parser.printUsage(System.out);
			System.exit(1);
		}
		
		if(!params.validate()) {
			System.out.println("Type --help option and type correct values for parameters!");
			System.exit(2);
		}
	}
	
	public void connect() {
		if(params.getUsbDevice() != null) {
			connection = new AquaUSBConnection(params.getUsbDevice().getAbsolutePath());
		} else {
			connection = new AquaUDPConnection(params.getIp().getHostAddress(), params.getPort(), true);
		}
		device = new AquaDevice(connection);
		device.connect();
	}
	
	public void close() {
		if(device != null) {
			device.close();
		}
	}
	
	
	public void showAliveStatus() {
		if(params.isAlive()) {
			if(device.isAlive()) {
				System.out.println("Device is alive!");
			} else {
				System.out.println("Device is not alive!");	
			}
		}
	}
	
	public void showDatetime() {
		if(params.isGetDatetime()) {
			Date time = device.getTime();
			System.out.println(dateFormat.format(time));
		}		
	}
	
	public void setDatetime() {
		if(params.getNewTime() != null) {
			device.setTime(params.getNewTime());
			//System.out.println("New device time: " + dateFormat.format(device.getTime()));
		}
	}
	
	public void showTemperatures() {
		if(params.isGetTemperatures()) {
			float temp1 = device.getTemparature(Thermometer.THERMOMETER1);
			float temp2 = device.getTemparature(Thermometer.THERMOMETER2);
			
			System.out.printf("Temperature 1: %.2f \u00B0C\nTemperature 2: %.2f \u00B0C\n", temp1, temp2);
		}
	}
	
	public void showIlluminances() {
		if(params.isGetIlluminances()) {
			int illum1 = device.getIlluminance(Photoresistor.PHOTORESISTOR1);
			int illum2 = device.getIlluminance(Photoresistor.PHOTORESISTOR2);
			
			System.out.printf("Illuminance 1: %d lux\nIlluminance 2: %d lux\n", illum1, illum2);
		}
	}
	
	public void setBacklight() {
		if(params.getBacklight() > -1) {
			device.setBacklight(params.getBacklight());
		}
	}
	
	public void showBacklight() {
		if(params.isGetBacklight()) {
			int backlight = device.getBacklight();
			float percentBacklight = (float)((backlight / 255.0) * 100.0);
			
			System.out.printf("Current LCD backlight: %.2f%% (%d)\n", percentBacklight, backlight);
		}
	}
	
	public void setOutputState() {
		if(params.getState() != null) {
			int out = params.getOutput();
			Outputs o = Outputs.values()[out];
			device.getOutput(o).setState(OutputState.valueOf(params.getState().toUpperCase()));
			OutputState s = device.getOutput(o).getState();
			System.out.println("Current state: " + s.name().toLowerCase());
		}
	}
	
	public Outputs getOutputParam() {
		int out = params.getOutput();
		return Outputs.values()[out];
	}
	
	public DeviceOutput getDeviceOutput() {
		return device.getOutput(getOutputParam());
	}
	
	public void showOutputState() {
		if(params.isGetState()) {
			
			if(getOutputParam().compareTo(Outputs.OUTPUT0) == 0) {
				byte states = device.getOutputsState();
				
				System.out.printf("Output 1: %s\n", ((states >> 3) & 0x01) == 1 ? "on" : "off");
				System.out.printf("Output 2: %s\n", ((states >> 4) & 0x01) == 1 ? "on" : "off");
				System.out.printf("Output 3: %s\n", ((states >> 5) & 0x01) == 1 ? "on" : "off");
				System.out.printf("Output 4: %s\n", ((states >> 6) & 0x01) == 1 ? "on" : "off");
				System.out.printf("Output 5: %s\n", ((states >> 7) & 0x01) == 1 ? "on" : "off");
				
			} else {
				OutputState s = getDeviceOutput().getState();
				System.out.println(s.name().toLowerCase());
			}
		}		
	}
	
	public void setDeviceIp() {
		if(params.getDeviceIp() != null) {
			device.setIpAddress(params.getDeviceIp().getHostAddress());
		}
	}
	
	public void showDeviceIp() {
		if(params.isGetDeviceIp()) {
			String devIp = device.getIpAddress();
			System.out.println("Device IP address: " + devIp);
		}
	}
	
	public void setDevicePort() {
		if(params.getDevicePort() > 0) {
			device.setPort(params.getDevicePort());
		}
	}
	
	public void showDevicePort() {
		if(params.isGetDevicePort()) {
			int port = device.getPort();
			System.out.println("Device port: " + port);
		}
	}
	
	public void setOutputMode() {
		if(params.getOutputMode() > -1) {
			getDeviceOutput().setMode(OutputMode.values()[params.getOutputMode()]);
		}
	}
	
	public void showOutputMode() {
		if(params.isGetOutputMode()) {
			OutputMode mode = getDeviceOutput().getMode();
			System.out.println(mode.name().toLowerCase());
		}
	}
	
	public void setOutputTimer() {
		if(params.isSetOutputTimer()) {
			Date from = params.getFromDate();
			Date to = params.getToDate();
			OutputTimer outTimer = new OutputTimer();
			
			Calendar cal = Calendar.getInstance();
			cal.setTime(from);
			outTimer.setFromHour(cal.get(Calendar.HOUR));
			outTimer.setFromMinute(cal.get(Calendar.MINUTE));
			cal.setTime(to);
			outTimer.setToHour(cal.get(Calendar.HOUR));
			outTimer.setToMinute(cal.get(Calendar.MINUTE));

			getDeviceOutput().setTimer(outTimer);	
		}
	}
	
	public void showOutputTimer() {
		if(params.isGetOutputTimer()) {
			OutputTimer timer = getDeviceOutput().getTimer();
			System.out.printf("From: %02d:%02d To: %02d:%02d", timer.getFromHour(), timer.getFromMinute(), timer.getToHour(), timer.getToMinute());
		}
	}
	
	public void setOutputTemperature() {
		if(params.getOutputTemperature() > 0) {
			getDeviceOutput().setTemperature(params.getOutputTemperature());
		}
	}
	
	public void showOutputTemperature() {
		if(params.isGetOutputTemperature()) {
			float temp = getDeviceOutput().getTemperature();
			System.out.printf("%.2f \u00B0C\n",temp);
		}
	}
	
	public void setOutputHysteresis() {
		if(params.getOutputHysteresis() > 0) {
			getDeviceOutput().setHysteresis(params.getOutputHysteresis());
		}
	}
	
	public void showOutputHysteresis() {
		if(params.isGetOutputHysteresis()) {
			float hyst = getDeviceOutput().getHysteresis();
			System.out.printf("%.2f \u00B0C\n",hyst);
		}
	}
	
	public void setOutputIlluminance() {
		if(params.getOutputLuminance() > 0) {
			getDeviceOutput().setLuminance(params.getOutputLuminance());
		}
	}
	
	public void showOutputIlluminance() {
		if(params.isGetOutputLuminance()) {
			int lum = getDeviceOutput().getLuminance();
			System.out.printf("%d lux\n",lum);
		}
	}
	
	public void playMelody() {
		if(params.getMelody() > 0) {
			device.playMelody(Melody.values()[params.getMelody()], params.getMelodyRepeat());
		}
	}
	
	public void loadDefaultSettings() {
		if(params.isLoadDefault()){
			device.loadDefaultSettings();
		}
	}
	
	public void reboot() {
		if(params.isReboot()) {
			device.reboot();
		}
	}
	
	public void execute() {
		
		showAliveStatus();
		setDatetime();
		showDatetime();
		showTemperatures();
		showIlluminances();
		setBacklight();
		showBacklight();
		setOutputState();
		showOutputState();
		setDeviceIp();
		showDeviceIp();
		setDevicePort();
		showDevicePort();
		setOutputMode();
		showOutputMode();
		setOutputTimer();
		showOutputTimer();
		setOutputTemperature();
		showOutputTemperature();
		setOutputHysteresis();
		showOutputHysteresis();
		setOutputIlluminance();
		showOutputIlluminance();
		playMelody();
		loadDefaultSettings();
		reboot();
	}
	
	public static void main(String[] args) {
		
		AquaParams params = new AquaParams();
		Aqua aqua = new Aqua(params);
		aqua.parseArguments(args);
		
		try{
			aqua.connect();
			aqua.execute();
			aqua.close();
		} catch(DeviceException ex){
			System.err.println(ex.getMessage());
		}
	}
}
