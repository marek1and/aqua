#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"
#include "utils/timer.h"
#include "utils/reboot.h"

#include "dev/hd44780.h"
#include "dev/twi.h"
#include "dev/usart.h"
#include "dev/adc.h"
#include "dev/ds18b20.h"
#include "dev/relay.h"
#include "dev/buttons.h"
#include "dev/time.h"
#include "dev/eeprom.h"
#include "dev/sound.h"

#include "net/network.h"

#include "settings/settings.h"
#include "settings/outputs.h"
#include "settings/backlight.h"
#include "settings/connection.h"
#include "settings/time-settings.h"

#include "control.h"
#include "proto.h"
#include "usb-proto.h"
#include "remote-control.h"


#define MAIN_SCREEN      						0

#define DATETIME_SETTINGS_SCREEN				1
#define DATETIME_SCREEN							11
#define OUT_TIMER_SETTINGS_SCREEN				2
#define OUT_TIMER_SCREEN						21
#define OUTX_TIMER_SCREEN						22
#define LCD_BACKLIGHT_SETTINGS_SCREEN			3
#define LCD_BACKLIGHT_SCREEN					31
#define CONNECTION_SETTINGS_SCREEN				4
#define CONNECTION_SETTINGS_PARAM_SCREEN		41
#define CONNECTION_PARAM_SCREEN					42
#define DEFAULT_SETTINGS_SCREEN					5
#define DEFAULT_SETTINGS_CONFIRM_SCREEN			51
#define OUTPUT_CONTROL_SCREEN					6
#define TEMPERATURE_ILLUMINANCE_SCREEN			7


#define BUFFER_SIZE 100
static uint8_t ethBuffer[BUFFER_SIZE];

//static uint8_t  myMACAddress[6] = {'0','A','Q','U','A','0'};
static uint8_t  myIPAddress[4]  = {192,168,1,10};
static uint16_t myPort = 10;

static int screen = 0;
static int clearScreenFlag = 0;

static int readTemperaturesFlag = 1;

void setReadTemperaturesFlag(){
	readTemperaturesFlag = 1;
}

void readMeasurements(){
	readTime();
	adcConvert();

	if(readTemperaturesFlag){
		readTemperatures();
		readTemperaturesFlag = 0;
		_delay_ms(115);
		scheduleTimerTask(5, setReadTemperaturesFlag);
	} else {
		_delay_ms(490);
	}

}

void setScreen(int scr){
	screen = scr;
	clearScreenFlag = 1;
}

void prevScreen(){
	screen--;
}

void nextScreen(){
	screen++;
}

void goToMainScreen(){
	setReadTemperaturesFlag();
	setScreen(MAIN_SCREEN);
}

void showTemperatureOnLCD(double temp, char posX, char posY){
	char temperature[6];
	dtostrf(temp, 5, 2, temperature);
	LCD_GoTo(posX,posY);
	LCD_WriteText(temperature);
	LCD_WriteText("\xdf""C");
}

void showIlluminanceOnLCD(double illum, char posX, char posY){

	char illuminance[5];
	dtostrf(illum, 5, 0, illuminance);
	LCD_GoTo(posX,posY);
	LCD_WriteText(illuminance);
	LCD_GoTo(posX + 6,posY);
	LCD_WriteText("lx");
}

void showTimeOnLCD(uint8_t *time){
	LCD_GoTo(0,0);
	char t[2];
	for(int i=0;i<10;i++){
		itoa(time[i],t,10);
		LCD_WriteText(t);
		if(i == 1 || i==3){LCD_WriteText(":");}
		if(i == 5){LCD_GoTo(11,0);}
		if(i == 7){LCD_WriteText("/");}
	}
}

void showMainScreen(){

	uint8_t *time = getTime();
	showTimeOnLCD(time);

	if(isADCEnabled() == ADC_ENABLED){
		showTemperatureOnLCD(getTemperatures()[0], 0, 1);
		showIlluminanceOnLCD(getIlluminances()[0], 8, 1);
	}else {
		//showTemperatureOnLCD(getTemperatures()[1], 9, 1);
		showTemperatureOnLCD(getTemperatures()[0], 0, 1);
	}
}

void showDatetimeSettingsScreen(){
	LCD_Home();
	LCD_GoTo(3, 0);
	LCD_WriteText("Czas/Data");
}

void showDatetimeScreen(){
	LCD_Home();

	LCD_WriteText("Czas:");
	uint8_t *tmpTime = getTmpTime();

	LCD_GoTo(11,0);
	char tmp[4];

	for(uint8_t i = 0; i <= YEAR; i++){
		if(i == DAY){
			LCD_GoTo(0,1);
			LCD_WriteText("Data: ");
		}

		if(i == YEAR){
			itoa((CALENDAR_START_YEAR + tmpTime[i]),tmp, 10);
		} else{
			itoa(tmpTime[i],tmp, 10);
		}

		if(i == HOUR || i == MINUTE || i == DAY || i == MONTH){
			if(tmpTime[i] < 10){
				LCD_WriteText("0");
			}
		}
		LCD_WriteText(tmp);
		if(i == HOUR){
			LCD_WriteText(":");
		}else if(i == DAY || i == MONTH){
			LCD_WriteText("/");
		}
	}
}

void showOutputsSettingsScreen(){
	LCD_Home();
	LCD_GoTo(4, 0);
	LCD_WriteText("Wyj\x04""cia");
}

void showOutputsScreen(){
	LCD_Home();
	LCD_GoTo(3, 0);
	LCD_WriteText("Wyj\x04""cie ");

	int out = getCurrentOutput();
	char t[1];
	itoa(out, t, 10);

	LCD_WriteText(t);
}

void showOutputXScreen(){
	LCD_Home();

	uint8_t mode = getCurrentOutputMode();
	uint8_t *timers = getCurrentOutputTime();
	double temp = getCurrentOutputTemperature();
	double hist = getCurrentOutputHysteresis();
	uint16_t lum = getCurrentOutputLuminance();
	char t[5];

	LCD_WriteText("Tryb");
	LCD_GoTo(0,1);

	int showMode = 0;

	switch(mode){
		case OUT_MODE_MANUAL:
			LCD_WriteText("Manual");
			break;
		case OUT_MODE_ON:
			LCD_WriteText("On");
			break;
		case OUT_MODE_OFF:
			LCD_WriteText("Off");
			break;
		case OUT_MODE_TIMER:
			LCD_WriteText("Timer");
			showMode = 1;
			break;
		case OUT_MODE_TEMP1_L:
			LCD_WriteText("< T1");
			showMode = 2;
			break;
		case OUT_MODE_TEMP1_G:
			LCD_WriteText("> T1");
			showMode = 2;
			break;
		case OUT_MODE_TEMP2_L:
			LCD_WriteText("< T2");
			showMode = 2;
			break;
		case OUT_MODE_TEMP2_G:
			LCD_WriteText("> T2");
			showMode = 2;
			break;
		case OUT_MODE_LUM1_L:
			LCD_WriteText("< L1");
			showMode = 3;
			break;
		case OUT_MODE_LUM1_G:
			LCD_WriteText("> L1");
			showMode = 3;
			break;
		case OUT_MODE_LUM2_L:
			LCD_WriteText("< L2");
			showMode = 3;
			break;
		case OUT_MODE_LUM2_G:
			LCD_WriteText("> L2");
			showMode = 3;
			break;
	}

	if(showMode == 1){
		for(int i=0; i < OUT_TO_MINUTE + 1; i++){

			if(i == OUT_FROM_HOUR){
				LCD_GoTo(7, 0);
				LCD_WriteText("Od: ");
			} else if (i == OUT_TO_HOUR){
				LCD_GoTo(7, 1);
				LCD_WriteText("Do: ");
			}

			if(timers[i] < 10){
				LCD_WriteText("0");
			}

			itoa(timers[i],t,10);
			LCD_WriteText(t);

			if(i == OUT_FROM_HOUR || i == OUT_TO_HOUR){
				LCD_WriteText(":");
			}
		}
	}else if(showMode == 2){
		LCD_GoTo(6, 0);
		LCD_WriteText("T: ");
		dtostrf(temp, 5, 2, t);
		LCD_WriteText(t);
		LCD_WriteText("\xdf""C");

		LCD_GoTo(6, 1);
		LCD_WriteText("H: ");
		dtostrf(hist, 5, 2, t);
		LCD_WriteText(t);
		LCD_WriteText("\xdf""C");

	}else if(showMode == 3){
		LCD_GoTo(6, 0);
		LCD_WriteText("L: ");
		itoa(lum, t, 10);
		LCD_WriteText(t);
		LCD_WriteText(" lux");
	}
}

void drawProgressBar(int light){

	LCD_GoTo(0,1);
	for(int i=0;i<16;i++){
		if(i==0)LCD_WriteData(1);
		else if(i==15)LCD_WriteData(2);
		else LCD_WriteData(0);
	}
	LCD_GoTo(0,1);

	int iterations = (light/16);
	if(light == 255){
		iterations = 16;
	}

	for(int i = 0; i < iterations; i++){
		LCD_WriteData(255);
	}
}

void showLCDBacklightSettingsScreen(){
	LCD_Home();
	LCD_GoTo(1, 0);
	LCD_WriteText("Pod\x04wietlenie");
}

void showLCDBacklightScreen(){
	showLCDBacklightSettingsScreen();
	drawProgressBar(getLCDBackLight());
}

uint8_t getStateChar(uint8_t data, int output){
	if((data >> output) & 0x01){
		return 6;
	} else {
		return 7;
	}
}

void showConnectionSettingsScreen(){
	LCD_Home();
	LCD_GoTo(2, 0);
	LCD_WriteText("Komunikacja");
}

void showConnectionParamSettingsScreen(){
	LCD_Home();

	uint8_t param = getConnectionParam();
	switch(param){
		case CONNECTION_PARAM_IP:
			LCD_GoTo(4, 0);
			LCD_WriteText("Adres IP");
			break;
		case CONNECTION_PARAM_PORT:
			LCD_GoTo(6, 0);
			LCD_WriteText("Port");
			break;
	}
}

void showConnectionParamScreen(){
	LCD_Home();

	uint8_t param = getConnectionParam();
	char t[3];

	switch(param){
		case CONNECTION_PARAM_IP:
			LCD_GoTo(4, 0);
			LCD_WriteText("Adres IP");
			LCD_GoTo(2,1);

			uint8_t *ip = getConnectionIP();

			for(uint8_t i = 0; i < 4; i++){
				itoa(ip[i], t, 10);
				LCD_WriteText(t);
				if(i < 3){
					LCD_WriteText(".");
				}
			}

			break;
		case CONNECTION_PARAM_PORT:
			LCD_GoTo(6, 0);
			LCD_WriteText("Port");
			LCD_GoTo(7,1);

			uint8_t p = getConnectionPort();
			itoa(p, t, 10);
			LCD_WriteText(t);

			break;
	}
}

void showDefaultSettingsScreen(){
	LCD_Home();
	LCD_GoTo(3, 0);
	LCD_WriteText("Ustawienia");
	LCD_GoTo(4, 1);
	LCD_WriteText("domy\x04lne");
}

void showDefaultSettingsConfirmScreen(){
	LCD_Home();
	LCD_GoTo(2, 0);
	LCD_WriteText("Za\x03""aduj ust.");
	LCD_GoTo(4, 1);
	LCD_WriteText("domy\x04lne");

}

void showOutputControlScreen(){

	LCD_Home();

	uint8_t data = readRelays();

	LCD_GoTo(3,0);
	LCD_WriteText("1 2 3 4 5");
	LCD_GoTo(3,1);

	for(int out = RELAY1; out <= RELAY5; out++){
		LCD_WriteData(getStateChar(data, out));
		LCD_WriteText(" ");
	}
}

void showTemperatureIlluminanceScreen(){

	showTemperatureOnLCD(getTemperatures()[0], 0, 0);
	showTemperatureOnLCD(getTemperatures()[1], 0, 1);
	showIlluminanceOnLCD(getIlluminances()[0], 8,0);
	showIlluminanceOnLCD(getIlluminances()[1], 8,1);

}

void playMelody(){
	wdt_disable();
	play();
	wdt_enable(WDTO_2S);
}

void press(uint8_t btn){

	switch(screen){
		case MAIN_SCREEN:

			switch(btn){
				case S1:
					nextScreen();
					break;
				case S2:
					setScreen(DEFAULT_SETTINGS_SCREEN);
					break;
				case S3:
					setScreen(TEMPERATURE_ILLUMINANCE_SCREEN);
					break;
				case S4:
					setScreen(OUTPUT_CONTROL_SCREEN);
					scheduleTimerTask(4, goToMainScreen);
					break;
			}

			break;
		case DATETIME_SETTINGS_SCREEN:

			switch(btn){
				case S1:
					nextScreen();
					break;
				case S2:
					setScreen(DEFAULT_SETTINGS_SCREEN);
					break;
				case S3:
					setScreen(DATETIME_SCREEN);
					initTimeSettings();
					break;
				case S4:
					setScreen(MAIN_SCREEN);
					break;
			}

			break;
		case DATETIME_SCREEN:
			switch(btn){
				case S1:
					saveTimeSettings();
					goToMainScreen();
					break;
				case S2:
					increaseTimeParam();
					break;
				case S3:
					nextTimeParam();
					break;
				case S4:
					clearTimeSettings();
					setScreen(MAIN_SCREEN);
					break;
			}
			break;
		case OUT_TIMER_SETTINGS_SCREEN:

			switch(btn){
				case S1:
					nextScreen();
					break;
				case S2:
					prevScreen();
					break;
				case S3:
					setScreen(OUT_TIMER_SCREEN);
					initOutputsSettings();
					break;
				case S4:
					setScreen(MAIN_SCREEN);
					break;
			}

			break;
		case OUT_TIMER_SCREEN:

				switch(btn){
					case S1:
						increaseCurrentOutput();
						break;
					case S2:
						decreaseCurrentOutput();
						break;
					case S3:
						setScreen(OUTX_TIMER_SCREEN);
						initOutputXSettings();
						break;
					case S4:
						clearOutputsSettings();
						setScreen(OUT_TIMER_SETTINGS_SCREEN);
						break;
				}

				break;
		case OUTX_TIMER_SCREEN:

				switch(btn){
					case S1:
						setScreen(OUT_TIMER_SCREEN);
						saveOutputSettings();
						clearOutputXSettings();
						_delay_ms(50);
						loadControlParametersFromMemory();
						break;
					case S2:
						increaseOutputParam();
						break;
					case S3:
						increaseOutputParamIdx();
						break;
					case S4:
						clearOutputXSettings();
						setScreen(OUT_TIMER_SCREEN);
						break;
				}

				break;

			break;
		case LCD_BACKLIGHT_SETTINGS_SCREEN:

			switch(btn){
				case S1:
					nextScreen();
					break;
				case S2:
					prevScreen();
					break;
				case S3:
					setScreen(LCD_BACKLIGHT_SCREEN);
					break;
				case S4:
					setScreen(MAIN_SCREEN);
					break;
			}

			break;
		case LCD_BACKLIGHT_SCREEN:

			switch(btn){
				case S1:
					saveLCDBacklighToMemory();
					goToMainScreen();
					break;
				case S2:
					increaseLCDBacklight();
					break;
				case S3:
					decreaseLCDBacklight();
					break;
				case S4:
					loadLCDBacklightFromMemory();
					setScreen(LCD_BACKLIGHT_SETTINGS_SCREEN);
					break;
			}
			LCD_SetBacklight(getLCDBackLight());

			break;

		case CONNECTION_SETTINGS_SCREEN:

			switch(btn){
				case S1:
					nextScreen();
					break;
				case S2:
					prevScreen();
					break;
				case S3:
					setScreen(CONNECTION_SETTINGS_PARAM_SCREEN);
					initConnectionSettings();
					break;
				case S4:
					goToMainScreen();
					break;
			}

			break;

		case CONNECTION_SETTINGS_PARAM_SCREEN:

			switch(btn){
				case S1:
					nextConnectionParam();
					break;
				case S2:
					prevConnectionParam();
					break;
				case S3:
					setScreen(CONNECTION_PARAM_SCREEN);
					break;
				case S4:
					setScreen(CONNECTION_SETTINGS_SCREEN);
					clearConnectionSetings();
					break;
			}

			break;

		case CONNECTION_PARAM_SCREEN:

			switch(btn){
				case S1:
					saveConnectionParamsToMemory();
					setIPAddress(getConnectionIP());
					setPort(getConnectionPort());
					clearConnectionSetings();
					goToMainScreen();
					break;
				case S2:
					increaseConnectionParam();
					break;
				case S3:
					increaseConnectionIPIdx();
					decreaseConnectionParam();
					break;
				case S4:
					setScreen(CONNECTION_SETTINGS_PARAM_SCREEN);
					break;
			}

			break;

		case DEFAULT_SETTINGS_SCREEN:

			switch(btn){
				case S1:
					setScreen(DATETIME_SETTINGS_SCREEN);
					break;
				case S2:
					prevScreen();
					break;
				case S3:
					setScreen(DEFAULT_SETTINGS_CONFIRM_SCREEN);
					break;
				case S4:
					goToMainScreen();
					break;
			}

			break;

		case DEFAULT_SETTINGS_CONFIRM_SCREEN:

			switch(btn){
				case S1:
					loadDefaultSettings();
					goToMainScreen();
					break;
				case S2:
				case S3:
				case S4:
					setScreen(DEFAULT_SETTINGS_SCREEN);
					break;
			}

			break;

		case OUTPUT_CONTROL_SCREEN:

			scheduleTimerTask(4, goToMainScreen);

			switch(btn) {
				case S1:
					negateOutput(OUTPUT1);
					break;
				case S2:
					negateOutput(OUTPUT2);
					break;
				case S3:
					negateOutput(OUTPUT3);
					break;
				case S4:
					negateOutput(OUTPUT4);
					break;
			}
			break;
			case TEMPERATURE_ILLUMINANCE_SCREEN:
				goToMainScreen();
				break;
		default:
			goToMainScreen();
	}
}

void release(uint8_t btn){
	LCD_Clear();
}

void loadConnectionParams() {
	_delay_ms(5);
	readDataBlockFromMemory(IP_PARAM_ADDR, myIPAddress, 4);
	_delay_ms(5);
	myPort = readDataFromMemory(PORT_PARAM_ADDR);
}

void loadParams(){
	loadLCDBacklightFromMemory();
	loadControlParametersFromMemory();
	loadConnectionParams();
}

void init(){

	wdt_enable(WDTO_2S);

	buttonsInit();
	LCD_Initalize();
	clockInit();
	loadParams();

	USART_init();
	//networkInit(myIPAddress, myMACAddress, myPort);
	OneWireInit();

	adcInit();
	soundInit();

	LCD_SetBacklight(getLCDBackLight());

	wdt_reset();

	sei();
}

int main(void){

	init();

	while(1){

		if(clearScreenFlag){
			LCD_Clear();
			clearScreenFlag = 0;
		}

		if(isPlaying()){
			playMelody();
		}

		switch(screen){
			case MAIN_SCREEN:

				readMeasurements();
				showMainScreen();

				break;
			case DATETIME_SETTINGS_SCREEN:

				showDatetimeSettingsScreen();

				break;
			case DATETIME_SCREEN:

				showDatetimeScreen();

				break;
			case OUT_TIMER_SETTINGS_SCREEN:

				showOutputsSettingsScreen();

				break;
			case OUT_TIMER_SCREEN:

				showOutputsScreen();

				break;
			case OUTX_TIMER_SCREEN:

				showOutputXScreen();

				break;
			case LCD_BACKLIGHT_SETTINGS_SCREEN:

				showLCDBacklightSettingsScreen();

				break;
			case LCD_BACKLIGHT_SCREEN:

				showLCDBacklightScreen();

				break;

			case CONNECTION_SETTINGS_SCREEN:

				showConnectionSettingsScreen();

				break;

			case CONNECTION_SETTINGS_PARAM_SCREEN:

				showConnectionParamSettingsScreen();

				break;

			case CONNECTION_PARAM_SCREEN:

				showConnectionParamScreen();

				break;
			case DEFAULT_SETTINGS_SCREEN:

				showDefaultSettingsScreen();

				break;
			case DEFAULT_SETTINGS_CONFIRM_SCREEN:

				showDefaultSettingsConfirmScreen();

				break;
			case OUTPUT_CONTROL_SCREEN:

				showOutputControlScreen();

				break;
			case TEMPERATURE_ILLUMINANCE_SCREEN:

				readMeasurements();
				showTemperatureIlluminanceScreen();

				break;
			default:
				goToMainScreen();
		}

		readButton(S1, press, release);
		readButton(S2, press, release);
		readButton(S3, press, release);
		readButton(S4, press, release);

		controlOutputs(getTemperatures(), getIlluminances());

		wdt_reset();
	}

	return 0;
}

// WOL packet received
// Reset MCU by Watchdog
ISR(INT0_vect) {
	reboot();
}

// Ethernet packet received
ISR(INT1_vect) {

	uint16_t plen;
	plen = receivePacket(BUFFER_SIZE, ethBuffer);

	if(plen > 0){
		uint8_t ethType = getEthernetDataProtocol(ethBuffer);
		if(ethType == IP){

			if(isMyIP(ethBuffer)){
				uint8_t ipProto = getIPDataProtocol(ethBuffer);
				if(ipProto == ICMP && ethBuffer[ICMP_TYPE] == ICMP_TYPE_ECHOREQUEST){
					doPingReply(ethBuffer, plen);
				}else if(ipProto == UDP){
					uint16_t len = (((ethBuffer[UDP_LEN] << 8) & 0xFF00) | (ethBuffer[UDP_LEN + 1] & 0x00FF)) - UDP_HEADER_LEN;
					uint8_t l = processProtoData(&ethBuffer[UDP_DATA], (uint8_t)len);
					replyUDPPacket(ethBuffer, &ethBuffer[UDP_DATA], l);
				}
			}
		} else if(ethType == ARP){
			if(isMyArpIP(ethBuffer)){
				doArpAnswer(ethBuffer);
			}
		}
	}
}

ISR(USART_RXC_vect){
	uint8_t receivedByte = UDR;
	processUSBByte(receivedByte);
}
