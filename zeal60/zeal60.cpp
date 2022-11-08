
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <string>
#include <map>
#include <iostream>

#include <vector>
#include <string>

// Headers needed for sleeping.
#ifdef _WIN32
#include <windows.h>
#include "hidapi.h"
#else
#include <unistd.h>
#include "hidapi/hidapi.h"
#endif

#ifdef _MSC_VER
// Stop compiler bitching about sscanf() being unsafe.
// Prefer the portable sscanf() to the suggested, MSVC-only sscanf_c()
#pragma warning(disable:4996)
#endif

#include "config.h"
#include "keycode.h"

// ../../qmk_firmware/ is in include path
//#include "quantum/action.h"
typedef int keyrecord_t;
#include "quantum/via.h"
#include "keyboards/wilba_tech/wt_rgb_backlight_api.h"
#include "quantum/color.h"

#ifdef _DEBUG
#define LOG(x) std::err << x
#else

#endif

#define RAMA_WORKS_HACK

bool parse_hsv_color_string( const char *string, HSV *color )
{
	std::string s = string;

	if ( s.find( "hsv(" ) == 0 &&
		s.find( ")" ) == s.length() - 1 )
	{
		std::string values = s.substr( 4, s.length() - 1 );
		int h, s, v;
		if ( sscanf( values.c_str(), "%d,%d,%d", &h, &s, &v ) == 3 )
		{
			color->h = uint8_t( double( h ) / 360.0 * 255.0 );
			color->s = uint8_t( double( s ) / 100.0 * 255.0 );
			color->v = uint8_t( double( v ) / 100.0 * 255.0 );
			return true;
		}
	}
	return false;
}

bool parse_hsv_color_string2( const char *string, HSV *color )
{
	std::string values = string;

	int h, s, v;
	if ( sscanf( values.c_str(), "%d,%d,%d", &h, &s, &v ) == 3 )
	{
		color->h = uint8_t( double( h ) / 360.0 * 255.0 );
		color->s = uint8_t( double( s ) / 100.0 * 255.0 );
		color->v = uint8_t( double( v ) / 100.0 * 255.0 );
		return true;
	}
	return false;
}

bool parse_indicator_row_column( const char *string, int *row, int *column )
{
	std::string s = string;
	int r, c;
	if ( s == "none" )
	{
		*row = 255;
		*column = 255;
		return true;
	}
	else if ( s == "all" )
	{
		*row = 254;
		*column = 254;
		return true;
	}
	else if ( sscanf( s.c_str(), "%d,%d", &r, &c ) == 2 )
	{
		*row = r;
		*column = c;
		return true;
	}
	return false;
}

hid_device *
hid_open( unsigned short vendor_id, unsigned short product_id, unsigned short interface_number )
{
	hid_device *device = NULL;
	struct hid_device_info *deviceInfos;
	struct hid_device_info *currentDeviceInfo;
	struct hid_device_info *foundDeviceInfo = NULL;
	deviceInfos = hid_enumerate( vendor_id, product_id );
	currentDeviceInfo = deviceInfos;
	while ( currentDeviceInfo )
	{
		if ( currentDeviceInfo->interface_number == interface_number )
		{
			if ( foundDeviceInfo )
			{
				// More than one matching device.
				// TODO: return error?
			}
			else
			{
				foundDeviceInfo = currentDeviceInfo;
			}
		}
		currentDeviceInfo = currentDeviceInfo->next;
	}

	if ( foundDeviceInfo )
	{
		device = hid_open_path( foundDeviceInfo->path );
	}

	hid_free_enumeration(deviceInfos);

	return device;
}

std::vector<std::string>
hid_get_device_paths( unsigned short vendor_id, unsigned short product_id, unsigned short interface_number )
{
	std::vector<std::string> devicePaths;
	struct hid_device_info *deviceInfos;
	struct hid_device_info *currentDeviceInfo;
	struct hid_device_info *foundDeviceInfo = NULL;
	deviceInfos = hid_enumerate( vendor_id, product_id );
	currentDeviceInfo = deviceInfos;
	while ( currentDeviceInfo )
	{
		if ( currentDeviceInfo->interface_number == interface_number )
		{
			devicePaths.push_back( currentDeviceInfo->path );
		}
		currentDeviceInfo = currentDeviceInfo->next;
	}

	hid_free_enumeration(deviceInfos);

	return devicePaths;	
}

bool send_message( hid_device *device, uint8_t id, void *outMsg = NULL, uint8_t outMsgLength = 0, void *retMsg = NULL, uint8_t retMsgLength = 0 )
{
	//assert( outMsgLength <= RAW_HID_BUFFER_SIZE );
	if ( outMsgLength > RAW_HID_BUFFER_SIZE )
	{
		printf("Message size %d is bigger than maximum %d\n", outMsgLength, RAW_HID_BUFFER_SIZE);
		return false;
	}

	int res;
	uint8_t data[RAW_HID_BUFFER_SIZE + 1];
	memset( data, 0xFE, sizeof( data ) );
	data[0] = 0x00; // NULL report ID. IMPORTANT!
	data[1] = id;

	if ( outMsg && outMsgLength > 0 )
	{
		memcpy( &data[2], outMsg, outMsgLength );
	}

	res = 0;
	res = hid_write( device, data, RAW_HID_BUFFER_SIZE + 1 );
	if ( res < 0 )
	{
		printf( "Unable to write()\n" );
		printf( "Error: %ls\n", hid_error( device ) );
		return false;
	}

	hid_set_nonblocking( device, 1 );

	res = 0;
	memset( data, 0xFE, sizeof( data ) );
	// Timeout after 500ms
	for ( int i=0; i<500; i++ )
	{
		res = hid_read( device, data, RAW_HID_BUFFER_SIZE );
		if ( res != 0 )
		{
			break;
		}
		// waiting
#ifdef WIN32
		Sleep( 1 );
#else
		usleep( 1 * 1000 );
#endif
	}

	if ( res < 0 )
	{
		printf( "Unable to read()\n" );
		printf( "Error: %ls\n", hid_error( device ) );
		return false;
	}

	if ( res > 0 )
	{
		if ( data[1] == id_unhandled )
		{
			return false;
		}

		if ( retMsg && retMsgLength > 0 )
		{
			memcpy( retMsg, &data[1], retMsgLength );
		}
	}

	return true;
}

bool get_protocol_version( hid_device *device, uint16_t *protocol_version )
{
	uint8_t msg[2];
	if ( send_message( device, id_get_protocol_version, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*protocol_version = ( msg[0] << 8 ) | msg[1];
		return true;
	}
	return false;
}

bool get_keyboard_value_uint32( hid_device *device, uint8_t value_id, uint32_t *value )
{
	uint8_t msg[5];
	msg[0] = value_id;
	msg[1] = 0xFF;
	msg[2] = 0xFF;
	msg[3] = 0xFF;
	msg[4] = 0xFF;
	if ( send_message( device, id_get_keyboard_value, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*value = ( msg[1] << 24 ) | ( msg[2] << 16 ) | ( msg[3] << 8 ) | msg[4];
		return true;
	}
	return false;
}

bool dynamic_keymap_get_keycode( hid_device *device, uint8_t layer, uint8_t row, uint8_t column, uint16_t *keycode )
{
	uint8_t msg[5];
	msg[0] = layer;
	msg[1] = row;
	msg[2] = column;
	msg[3] = 0xFF;
	msg[4] = 0xFF;
	if ( send_message( device, id_dynamic_keymap_get_keycode, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*keycode = ( msg[3] << 8 ) | msg[4];
		return true;
	}
	return false;
}

bool dynamic_keymap_set_keycode( hid_device *device, uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode )
{
	uint8_t msg[5];
	msg[0] = layer;
	msg[1] = row;
	msg[2] = column;
	msg[3] = keycode >> 8;
	msg[4] = keycode & 0xFF;
	return send_message( device, id_dynamic_keymap_set_keycode, msg, sizeof(msg) );
}

bool dynamic_keymap_reset( hid_device *device )
{
	return send_message( device, id_dynamic_keymap_reset );
}

bool dynamic_keymap_macro_get_count( hid_device *device, uint8_t *count )
{
	uint8_t msg[1];
	if ( send_message( device, id_dynamic_keymap_macro_get_count, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*count = msg[0];
		return true;
	}
	return false;
}

bool dynamic_keymap_macro_get_buffer_size( hid_device *device, uint16_t *buffer_size )
{
	uint8_t msg[2];
	if ( send_message( device, id_dynamic_keymap_macro_get_buffer_size, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*buffer_size = ( msg[0] << 8 ) | msg[1];
		return true;
	}
	return false;
}

bool dynamic_keymap_macro_get_buffer( hid_device *device, uint8_t *buffer, uint16_t buffer_size )
{
	// Raw HID packet size is 32
	// 1 byte command + 3 bytes for args (offset, size)
	// leaves 28 bytes size
	const uint16_t max_size = 28;
	uint8_t msg[31]; // max_size + 3

	uint16_t offset = 0;
	uint8_t size = 0;
	for ( offset = 0; offset < buffer_size; offset += max_size )
	{
		// clip last read
		uint8_t size = min( buffer_size - offset, max_size );

		msg[0] = offset >> 8;
		msg[1] = offset & 0xFF;
		msg[2] = size;

		if ( send_message( device, id_dynamic_keymap_macro_get_buffer, msg, sizeof(msg), msg, sizeof(msg) ) )
		{
			memcpy( buffer+offset, &msg[3], size );
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool dynamic_keymap_macro_set_buffer( hid_device *device, uint8_t *buffer, uint16_t buffer_size )
{
	// Raw HID packet size is 32
	// 1 byte command + 3 bytes for args (offset, size)
	// leaves 28 bytes size
	const uint16_t max_size = 28;
	uint8_t msg[31]; // max_size + 3


	// Set last byte of buffer to non-zero
	{
		msg[0] = (buffer_size-1) >> 8;
		msg[1] = (buffer_size-1) & 0xFF;
		msg[2] = 1;
		msg[3] = 0xFF; // non-zero

		if ( ! send_message( device, id_dynamic_keymap_macro_set_buffer, msg, sizeof(msg), msg, sizeof(msg) ) )
		{
			return false;
		}
	}

	uint16_t offset = 0;
	uint8_t size = 0;
	for ( offset = 0; offset < buffer_size; offset += max_size )
	{
		// clip last write
		uint8_t size = min( buffer_size - offset, max_size );

		msg[0] = offset >> 8;
		msg[1] = offset & 0xFF;
		msg[2] = size;
		memcpy( &msg[3], buffer+offset, size );

		if ( send_message( device, id_dynamic_keymap_macro_set_buffer, msg, sizeof(msg), msg, sizeof(msg) ) )
		{
		}
		else
		{
			return false;
		}
	}

	// Set last byte of buffer to zero
	{
		msg[0] = (buffer_size-1) >> 8;
		msg[1] = (buffer_size-1) & 0xFF;
		msg[2] = 1;
		msg[3] = 0x00; // zero

		if ( ! send_message( device, id_dynamic_keymap_macro_set_buffer, msg, sizeof(msg), msg, sizeof(msg) ) )
		{
			return false;
		}
	}

	return true;
}

bool dynamic_keymap_macro_reset( hid_device *device )
{
	return send_message( device, id_dynamic_keymap_macro_reset );
}


bool backlight_config_set_value_uint8( hid_device *device, uint8_t value_id, uint8_t value )
{
	uint8_t msg[2];
	msg[0] = value_id;
	msg[1] = value;
	return send_message( device, id_lighting_set_value, msg, sizeof(msg) );
}

bool backlight_config_set_value_bool( hid_device *device, uint8_t value_id, bool value )
{
	return backlight_config_set_value_uint8( device, value_id, value ? 0x01 : 0x00 );
}
bool backlight_config_set_value_uint8_2( hid_device *device, uint8_t value_id, uint8_t value1, uint8_t value2 )
{
	uint8_t msg[3];
	msg[0] = value_id;
	msg[1] = value1;
	msg[2] = value2;
	return send_message( device, id_lighting_set_value, msg, sizeof(msg) );
}

bool backlight_config_set_value_HSV( hid_device *device, uint8_t value_id, HSV value )
{
	uint8_t msg[4];
	msg[0] = value_id;
	msg[1] = value.h;
	msg[2] = value.s;
	msg[3] = value.v;
	return send_message( device, id_lighting_set_value, msg, sizeof(msg) );
}

bool backlight_config_set_alphas_mods( hid_device *device, uint16_t *alphas_mods )
{
	uint8_t msg[11];
	msg[0] = id_alphas_mods;
	for ( int i=0; i<5; i++ )
	{
		msg[1+(i*2)+0] = alphas_mods[i] >> 8;
		msg[1+(i*2)+1] = alphas_mods[i] & 0xFF;
	}
	return send_message( device, id_lighting_set_value, msg, sizeof(msg) );
}

bool backlight_config_save(  hid_device *device )
{
	return send_message( device, id_lighting_save );
}

bool backlight_config_get_value_uint32( hid_device *device, uint8_t value_id, uint32_t *value )
{
	uint8_t msg[5];
	msg[0] = value_id;
	msg[1] = 0xFF;
	msg[2] = 0xFF;
	msg[3] = 0xFF;
	msg[4] = 0xFF;
	if ( send_message( device, id_lighting_get_value, msg, sizeof(msg), msg, sizeof(msg) ) )
	{
		*value = ( msg[1] << 24 ) | ( msg[2] << 16 ) | ( msg[3] << 8 ) | msg[4];
		return true;
	}
	return false;
}




hid_device *
hid_open_least_uptime( unsigned short vendor_id, unsigned short product_id, unsigned short interface_number )
{
	std::vector<std::string> devicePaths = hid_get_device_paths( vendor_id, product_id, interface_number );

	// early abort
	if ( devicePaths.size() == 0 )
	{
		return NULL;
	}

	// no need to check ticks
	if ( devicePaths.size() == 1 )
	{
		return hid_open_path( devicePaths[0].c_str() ); 
	}

	std::string bestDevicePath;
	uint32_t bestDeviceTick = 0;

	for ( int i=0; i<(int)devicePaths.size(); i++ )
	{
		hid_device *device = hid_open_path( devicePaths[i].c_str() );
		
		uint16_t protocolVersion;
		if ( !get_protocol_version( device, &protocolVersion ) )
		{
			std::cerr << "*** Error: Error getting protocol version" << std::endl;
			hid_close( device );
			continue;
			//return 0;
		}

		if ( protocolVersion != VIA_PROTOCOL_VERSION )
		{
			std::cerr << "*** Error: Device uses protocol version " << protocolVersion << std::endl;
			std::cerr << "This program uses protocol version " << VIA_PROTOCOL_VERSION << std::endl;
			hid_close( device );
			continue;
			//return 0;
		}

		uint32_t thisDeviceTick = 0;
		if ( !get_keyboard_value_uint32( device, id_uptime, &thisDeviceTick ) )
		{
			std::cerr << "*** Error: Error getting uptime" << std::endl;
			hid_close( device );
			continue;
		}

		if ( bestDevicePath.empty() || thisDeviceTick < bestDeviceTick )
		{
			bestDevicePath = devicePaths[i];
			bestDeviceTick = thisDeviceTick;
		}

		hid_close( device );
	}

	if ( !bestDevicePath.empty() )
	{
		return hid_open_path( bestDevicePath.c_str() );
	}

	return NULL;
}

void
hid_test(void)
{
	struct hid_device_info *devs, *cur_dev;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");

		if ( cur_dev->interface_number == 1 )
		{
			hid_device *device = hid_open_path( cur_dev->path );
			if ( device )
			{
				uint16_t protocolVersion = 0x0000;
				if ( get_protocol_version( device, &protocolVersion ) )
				{
					printf("get_protocol_version() returned %04x\n", protocolVersion );
				}
				else
				{
					printf("get_protocol_version() failed\n" );
				}
				hid_close( device );
			}
			else
			{
				printf("hid_open_path() failed\n" );
			}

			printf("\n");
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
}

int main(int argc, char **argv)
{
	if (hid_init())
	{
		std::cerr << "*** Error: hidapi initialization failed" << std::endl;
		return -1;
	}

	if (argc <= 1)
	{
		// No args, do nothing
		return 0;
	}

	// First arg is the command
	std::string command = argv[1];

	if ( command == "hidtest" )
	{
		hid_test();
		return 0;
	}

	hid_device *device = hid_open_least_uptime( DEVICE_VID, DEVICE_PID, DEVICE_INTERFACE_NUMBER );
#ifdef RAMA_WORKS_HACK
	// Extreme hack, zeal60.exe being used to find a RAMA M60-A device
	// This is a short-term workaround until I refactor the code...
	// Really don't want to make a "m60a.exe" that's essentially zeal60.exe compiled with different VID/PID
	if ( !device && DEVICE_VID == 0x5A45 && DEVICE_PID == 0x0060)
	{
		device = hid_open_least_uptime( 0x5241, 0x060A, DEVICE_INTERFACE_NUMBER );
	}
	// Ditto for zeal65.exe and KOYU
	if ( !device && DEVICE_VID == 0x5A45 && DEVICE_PID == 0x0065)
	{
		device = hid_open_least_uptime( 0x5241, 0x4B59, DEVICE_INTERFACE_NUMBER );
	}
#endif
	if ( ! device )
	{
		std::cerr << "*** Error: Device not found" << std::endl;
		return -1;
	}

	bool res = false;
	uint16_t protocolVersion;
	res = get_protocol_version( device, &protocolVersion );
	if ( ! res )
	{
		std::cerr << "*** Error: Error getting protocol version" << std::endl;
		hid_close( device );
		return -1;
	}

	if ( protocolVersion != VIA_PROTOCOL_VERSION )
	{
		std::cerr << "*** Error: Device uses protocol version " << protocolVersion << std::endl;
		std::cerr << "This program uses protocol version " << VIA_PROTOCOL_VERSION << std::endl;
		hid_close( device );
		return -1;
	}

	if ( command == "debug" )
	{

		while ( true )
		{
			uint32_t thisDeviceTick = 0;
			if ( !get_keyboard_value_uint32( device, id_uptime, &thisDeviceTick ) )
			{
				std::cerr << "*** Error: Error getting uptime" << std::endl;
				hid_close( device );
				return -1;
			}
			printf("%08hx\n", thisDeviceTick);
			Sleep( 1000 );
		}
		hid_close( device );
		return 0;	
	}
	else if ( command == "eeprom_reset" )
	{
		res = send_message( device, id_eeprom_reset );
		if ( ! res )
		{
			std::cerr << "*** Error: Error resetting EEPROM" << std::endl;
			hid_close( device );
			return -1;
		}

		hid_close( device );
		std::cout << "EEPROM reset" << std::endl;
		return 0;
	}
	else if ( command == "bootloader_jump" )
	{
		res = send_message( device, id_bootloader_jump );
		if ( ! res )
		{
			std::cerr << "*** Error: Error jumping to bootloader" << std::endl;
			hid_close( device );
			return -1;
		}	
		
		hid_close( device );
		std::cout << "Jumped to bootloader" << std::endl;
		return 0;
	}
	else if ( command == "backlight_config_set_value" ||
			 command == "backlight_config_set_values" )
	{
		for ( int i = 2; i < argc; i++ )
		{
			std::string s = argv[i];

			// Handle "name=value" cases first
			size_t equalPos = s.find( "=" );
			if ( equalPos == std::string::npos )
			{
				std::cerr << "*** Error: Invalid value '" << s << "', not in form 'name=value'" << std::endl;
				return -1;
			}

			std::string name = s.substr( 0, equalPos );
			std::string value = s.substr( equalPos + 1 );
			int intValue = 0;
			HSV hsvValue;
			int row,column = 0;

			if ( name == "use_split_backspace" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_use_split_backspace, intValue != 0 );
			}
			else if ( name == "use_split_left_shift" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_use_split_left_shift, intValue != 0 );
			}
			else if ( name == "use_split_right_shift" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_use_split_right_shift, intValue != 0 );
			}
			else if ( name == "use_7u_spacebar" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_use_7u_spacebar, intValue != 0 );
			}
			else if ( name == "use_iso_enter" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_use_iso_enter, intValue != 0 );
			}
			else if ( name == "disable_hhkb_blocker_leds" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_disable_hhkb_blocker_leds, intValue != 0 );
			}
			else if ( name == "disable_when_usb_suspended" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_bool( device, id_disable_when_usb_suspended, intValue != 0 );
			}
			else if ( name == "disable_after_timeout" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_uint8( device, id_disable_after_timeout, intValue);
			}
			else if ( name == "brightness" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_uint8( device, id_brightness, uint8_t( double( intValue ) / 100.0 * 255.0 ) );
			}
			else if ( name == "effect" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_uint8( device, id_effect, intValue);
			}
			else if ( name == "effect_speed" &&
				sscanf( value.c_str(), "%d", &intValue ) == 1 )
			{
				res = backlight_config_set_value_uint8( device, id_effect_speed, intValue);
			}
			else if ( name == "color_1" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_color_1, hsvValue );
			}
			else if ( name == "color_2" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_color_2, hsvValue );
			}
			else if ( name == "caps_lock_indicator_color" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_caps_lock_indicator_color, hsvValue );
			}
			else if ( name == "layer_1_indicator_color" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_layer_1_indicator_color, hsvValue );
			}
			else if ( name == "layer_2_indicator_color" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_layer_2_indicator_color, hsvValue );
			}
			else if ( name == "layer_3_indicator_color" &&
				parse_hsv_color_string2( value.c_str(), &hsvValue ) )
			{
				res = backlight_config_set_value_HSV( device, id_layer_3_indicator_color, hsvValue );
			}
			else if ( name == "caps_lock_indicator_row_col" &&
				parse_indicator_row_column( value.c_str(), &row, &column ) )
			{
				res = backlight_config_set_value_uint8_2( device, id_caps_lock_indicator_row_col, row, column );
			}
			else if ( name == "layer_1_indicator_row_col" &&
				parse_indicator_row_column( value.c_str(), &row, &column ) )
			{
				res = backlight_config_set_value_uint8_2( device, id_layer_1_indicator_row_col, row, column );
			}
			else if ( name == "layer_2_indicator_row_col" &&
				parse_indicator_row_column( value.c_str(), &row, &column ) )
			{
				res = backlight_config_set_value_uint8_2( device, id_layer_2_indicator_row_col, row, column );
			}
			else if ( name == "layer_3_indicator_row_col" &&
				parse_indicator_row_column( value.c_str(), &row, &column ) )
			{
				res = backlight_config_set_value_uint8_2( device, id_layer_3_indicator_row_col, row, column );
			}
			else
			{
				std::cerr << "*** Error: Invalid name/value '" << s << "'" << std::endl;
				return -1;
			}

			if ( ! res )
			{
				std::cerr << "*** Error: Error setting backlight config values" << std::endl;
				hid_close( device );
				return -1;
			}
		}

		// Save backlight config to EEPROM
		backlight_config_save( device );

		hid_close( device );
		std::cout << "Backlight config values set" << std::endl;
		return 0;
	}
	else if ( command == "backlight_config_set_alphas_mods" )
	{
		if ( argc != 2 + MATRIX_ROWS * MATRIX_COLS )
		{
			std::cerr << "*** Error: Invalid number of arguments for '" << command << "'" << std::endl;
			return -1;
		}
		uint16_t alphas_mods[5];
		int arg = 2;
		for ( int row = 0; row < MATRIX_ROWS; row++ )
		{
			alphas_mods[row] = 0;
			for ( int col = 0; col < MATRIX_COLS; col++ )
			{
				std::string s = argv[arg];
				if ( s == "ALPHA" )
				{
					// leave it 0
				}
				else if ( s == "MOD" )
				{
					alphas_mods[row] |= (1<<col);
				}
				else
				{
					std::cerr << "Invalid alphas/mods string '" << argv[arg] << "'" << std::endl;
					return -1;
				}

				arg++;
			}
		}

		if (!backlight_config_set_alphas_mods( device, alphas_mods ) )
		{
			std::cerr << "*** Error: Error setting backlight config alpha/mods" << std::endl;
			hid_close( device );
			return -1;
		}

		// Save backlight config to EEPROM
		backlight_config_save( device );

		hid_close( device );
		std::cout << "Backlight config alphas/mods set" << std::endl;
		return 0;
	}
	else if (command == "keymap")
	{
		if (argc != 2 + 1 + MATRIX_ROWS * MATRIX_COLS)
		{
			std::cerr << "*** Error: Invalid number of arguments for '" << command << "'" << std::endl;
			return -1;
		}

		int layer = atoi(argv[2]);
		if ( layer < 0 || layer > 3)
		{
			std::cerr << "*** Error: Invalid layer '" << argv[2] << "'" << std::endl;
			return -1;
		}

		int keymap[MATRIX_ROWS][MATRIX_COLS];
		int arg = 3;
		for ( int row = 0; row < MATRIX_ROWS; row++ )
		{
			for (int col = 0; col < MATRIX_COLS; col++)
			{
				uint16_t value;
				if ( stringToValue(argv[arg], &value ) )
				{
					keymap[row][col] = value;
				}
				else
				{
					std::cerr << "*** Error: Invalid keycode '" << argv[arg] << "'" << std::endl;
					return -1;
				}

				arg++;
			}
		}

		for (int row = 0; row < MATRIX_ROWS; row++)
		{
			for (int column = 0; column < MATRIX_COLS; column++)
			{

				if ( ! dynamic_keymap_set_keycode( device, layer, row, column, keymap[row][column] ) )
				{
					std::cerr << "*** Error: Error saving keymap layer " << layer << " row " << row << " column " << column << std::endl;
					hid_close( device );
					return -1;
				}
			}
		}

		hid_close( device );
		std::cout << "Keymap layer " << layer << " saved" << std::endl;
		return 0;
	}
	else if (command == "get_keymap")
	{
		if (argc != 2 + 1)
		{
			std::cerr << "*** Error: Invalid number of arguments for '" << command << "'" << std::endl;
			return -1;
		}
		int layer = atoi(argv[2]);
		if ( layer < 0 || layer > 3)
		{
			std::cerr << "*** Error: Invalid layer '" << argv[2] << "'" << std::endl;
			return -1;
		}

		uint16_t keymap[MATRIX_ROWS][MATRIX_COLS];
		int arg = 3;
		for ( int row = 0; row < MATRIX_ROWS; row++ )
		{
			for (int column = 0; column < MATRIX_COLS; column++)
			{
				if ( ! dynamic_keymap_get_keycode( device, layer, row, column, &(keymap[row][column]) ) )
				{
					std::cerr << "*** Error: Error getting keymap layer " << layer << " row " << row << " column " << column << std::endl;
					hid_close( device );
					return -1;
				}
			}
		}

		for ( int row = 0; row < MATRIX_ROWS; row++ )
		{
			for (int column = 0; column < MATRIX_COLS; column++)
			{
				std::string keycodeString = valueToString( keymap[row][column] );
				printf("%-7s ", keycodeString.c_str());
			}
			printf("\n");
		}

		hid_close( device );
		return 0;
	}
	else if ( command == "get_macro_buffer" )
	{
		uint16_t buffer_size = 0;
		res = dynamic_keymap_macro_get_buffer_size( device, &buffer_size );
		if ( ! res )
		{
			std::cerr << "*** Error: Error getting macro buffer size" << std::endl;
			hid_close( device );
			return -1;
		}

		if ( buffer_size >= 1024 )
		{
			std::cerr << "*** Error: Error getting macro buffer size, it's >1024" << std::endl;
			hid_close( device );
			return -1;
		}

		// Big enough
		uint8_t buffer[1024];
		res = dynamic_keymap_macro_get_buffer( device, buffer, buffer_size );
		if ( ! res )
		{
			std::cerr << "*** Error: Error getting macro buffer" << std::endl;
			hid_close( device );
			return -1;
		}

		hid_close( device );
		return 0;
	}
	else if ( command == "set_macro_buffer" )
	{
		uint16_t buffer_size = 0;
		res = dynamic_keymap_macro_get_buffer_size( device, &buffer_size );
		if ( ! res )
		{
			std::cerr << "*** Error: Error getting macro buffer size" << std::endl;
			hid_close( device );
			return -1;
		}

		if ( buffer_size >= 1024 )
		{
			std::cerr << "*** Error: Error getting macro buffer size, it's >1024" << std::endl;
			hid_close( device );
			return -1;
		}

		// Big enough
		uint8_t buffer[1024];
		// Pad with zeroes
		memset(buffer,0,1024);

		// TODO: some actual compiling of N macros into a single buffer.
		// For now, just copy from a string

		char debug_macro_string[] = "macro00\0macro01\0macro02\0macro03\0macro04\0macro05\0macro06\0macro07\0macro08\0macro09\0macro10\0macro11\0macro12\0macro13\0macro14\0macro15\0";
		memcpy(buffer,debug_macro_string,sizeof(debug_macro_string));

		res = dynamic_keymap_macro_set_buffer( device, buffer, buffer_size );
		if ( ! res )
		{
			std::cerr << "*** Error: Error setting macro buffer" << std::endl;
			hid_close( device );
			return -1;
		}

		hid_close( device );
		return 0;
	}

	std::cerr << "*** Error: Invalid command '" << command << "'" << std::endl;
	return -1;
}

