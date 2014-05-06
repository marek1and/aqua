#include "settings.h"
#include "backlight.h"
#include "outputs.h"
#include "connection.h"

#include "../dev/hd44780.h"
#include "../net/network.h"

#include "../control.h"

#include <stdint.h>

void loadDefaultLCDBacklightValue(){
	setLCDBacklight(LCD_BACKLIGHT_DEFAULT_VALUE);
	saveLCDBacklighToMemory();
	LCD_SetBacklight(LCD_BACKLIGHT_DEFAULT_VALUE);
}

void loadDefaultOutputValues(uint8_t output, uint32_t timer, uint8_t mode, double temperature, double hysteresis, uint16_t luminance){
	setCurrentOutput(output);
	setCurrentOutputTime((uint8_t*)&timer);
	setCurrentOutputMode(mode);
	setCurrentOutputTemperature(temperature);
	setCurrentOutputHysteresis(hysteresis);
	setCurrentOutputLuminance(luminance);
	saveOutputSettings();
	clearOutputXSettings();
	clearOutputsSettings();
}

void loadDefaultOutputsValues(){
	loadDefaultOutputValues(OUTPUT1, OUTPUT1_TIMER_DEFAULT_VALUE, OUTPUT1_MODE_DEFAULT_VALUE, OUTPUT1_TEMPERATURE_DEFAULT_VALUE, OUTPUT1_HYSTERESIS_DEFAULT_VALUE, OUTPUT1_LUMINANCE_DEFAULT_VALUE);
	loadDefaultOutputValues(OUTPUT2, OUTPUT2_TIMER_DEFAULT_VALUE, OUTPUT2_MODE_DEFAULT_VALUE, OUTPUT2_TEMPERATURE_DEFAULT_VALUE, OUTPUT2_HYSTERESIS_DEFAULT_VALUE, OUTPUT2_LUMINANCE_DEFAULT_VALUE);
	loadDefaultOutputValues(OUTPUT3, OUTPUT3_TIMER_DEFAULT_VALUE, OUTPUT3_MODE_DEFAULT_VALUE, OUTPUT3_TEMPERATURE_DEFAULT_VALUE, OUTPUT3_HYSTERESIS_DEFAULT_VALUE, OUTPUT3_LUMINANCE_DEFAULT_VALUE);
	loadDefaultOutputValues(OUTPUT4, OUTPUT4_TIMER_DEFAULT_VALUE, OUTPUT4_MODE_DEFAULT_VALUE, OUTPUT4_TEMPERATURE_DEFAULT_VALUE, OUTPUT4_HYSTERESIS_DEFAULT_VALUE, OUTPUT4_LUMINANCE_DEFAULT_VALUE);
	loadDefaultOutputValues(OUTPUT5, OUTPUT5_TIMER_DEFAULT_VALUE, OUTPUT5_MODE_DEFAULT_VALUE, OUTPUT5_TEMPERATURE_DEFAULT_VALUE, OUTPUT5_HYSTERESIS_DEFAULT_VALUE, OUTPUT5_LUMINANCE_DEFAULT_VALUE);

	loadControlParametersFromMemory();
}

void loadDefaultConnectionSettings(){
	uint32_t ip = IP_ADDRESS_DEFAULT_VALUE;
	setConnectionIP((uint8_t*)&ip);
	setConnectionPort(PORT_DEFAULT_VALUE);

	saveConnectionParamsToMemory();

	setIPAddress(getConnectionIP());
	setPort(getConnectionPort());

	clearConnectionSetings();
}

void loadDefaultSettings(){
	loadDefaultLCDBacklightValue();
	loadDefaultOutputsValues();
	loadDefaultConnectionSettings();
}
