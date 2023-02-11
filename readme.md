# AT93C46 library for RP2040

This is a library I created for a [personal project](https://github.com/skylordants/Pico_receiver). It is the first one I have created and published. I didn't want to use Arduino and couldn't find any other working library. It was a learning experience for me and it works for me. I haven't extensively tested it and don't intend on regularily maintaining it. Unless I need to or something else happens.

## Usage
The simplest way to use the library is to include it in a platformio project using [wizio-pico baremetal](https://github.com/Wiz-IO/wizio-pico) platform. Then an AT93C46 object may be created using the constructor by giving it the right pins for your project. After that you can just use *read_words* and *write_words* functions to read and write data without bothering with the details. The necessary 8 bit or 16 bit word memory layout can also be set with *set_mode* either by giving it the pin connected to the **ORG** pin or giving it -1 to only set the software side, if it has been set in hardware.

## Sources
The sources I used for writing this code:
* [Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/doc5140.pdf) for the necessary information for the implementation
* 0xJoey's [Arduino_93C46](https://github.com/0xJoey/Arduino_93C46) to get a sense, how people might implement it and what ways are awailable. I ended up going a different route, using RP2040's built in SPI instead of bit banging the protocol.

## License
I gave it an MIT license, because I haven't dealt with licenses before, it seemed the easiest and I didn't want to bother with longer licenses. While I did read a [GPL licenced library](https://github.com/0xJoey/Arduino_93C46) meant for Arduino to get sense of how to interface with the eeprom, all of the implementation is created by myself using the information in the [datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/doc5140.pdf). I did use similar names for some variables and functions, but I could argue, that they are universal and basically the same as the names given in the [datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/doc5140.pdf).

I don't intend on making any profit from this. I only wrote it for a personal project and wanted to share it, if it would help anyone else. I don't care how others use this code. If it somehow miraculously is a necessary part of some widespread project or something bringing in thousands, it would be nice to get some recognition or compensation, but I'm not expecting anything.

Please don't come after me because of this repository. If anyone has a problem with the code or the licence, they can contact me to fix it or to make me take it down.