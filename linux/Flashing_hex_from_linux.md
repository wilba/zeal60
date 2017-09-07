# Flashing Zeal60 firmware from Linux

This worked for me:

```
git clone https://github.com/kairyu/tkg-toolkit
cd tkg-toolkit/linux
./setup.sh
```

Then pick:

```
2. GH60 RevCHN
Y (continue)
1. Default
1. atmel_dfu
```

Make sure dfu-programmer is installed:

```
sudo apt-get install dfu-programmer
```

Finally, install the firmware (assuming you have the firmware in that location):

```
sudo ./reflash.sh ../../zeal60_default.hex
```