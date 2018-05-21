
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <string>
#include <map>
#include <iostream>

#include <vector>
#include <string>

#include "keycode.h"


int main()
{
	for ( size_t i = 0; i < getKeycodeStringValueCount(); i++ )
	{
		KeycodeStringValue *p = getKeycodeStringValue(i);
		printf("%s = 0x%04x\n", p->string, p->value);
	}
    return 0;
}

