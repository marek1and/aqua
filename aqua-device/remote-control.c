#include "proto.h"
#include "remote-control.h"
#include <stdint.h>

#include "dev/time.h"
#include "dev/ds18b20.h"
#include "dev/adc.h"
#include "dev/hd44780.h"
#include "dev/sound.h"
#include "settings/settings.h"
#include "settings/backlight.h"
#include "settings/outputs.h"
#include "settings/connection.h"
#include "net/network.h"
#include "control.h"

#include "utils/reboot.h"

uint8_t processProtoData(uint8_t *data, uint8_t dataLength){

	uint8_t size = 0;

	uint8_t out = getProtoOutput(data);
	uint8_t mode = getProtoMode(data);
	uint8_t func = getProtoFunction(data);
	uint8_t *protoData = getProtoData(data);
	//uint8_t protoDataLength = getProtoDataLength((uint8_t)dataLength);

	switch(func){
		case PROTO_FUNC_DATETIME:

			if(mode == PROTO_GET) {

				protoData[0] = getHour();
				protoData[1] = getMinute();
				protoData[2] = getSeconds();
				protoData[3] = getDay();
				protoData[4] = getMonth();
				protoData[5] = (getYear() >> 8) & 0x00FF;
				protoData[6] = getYear() & 0x00FF;

				size = 7 + PROTO_HEADER_LEN;

			} else if(mode == PROTO_SET) {
				uint8_t hour   = protoData[0];
				uint8_t minute = protoData[1];
				uint8_t seconds = protoData[2];
				uint8_t day    = protoData[3];
				uint8_t month  = protoData[4];
				uint16_t year  = ((protoData[5] << 8) & 0xFF00) | (protoData[6] & 0x00FF);

				writeTime(hour, minute, seconds);
				writeDate(day, month, (uint8_t)(year % 4));

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;
			}

			break;
		case PROTO_FUNC_TEMPERATURE:
			if(mode == PROTO_GET) {

				double temperature = getTemperatures()[out - 1];
				uint8_t *temp = (uint8_t*) &temperature;

				for(uint8_t i = 0; i < sizeof(double); i++){
					protoData[i] = temp[i];
				}

				size = sizeof(double) + PROTO_HEADER_LEN;

			} else {
				protoData[0] = PROTO_INVALID_OPTION;
				size = 1 + PROTO_HEADER_LEN;
			}

			break;
		case PROTO_FUNC_ILLUMINANCE:

			if(mode == PROTO_GET) {
;
				uint16_t illuminance = getIlluminances()[out - 1];
				protoData[0] = (illuminance >> 8) & 0xFF;
				protoData[1] = illuminance & 0xFF;

				size = sizeof(int) + PROTO_HEADER_LEN;

			} else {
				protoData[0] = PROTO_INVALID_OPTION;
				size = 1 + PROTO_HEADER_LEN;
			}

			break;
		case PROTO_FUNC_BACKLIGHT:
			if(mode == PROTO_GET) {

				protoData[0] = getLCDBackLight();
				size = 1 + PROTO_HEADER_LEN;

			} else if(mode == PROTO_SET) {

				setLCDBacklight(protoData[0]);
				LCD_SetBacklight(getLCDBackLight());

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;
			}
			break;

		case PROTO_FUNC_OUT_MODE:
			if(mode == PROTO_GET) {

				setCurrentOutput(out);
				initOutputXSettings();

				protoData[0] = getCurrentOutputMode();
				size = 1 + PROTO_HEADER_LEN;

				clearOutputXSettings();
				clearOutputsSettings();

			} else if(mode == PROTO_SET) {

				setCurrentOutput(out);
				initOutputXSettings();

				setCurrentOutputMode(protoData[0]);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				saveOutputSettings();
				clearOutputXSettings();
				clearOutputsSettings();
				readOutputControlParametersFromMemory(out);

			}
			break;

		case PROTO_FUNC_OUT_TIMER:
			if(mode == PROTO_GET) {

				setCurrentOutput(out);
				initOutputXSettings();

				 uint8_t *time = getCurrentOutputTime();

				for(uint8_t i = 0; i < 4; i++){
					protoData[i] = time[i];
				}

				size = 4 + PROTO_HEADER_LEN;

				clearOutputXSettings();
				clearOutputsSettings();

			} else if(mode == PROTO_SET) {

				setCurrentOutput(out);
				initOutputXSettings();

				setCurrentOutputTime(&protoData[0]);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				saveOutputSettings();
				clearOutputXSettings();
				clearOutputsSettings();
				readOutputControlParametersFromMemory(out);

			}
			break;

		case PROTO_FUNC_OUT_TEMPERATURE:

			if(mode == PROTO_GET) {

				setCurrentOutput(out);
				initOutputXSettings();

				double temperature = getCurrentOutputTemperature();
				uint8_t *temp = (uint8_t*) &temperature;


				for(uint8_t i = 0; i < sizeof(double); i++){
					protoData[i] = temp[i];
				}

				size = sizeof(double) + PROTO_HEADER_LEN;

				clearOutputXSettings();
				clearOutputsSettings();

			} else if(mode == PROTO_SET) {

				setCurrentOutput(out);
				initOutputXSettings();

				double *temp = (double*)&protoData[0];

				setCurrentOutputTemperature(*temp);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				saveOutputSettings();
				clearOutputXSettings();
				clearOutputsSettings();
				readOutputControlParametersFromMemory(out);

			}

			break;

		case PROTO_FUNC_OUT_HYSTERESIS:

			if(mode == PROTO_GET) {

				setCurrentOutput(out);
				initOutputXSettings();

				double hysteresis = getCurrentOutputHysteresis();
				uint8_t *hyst = (uint8_t*) &hysteresis;


				for(uint8_t i = 0; i < sizeof(double); i++){
					protoData[i] = hyst[i];
				}

				size = sizeof(double) + PROTO_HEADER_LEN;

				clearOutputXSettings();
				clearOutputsSettings();

			} else if(mode == PROTO_SET) {

				setCurrentOutput(out);
				initOutputXSettings();

				double *hyst = (double*)&protoData[0];

				setCurrentOutputHysteresis(*hyst);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				saveOutputSettings();
				clearOutputXSettings();
				clearOutputsSettings();
				readOutputControlParametersFromMemory(out);

			}

			break;

		case PROTO_FUNC_OUT_LUMINANCE:

			if(mode == PROTO_GET) {

				setCurrentOutput(out);
				initOutputXSettings();

				uint16_t illuminance = getCurrentOutputLuminance();
				protoData[0] = (illuminance >> 8) & 0xFF;
				protoData[1] = illuminance & 0xFF;

				size = sizeof(uint16_t) + PROTO_HEADER_LEN;

				clearOutputXSettings();
				clearOutputsSettings();

			} else if(mode == PROTO_SET) {

				setCurrentOutput(out);
				initOutputXSettings();

				uint16_t illum = (protoData[0] << 8) | protoData[1];

				setCurrentOutputLuminance(illum);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				saveOutputSettings();
				clearOutputXSettings();
				clearOutputsSettings();
				readOutputControlParametersFromMemory(out);

			}

			break;
		case PROTO_FUNC_IP_ADDRESS:

			if(mode == PROTO_GET) {

				loadConnectionParamsFromMemory();
				uint8_t *ip = getConnectionIP();

				for(uint8_t i = 0; i < 4; i++){
					protoData[i] = ip[i];
				}
				size = 4 + PROTO_HEADER_LEN;

				clearConnectionSetings();

			} else if(mode == PROTO_SET) {

				loadConnectionParamsFromMemory();
				setConnectionIP(&protoData[0]);
				saveConnectionParamsToMemory();
				setIPAddress(&protoData[0]);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				clearConnectionSetings();

			}

			break;
		case PROTO_FUNC_PORT:

			if(mode == PROTO_GET) {

				loadConnectionParamsFromMemory();
				protoData[0] = getConnectionPort();
				size = 1 + PROTO_HEADER_LEN;

				clearConnectionSetings();

			} else if(mode == PROTO_SET) {

				loadConnectionParamsFromMemory();
				setConnectionPort(protoData[0]);
				saveConnectionParamsToMemory();
				setPort(protoData[0]);

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;

				clearConnectionSetings();
			}

			break;
		case PROTO_FUNC_CONTROL:

			if(mode == PROTO_GET) {

				if(out == 0){
					protoData[0] = getStates();
				} else{
					protoData[0] = getState(out);
				}
				size = 1 + PROTO_HEADER_LEN;

			} else if(mode == PROTO_SET) {

				if(out == 0){
					setOutputs(protoData[0]);
				} else{
					setOutput(out, protoData[0]);
				}

				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;
			}

			break;
		case PROTO_FUNC_DEFAULT_SETTINGS:

			loadDefaultSettings();
			protoData[0] = PROTO_SUCCESS;
			size = 1 + PROTO_HEADER_LEN;

			break;
		case PROTO_FUNC_PLAY_MELODY:

			setRepeatCount(protoData[0]);
			setMelody(out);
			setPlaying(PLAYING);
			protoData[0] = PROTO_SUCCESS;
			size = 1 + PROTO_HEADER_LEN;

			break;
		case PROTO_FUNC_DEVICE_RESET:

			if(out == PROTO_OUT0 && mode == PROTO_WRITE){
				reboot();
			}

			break;
		case PROTO_FUNC_IS_ALIVE:

			if(out == PROTO_OUT0 && mode == PROTO_READ){
				protoData[0] = PROTO_SUCCESS;
				size = 1 + PROTO_HEADER_LEN;
			} else {
				protoData[0] = PROTO_INVALID_OPTION;
				size = 1 + PROTO_HEADER_LEN;
			}
			break;
		default:
			protoData[0] = PROTO_INVALID_OPTION;
			size = 1 + PROTO_HEADER_LEN;
			break;
	}
	return size;
}



