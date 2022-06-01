# Parts

This project was based off the work in the [RC with Adaptive Controls for Disability: How To Guide](https://www.youtube.com/watch?v=rYxwIrqE7q8) YouTube video. 

## Arduino Parts for RC Ghost Trap (this worked, but I really wish I'd gone straight for a Teensy 4.1!)
- Arduino Uno ($22.54 each) [Amazon Link](https://www.amazon.com/Arduino-A000066-ARDUINO-UNO-R3/dp/B008GRTSV6/)
- Adafruit i2c LCD ($19.95 each) [Adafruit Link](https://www.adafruit.com/product/772)
- USB Host Shield ($4.99) [HobbyKing Link](https://hobbyking.com/en_us/kingduino-compatible-usb-host-shield.html) - lots of clones of this board out there, this is just one example.
  - Note that these shields are LOW-QUALITY and may be missing the voltage jumpers (solder blobs), etc.
- 3D printed case [Thingiverse Link](https://www.thingiverse.com/thing:4946701)


## RC Parts for RC Ghost Trap
- iRangeX IRX4+ transmitter ($55.99 each) [Banggood Link](https://usa.banggood.com/IRangeX-IRX4-Plus-2_4G-CC2500-NRF24L01-A7105-CYRF6936-4-IN-1-Multiprotocol-ARM-TX-Module-With-Case-p-1225080.html)
  - To bind the IRX4+ to the AR410
    - New firmware download: From the [multi-module firmware page] choose the IRX4+, then the ppm-aetr firmware.
    - New firmware flash: [flash-multi](https://github.com/benlye/flash-multi) tool - note that on the new module, I was getting errors and I had to use the advanced menu to write the bootloader first.
    - Set Bank to 2 - you can see the required setting in this file [multi-module firmware config.h](https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/dbfccad568ee7f827302390d88e59ea7722af9e1/Multiprotocol/_Config.h#L451)
    - Set rotary switch to 1 (1 through 8 are all DSM modes, I tried 1, it worked, and I stopped testing)
- SPEKTRUM AR410 receiver ($34.99 each) [Amazon Link](https://www.amazon.com/Spektrum-AR410-4-Channel-Sport-Receiver/dp/B07GS2S7W8)
- RC Switches
  - Elechawk 4A RC Switch ($9.50 each) [Amazon Link](https://www.amazon.com/gp/product/B08FLZXSD7/)
  - APEX 3A RC Switch ($11.99 each) [Amazon Link](https://www.amazon.com/gp/product/B07BKSGW3D/)
  - DO NOT BUY THIS POS - Dilwe RC Switch ($11.98 for 3) [Amazon Link](https://www.amazon.com/gp/product/B08M6B46QB) - this switch requires you to sweep low->high->low->high to cycle on, then low->high->low->high to cycle off.


## Misc Parts for RC Ghost Trap
- XBOX ONE USB Controller ($24.69 each) [Amazon Link](https://www.amazon.com/gp/product/B08HYFNZL7)
- Voltage Regulator - 800ma ($8.89 for 10) [Amazon Link](https://www.amazon.com/gp/product/B07CP4P5XJ) - using one of these to step the RC car voltage down to 3v for the original toy ghost trap lights & sounds which will be switched by an RC switch
- Voltage Regulator - 3a ($11.99 each) [Amazon Link](https://www.amazon.com/dp/B00C0KL1OM) - using one of these to step the RC car voltage down to 5v for the flashlight "headlights" for the RC car

## Audio Parts for Proton Pack
- Adafruit Sound FX board ($29.99 each) [Adafruit Link](https://www.adafruit.com/product/2217)
- Adafruit Mini Speakers 3 watt ($7.50 for pair) [Adafruit Link](https://www.adafruit.com/product/1669)
