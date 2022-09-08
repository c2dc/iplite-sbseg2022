# iptables-lite (ESP-32)

<em>This tutorial was adapted from the blog post above. Credits to the author @saramonteiro.</em>

https://blog.espressif.com/getting-started-with-esp32-and-nuttx-fd3e1a3d182c

## Prerequisites
   1\. Have a ESP32 microcontroller with internet access

   2\. Install a series of required tools. They can be found <a href="http://nuttx.incubator.apache.org/docs/latest/quickstart/install.html">here</a>

## Espressif ESP32 setup
### 1. Toolchain
We are going use the prebuilt toolchain for Xtensa architecture.

Download and decompress the pre built cross compiler for ESP32 in Linux environment. The cross compiler will be used to convert the source code into an executable code.
```
$ curl https://dl.espressif.com/dl/xtensa-esp32-elf-gcc8_2_0-esp-2020r2-linux-amd64.tar.gz | tar -xz
```
Since the /opt/ directory is a space commonly used to store third party software, create a directory at /opt/ to keep the cross compiler for xtensa architecture:
```
$ sudo mkdir /opt/xtensa
```
Move the cross compiler to this new directory:
```
$ sudo mv xtensa-esp32-elf/ /opt/xtensa/
```
Now you have the cross compiler for ESP32 at this path, in order to invoke the cross compiler binaries as commands, you should add its absolute path to PATH, which is a Linux environment variable that informs the shell where to search for executables or programs that are invoked through commands. To do so, use the following command:
```
$ export PATH=$PATH:/opt/xtensa/xtensa-esp32-elf/bin
```
For flashing firmware, you will need to install `esptool.py` by installing the esptool Python module to perform the download of all binaries to the ESP32 through serial.
```
$ pip3 install esptool
```
You may have noticed the following warning message at the end of the installation:

`WARNING: The scripts pyserial-miniterm and pyserial-ports are installed in ‘/home/<user>/.local/bin’ which is not on PATH.`

This messages warns us that esptool, as well as other programs that are used by esptool, were installed at a path which is not included on PATH, so these programs are not visible at the current shell. To solve this issue, add this path to PATH and load it using the following command:
```
$ export PATH=$PATH:$HOME/.local/bin
```
**_NOTE:_**  Once you leave your terminal session, PATH will loose these paths added to it temporarily and you will need to run the “export” commands again in a new session. It may be a little annoying. If you want to keep these paths permanent to shell sessions, open your bash file and add these paths to PATH, through the following command:
```
$ sudo nano ~/.bashrc
```
Paste it to the end of the file:
```
# Add esptool.py and its dependencies directory 
PATH=$PATH:$HOME/.local/bin
# Add the cross compiler path for ESP32
PATH=$PATH:/opt/xtensa/xtensa-esp32-elf/bin
```
<br />

### 2. Bootloader and partitions

Besides the Operating System with the application, ESP32 also requires a bootloader and a partition table. Both of them can be customized and built to answer customer’s expectations. However, for the sake of simplicity, these binaries were previously generated for you from the latest ESP-IDF’s master branch and can be easily downloaded from this repository kept by Espressif. To do so, create a dedicated directory aside nuttx directory to keep these binaries and download these pre-configured binaries:

```
$ mkdir esp-bins
$ curl -L "https://github.com/espressif/esp-nuttx-bootloader/releases/download/latest/bootloader-esp32.bin" -o esp-bins/bootloader-esp32.bin
$ curl -L "https://github.com/espressif/esp-nuttx-bootloader/releases/download/latest/partition-table-esp32.bin" -o esp-bins/partition-table-esp32.bin
```
In case you want to generate these binaries yourself, take a look at here and check out the step by step.

<br />

### 3. Board configuration
The first step is to initialize NuttX configuration for a given board, based from a pre-existing configuration<sup>1</sup>.
```
$ cd nuttx
$ ./tools/configure.sh esp32-devkitc:wapi
```
   <sup>1</sup>To list all available configurations:
```
$ ./tools/configure.sh -L | less
```

<br />

### 4. Flashing
You have to add yourself to the dialout group on Linux to have permission to access serial ports. Since in the next step you will use the serial to download the binaries, run the following command to add your user to the dialout group, which has the permission to access the serial driver.
```
$ sudo adduser <user> dialout
```
Where `<user>` is your username. 

This addition will only permanently take effect after log out and log in. So, a workaround for it is to temporarily change the current user to the dialout group:
```
$ newgrp dialout
```

Firmware for ESP32 is flashed via the USB/UART interface using the esptool.py tool. To flash your NuttX firmware simply run:
```
$ make download ESPTOOL_PORT=/dev/ttyUSB0 ESPTOOL_BAUD=115200 ESPTOOL_BINDIR=../esp-bins
```

**_NOTE:_** adjust the USB port according to your configuration. The last two arguments are optional. In case they’re not specified, the command will download only the application and it will use the default baud rate of 921600. Once the bootloader and partition table binaries are downloaded, it’s not necessary to download them next time.

<br />

In case the above command is interrupted because the Pyserial module was not installed, install it by running the following command:
```
pip3 install pyserial
```

And make the download again.

<br />

## NuttX Shell Access
To access the NuttX shell, you only need a serial terminal. If you not have a serial terminal, try picocom. To install picocom run the following command:
```
$ sudo apt-get install -y picocom
```

And finally, access the nsh (NuttX Shell):
```
$ sudo picocom /dev/ttyUSB0 -b 115200
```
**_NOTE:_** adjust the USB port according to your configuration.


In case you want to connect to a WiFi network, a standard network interface will be configured and can be initialized such:
```
nsh> ifup wlan0
nsh> wapi essid wlan0 <ssid> 1
nsh> wapi psk wlan0 <passwd> 2
nsh> renew wlan0
```

<br />

## NuttX Tips

This section is a bonus in order to summarize the main commands that are most used and show some simple and typical problems that can happen in your journey and how to work around them. <br/><br/>

`make distclean`

It removes the configuration of the previous board and makes it available for the user to select another board or another configuration of the same board.

`./tools/configure.sh boardname:configname`

It selects the board (in this example it was esp32-devkitc) and the configuration to be used (in this example it was nsh config, which includes only the nuttx shell). There are other configurations that include support for certain peripherals and examples.

`make menuconfig`

It opens the configuration screen for the user to customize what he/she wants to add or to remove on the board. For example, it allows you to add drivers for a specific device, add debug messages, examples, etc.

`make clean`

It removes binary files generated from the previous built.

`make apps_distclean`

It cleans only application binaries. Kernel and driver binaries are kept.

`make`

It only builds your application.

This tutorial was adapted from the following article
https://blog.espressif.com/getting-started-with-esp32-and-nuttx-fd3e1a3d182c