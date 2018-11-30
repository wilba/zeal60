
- Program "zeal60_default.hex" using Flip
- Disconnect USB
- Connect USB
- Double-click on one of the "zeal60_keymap_*.bat" files to program a layout

Depending on the layout, Fn1 and Fn2 are assigned to right Win and Menu keys, or to split-shift "Fn" key and right Win key.

The default backlight controls are on layer 3, which is (by default) enabled by holding down Fn1 and Fn2

1 / 2 - change effect
 0= All Off
 1= Solid Color 1
 2= Alphas/Mods Color 1/2
 3= Gradient Vertical Color 1/2 
 4= Raindrops Color 1/2
 5= Cycle All
 6= Cycle Horizontal
 7= Cycle Vertical
 8= Jellybean Raindrops
 9= Radial All Hues
10= Radial Color 1

11= Custom Key Colors

3 / 4 - color 1 hue -/+
E / R - color 1 sat -/+
5 / 6 - color 2 hue -/+
T / Y - color 2 sat -/+
- / + - brightness -/+
[ / ] - effect speed -/+


ALSO:

When the zeal60 firmware is programmed, holding Esc down while powering on (i.e. connecting USB to keyboard) will reset ALL the stored settings (keymaps, backlight, etc.) and start the bootloader so it is ready for firmware programming with Flip. If you only want to reset the stored settings, disconnect/connect USB and the firmware is restarted.





zeal60.exe is a command-line tool for changing keymaps and backlight configuration.

The preferred way to use it is by editing a "batch" file, which will execute zeal60.exe and pass it information via command-line arguments.

NOTE: Batch files use the ^ symbol to break a single line into multiple lines. Make sure these are at the end of each line when the command-line arguments span multiple lines.

The example batch files should be used as a base for changing things to your preference.

Copy a batch file and then edit it to your preference.
Then double-click the batch file to execute the batch file.
A command-line window will open to show progress.


zeal60 Commands:

The following explains what commands are being used in the batch files to achieve backlight configuration and keymap configuration.

zeal60 backlight_config_set_values [ name=value ] ... 

Name/value pairs:

use_split_backspace=<0|1>
use_split_left_shift=<0|1> 
use_split_right_shift=<0|1> 
use_7u_spacebar=<0|1> 
use_iso_enter=<0|1> 

Configures which switches are being used, so that LEDs not under a switch will be disabled.
It needs to be combined with soldering the jumpers on the PCB to match the switches being used, to connect one of two LEDs to the LED matrix.
0 is false, 1 is true. e.g. use_split_backspace=1 enables split backspace LEDs.

disable_hhkb_blocker_leds=<0|1>

Disable the LEDs under the "blockers" of an HHKB layout.

disable_when_usb_suspended=<0|1>

Turn off backlight when the USB is suspended, i.e. when the PC is in Sleep/Hibernate/Shutdown mode.

disable_after_timeout=N

Turn off backlight after no keypresses for N minutes.

brightness=N

Global brightness setting, N in range 0-100

effect=N

Backlight effect, N in range 0-9 (see above for list)

effect_speed=N

Backlight effect speed, N in range 0-3

color_1=H,S,V
color_2=H,S,V

Set the colors used in backlight effects

caps_lock_indicator_color=H,S,V
layer_1_indicator_color=H,S,V
layer_2_indicator_color=H,S,V
layer_3_indicator_color=H,S,V

Set the color used for Caps Lock and active layer indicators

caps_lock_indicator_row_col=<ROW,COL|all|none>
layer_1_indicator_row_col=<ROW,COL|all|none>
layer_2_indicator_row_col=<ROW,COL|all|none>
layer_3_indicator_row_col=<ROW,COL|all|none>

Set the LEDs used for Caps Lock and active layer indicators.
ROW/COL specify which switch in the switch matrix

e.g.
ANSI Caps Lock Key:  caps_lock_indicator_row_col=0,2
Spacebar+stabs:      caps_lock_indicator_row_col=4,7
Esc Key:             caps_lock_indicator_row_col=0,0
Left Win Key:        caps_lock_indicator_row_col=4,1
Right Win Key:       caps_lock_indicator_row_col=4,11
Right Menu Key:      caps_lock_indicator_row_col=4,12
HHKB Fn Key:         caps_lock_indicator_row_col=3,13
All keys:            caps_lock_indicator_row_col=all 
None:                caps_lock_indicator_row_col=none

Note: Use layer_3_indicator_row_col=none if you use the default layer 3 keymaps which change the backlight.


zeal60 backlight_config_set_alphas_mods [ ALPHA | MOD ] ...

This command configures which switches in the switch matrix are alphanumeric keys and which are modifiers.
This is used in backlight effects which set the LEDs under alphanumeric and modifier keys to different colors.


zeal60 keymap <layer> <keycode> ...

This command sets the keycodes in a keymap. layer is 0 to 3. keycode is a QMK keycode name, e.g. KC_ESC, KC_1, KC_2 etc.

This is how the default keymaps are configured:
Keymap 0 is the default keymap, what you expect keys to do when not holding down a Fn key.
Keymap 1 is the Fn1 keymap, what you expect keys to do when holding down Fn1 key.
Keymap 2 is the Fn2 keymap, what you expect keys to do when holding down Fn2 key.
Keymap 3 is the "configuration" keymap, what you expect keys to do when holding down Fn1 and Fn2 keys simultaneously.

The switch matrix is 5 rows and 14 columns.
Due to split keys, ISO, winkey/winkeyless support, some positions in the keymap will be empty/not used.
Here are the row/column positions of multiple-position keys:

[0,13] is either left key of split backspace (e.g. HHKB \| key) or 2U backspace
[1,13] is either backslash or ISO Enter
[2,12] is either ANSI Enter or key left of ISO Enter
[2,13] is right key of split backspace (e.g. HHKB `~ key)
[3,1] is right key of split left-shift (e.g ISO key)
[3,13] is right key of split right-shift (e.g. HHKB Fn key)
[4,7] is space bar

NOTE: the least obvious of the above is the right key of the split backspace, which is to the right of the ANSI Enter.

Keycode names match the ones QMK uses, so in a lot of cases, you can copy them from existing QMK keymap.c files.

NOTE: Keycode KC_NO means don't output any keycode when this switch is pressed. It is present in the batch files to show switches which would not be present for that layout, like keys on an ISO keyboard not being present on an ANSI keyboard.

NOTE: Keycode KC_TRNS means when this layer is active, use the keycode for the layer "below" it. Use it in keymaps 1, 2 and 3 to fill in the positions where you don't have a keycode you want for that layer, especially in the position "above" keycodes that change layers, like Fn keys.

Refer to QMK Firmware documentation for keycode symbols:

https://docs.qmk.fm/

and a general understanding of how the keymaps work.

Special QMK keycodes like MO(layer), LT(layer,kc), LCTRL(kc), etc. will work, however they must not include any spaces when they appear in the batch file.
You CAN nest them, likesuchas LALT(LCTL(KC_DEL))

Avoid using DF(layer) unless you REALLY know what you are doing.

QMK keycodes that specify multiple mod keys need a ^ before the |  e.g. OSM(MOD_LCTL^|MOD_LSFT^|MOD_LALT)


Some Interesting QMK keycodes:

KC_PWR - PC power down (or sleep)
KC_SLEP - PC sleep
RESET - start the bootloader so new firmware can be programmed with Flip
-- useful for power users that build their own firmware, casual users can use Space + Esc while powering on

KC_MUTE - mute
KC_VOLU - volume up
KC_VOLD - volume down
KC_CALC - open calculator

MO(1) - Momentary enable layer 1 while held
LT(1,KC_CAPS) - Enable layer 1 while held, Caps Lock when tapped


Zeal60 Special Keycodes:

FN_MO13 - hold for layer 1, hold with FN_MO23 for layer 3
FN_MO23 - hold for layer 2, hold with FN_MO13 for layer 3
-- Use both these keycodes in a layout to get Fn1 + Fn2 = Fn3 functionality

FN_TT13 - hold for layer 1, triple-tap to toggle on/off layer 3
FN_TT23 - hold for layer 2, triple-tap to toggle on/off layer 3
TG_NKRO - toggle NKRO on/off and store the setting

Zeal60 Backlight Control Keycodes:

EF_DEC - backlight effect decrement
EF_INC - backlight effect increment
H1_DEC - color 1 hue decrement
H1_INC - color 1 hue increment
H2_DEC - color 2 hue decrement
H2_INC - color 2 hue increment
BR_DEC - brightness decrement
BR_INC - brightness increment
S1_DEC - color 1 saturation decrement
S1_INC - color 1 saturation increment
S2_DEC - color 2 saturation increment
S2_INC - color 2 saturation increment

