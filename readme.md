# Zeal60 Command Line Tool

This is the command line tool for changing keymaps and backlight settings for the Zeal60 PCB.

Zeal60 PCB uses firmware based on [QMK firmware](https://github.com/jackhumbert/qmk_firmware) with the required changes to support RGB backlight and changing keymaps via the PC.
Unlike typical QMK usage, users will not need to change QMK source files, build and program new firmware. They only need to upload the latest "zeal60" firmware and use this tool to change keymaps and backlight settings.

See [readme.txt](docs/readme.adoc) for instructions on how to use this tool.

## Latest Build

The latest official build of this tool, and the matching QMK firmware, can be found at [ZealPC.net](https://zealpc.net/)

## Build Instructions

The fork of QMK firmware with code to support this tool is found here: [https://github.com/Wilba6582/qmk_firmware/tree/zeal60](https://github.com/Wilba6582/qmk_firmware/tree/zeal60)

NOTE: it is in the "zeal60" branch not the "master" branch.

The source code of the tool will include files from the qmk_firmware directory, so this clone and the qmk_firmware clone must have the same parent directory.

Use Visual Studio 2015 (Community edition works, and is free).

The last built executable will be copied into the keymaps directory, so the batch files there will use it.

If you're interested in porting this code to Linux or OSX, let me know.


