# tccTomazi

## Install instructions

### Install avrdude and make

On linux: `sudo apt-get install avrdude make`

On Windows:

[WinAVR](https://sourceforge.net/projects/winavr/)

[GnuWin](https://sourceforge.net/projects/gnuwin32/)

### Install

`make install PORT=COMx`

Change `PORT=COMx` to the serial port arduino is connected.

## Build instructions for Linux

### Download [Atmel AVR 8-bit Toolchain for Linux](http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORLINUX.aspx).

Copy its contents to `/usr/local/avr8-gnu-toolchain`.

### Prepend to PATH

`PATH=/usr/local/avr8-gnu-toolchain/bin:$PATH`

It is recommended to append this line to ~/.profile.

### Install avrdude and make

`sudo apt-get install avrdude make`

### Build from source

`make clean all`
