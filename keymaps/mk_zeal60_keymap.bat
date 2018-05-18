@echo off

zeal60 backlight_config_set_values ^
use_split_backspace=0 ^
use_split_left_shift=0 ^
use_split_right_shift=1 ^
use_7u_spacebar=0 ^
use_iso_enter=0 ^
disable_hhkb_blocker_leds=0 ^
disable_when_usb_suspended=1 ^
disable_after_timeout=200 ^
brightness=50 ^
effect=11 ^
effect_speed=3 ^
color_1=180,100,100 ^
color_2=300,100,100 ^
caps_lock_indicator_color=65,100,100 ^
caps_lock_indicator_row_col=all ^
layer_1_indicator_color=180,100,100 ^
layer_1_indicator_row_col=0,0 ^
layer_2_indicator_color=300,100,100 ^
layer_2_indicator_row_col=all ^
layer_3_indicator_color=0,150,100 ^
layer_3_indicator_row_col=none

zeal60 backlight_config_set_alphas_mods ^
MOD      ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    MOD     ^
MOD      ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    MOD     ^
MOD      ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    MOD      MOD     ^
MOD      ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    ALPHA    MOD      MOD     ^
MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD      MOD   

zeal60 colors ^
SPEC=hsv(65,100,100) ^
ALPHA=hsv(180,100,100) ^
MODS=hsv(300,100,100) ^
SPEC     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     MODS    ^
MODS     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     MODS    ^
MODS     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     SPEC      SPEC    ^
MODS     MODS      ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     ALPHA     MODS      MODS    ^
MODS     MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS      MODS    

zeal60 keymap 0 ^
KC_ESC   KC_1     KC_2     KC_3     KC_4     KC_5     KC_6     KC_7     KC_8     KC_9     KC_0     KC_MINS  KC_EQL   KC_BSPC ^
KC_TAB   KC_Q     KC_W     KC_E     KC_R     KC_T     KC_Y     KC_U     KC_I     KC_O     KC_P     KC_LBRC  KC_RBRC  KC_BSLS ^
KC_CAPS  KC_A     KC_S     KC_D     KC_F     KC_G     KC_H     KC_J     KC_K     KC_L     KC_SCLN  KC_QUOT  KC_ENT   KC_NO   ^
KC_LSFT  KC_NO    KC_Z     KC_X     KC_C     KC_V     KC_B     KC_N     KC_M     KC_COMM  KC_DOT   KC_SLSH  KC_UP    TG(2)   ^
KC_LCTL  KC_LGUI  KC_LALT  KC_NO    KC_NO    KC_NO    KC_NO    KC_SPC   KC_NO    KC_NO    MO(1)    KC_LEFT  KC_DOWN  KC_RIGHT

zeal60 keymap 1 ^
KC_GRV   KC_F1    KC_F2    KC_F3    KC_F4    KC_F5    KC_F6    KC_F7    KC_F8    KC_F9    KC_F10   KC_F11   KC_F12   KC_DEL  ^
RESET	 EF_DEC   EF_INC   H1_DEC   H1_INC   H2_DEC   H2_INC   KC_TRNS  KC_TRNS  KC_TRNS  KC_PSCR  KC_VOLD  KC_VOLU  KC_TRNS ^
KC_TRNS  BR_DEC   BR_INC   S1_DEC   S1_INC   S2_DEC   S2_INC   ES_DEC   ES_INC   KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  RGB_TOG  RGB_MOD  RGB_HUD  RGB_HUI  RGB_SAD  RGB_SAI  RGB_VAD  RGB_VAI  KC_TRNS  KC_TRNS  KC_PGUP  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_HOME  KC_PGDN  KC_END

zeal60 keymap 2 ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_RSFT  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_RALT  KC_RGUI  MO(1)    KC_RCTL

zeal60 keymap 3 ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS ^
KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS  KC_TRNS

rem pause