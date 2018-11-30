
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

/*

{
  "KC_A": 0x0004,
  "KC_B":  0x0005,
  ...
}

*/

int main()
{
	printf("{\n");
	for ( size_t i = 0; i < getKeycodeStringValueCount(); i++ )
	{
		KeycodeStringValue *p = getKeycodeStringValue(i);
		printf("  %s: 0x%04x%s\n", p->string, p->value, i<getKeycodeStringValueCount()-1?",":"");
	}
	printf("}\n");
    return 0;
}
