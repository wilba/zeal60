# Zeal60 Command Line Tool

This is the command line tool for changing keymaps and backlight settings for the Zeal60 PCB.

Zeal60 PCB uses firmware based on [QMK firmware](https://github.com/jackhumbert/qmk_firmware) with the required changes to support RGB backlight and changing keymaps via the PC.
Unlike typical QMK usage, users will not need to change QMK source files, build and program new firmware. They only need to upload the latest "zeal60" firmware and use this tool to change keymaps and backlight settings.

See [readme.txt](/readme.txt) for instructions on how to use this tool.

## Latest Build

The latest official build of this tool, and the matching QMK firmware, can be found at [ZealPC.net](https://zealpc.net/)

## Build Instructions

The fork of QMK firmware with code to support this tool is found here: [https://github.com/Wilba6582/qmk_firmware/tree/zeal60](https://github.com/Wilba6582/qmk_firmware/tree/zeal60)

NOTE: it is in the "zeal60" branch not the "master" branch.

The source code of the tool will include files from the qmk_firmware directory, so this clone and the qmk_firmware clone must have the same parent directory.

Use Visual Studio 2015 (Community edition works, and is free).

The last built executable will be copied into the keymaps directory, so the batch files there will use it.

If you're interested in porting this code to Linux or OSX, let me know.

### Building under Linux

Make sure you have installed the hidapi library before. You can copy the compiled binary to your path (for example /usr/local/bin/).

You may also be able to use the version of `hdiapi` from your distribution's package manager. Ubuntu would be these:

```
sudo apt install libusb-1.0-0-dev libhidapi-dev libhidapi-libusb0
```

### Building under macOS

Currently the hidapi library does not support interface numbers for devices in macOS. There is an
[open pull request](https://github.com/signal11/hidapi/pull/380) to add interface number support for HID devices only.

Follow the instructions below to compile hidapi with interface number support:

```
git clone https://github.com/signal11/hidapi.git
cd hidapi
git remote add dylanmckay https://github.com/dylanmckay/hidapi.git
git fetch dylanmckay
git checkout mac-hid-interface-support
./bootstap
./configure
make
sudo make install
```

After compileing hidapi, you may build zeal60 using the Makefile found in the `zeal60/` directory:

```
cd zeal60
make
```

#### Using Makefile
To compile the Linux version you can use the supplied Makefile found in the `zeal60/` directrory:

```
cd zeal60
make
```

#### Using bare commands

You may need to adjust the paths here depending on how you installed hidapi.

NOTE: The order of these flags is important!

```
g++ -c -I/usr/local/include/hidapi -Wno-write-strings keycode.cpp -o keycode.o
g++ -c -I/usr/local/include/hidapi -Wno-write-strings zeal60.cpp -o zeal60.o
g++ -I/usr/local/include/hidapi -L/usr/local/lib -Wno-write-strings zeal60.cpp keycode.cpp -lhidapi-libusb -o zeal60
```

#### Udev rules

After compiling the Zeal60 command line tool, You need to either run it as root or place the provided udev rule file in `/etc/udev/rules.d/` 

The provided udev rules file gives write access to the usb device for everyone in the zeal60 group. You should either change this to another group, or make this group and add yourself to it ( `groupadd zeal60; gpasswd -a user zeal60` ).

#### Running the command under Linux

`linux/zeal60_keymap_poker2_ansi.sh` is the basic Poker 2 keymap converted to a script for Linux.

Make sure that you've put the `zeal60` binary you compiled in a place the script can find it (such as adding it to your 'PATH' environment variable).

