#pragma once

#include <stdint.h>
#include <stdio.h>


// We expose the internals so we can use it for dumping QMK keycodes
// for use in JavaScript

struct KeycodeStringValue
{
	uint16_t value;
	char *string;
};

KeycodeStringValue *getKeycodeStringValue(size_t index);
size_t getKeycodeStringValueCount();

bool stringToValue( const char *string, uint16_t *value );
