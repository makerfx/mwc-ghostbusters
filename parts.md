# Parts

This project was based off the work in the [RC with Adaptive Controls for Disability: How To Guide](https://www.youtube.com/watch?v=rYxwIrqE7q8) YouTube video.

## Arduino Parts for RC Ghost Trap
This worked, but I really wish I'd gone straight for a Teensy 4.1 :)
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
    - Set Bank to 2 - [Instructions](https://www.multi-module.org/using-the-module/ppm-mode#protocol-bank-selection) and you can see the required setting in this file [multi-module firmware config.h](https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/dbfccad568ee7f827302390d88e59ea7722af9e1/Multiprotocol/_Config.h#L451)
    - Set Protocol to 1 (1 through 8 are all DSM modes, I tried 1, it worked, and I stopped testing)
    - Press the binding button on the AR410 receiver, and boot the Arduino system with the IRX4+ (Make sure the Arduino is already setup, sending signal and wires are connected between the Arduino and the IRX4+ transmitter.
  - We had a lot of trouble with the connector on this device, so I opened it up and soldered pins into it. Problem solved.
- SPEKTRUM AR410 receiver ($34.99 each) [Amazon Link](https://www.amazon.com/Spektrum-AR410-4-Channel-Sport-Receiver/dp/B07GS2S7W8)
- RC Switches
  - Elechawk 4A RC Switch ($9.50 each) [Amazon Link](https://www.amazon.com/gp/product/B08FLZXSD7/)
  - APEX 3A RC Switch ($11.99 each) [Amazon Link](https://www.amazon.com/gp/product/B07BKSGW3D/)
  - DO NOT BUY THIS POS - Dilwe RC Switch ($11.98 for 3) [Amazon Link](https://www.amazon.com/gp/product/B08M6B46QB) - this switch requires you to sweep low->high->low->high to cycle on, then low->high->low->high to cycle off.
- Extended length .1 inch pin header for the connection to the RC transmitter and on the Uno for extra solder points


## Misc Parts for RC Ghost Trap
- XBOX ONE USB Controller ($24.69 each) [Amazon Link](https://www.amazon.com/gp/product/B08HYFNZL7)
- Voltage Regulator - 800ma ($8.89 for 10) [Amazon Link](https://www.amazon.com/gp/product/B07CP4P5XJ) - using one of these to step the RC car voltage down to 3v for the original toy ghost trap lights & sounds which will be switched by an RC switch
- Voltage Regulator - 3a ($11.99 each) [Amazon Link](https://www.amazon.com/dp/B00C0KL1OM) - using one of these to step the RC car voltage down to 5v for the flashlight "headlights" for the RC car
- 3v relay (bought a relay cheap board from amazon, then pulled off the relay component for size) - this was needed because the soundfx board needed constant power and then to have the trigger activated using the 3v relay.

## Audio Parts for Proton Pack
- Adafruit Sound FX board ($29.99 each) [Adafruit Link](https://www.adafruit.com/product/2217)
  - In the end, the onboard amp was not enough power, so we went to the powered bluetooth speaker setup and soldered on a male headphone cable
- Adafruit 1/8" headphone cable (we soldered on to the Sound FX board)
- Insignia Rugged Bluetooth speaker ($20) [Best Buy Link](https://www.bestbuy.com/site/insignia-rugged-portable-bluetooth-speaker-black/5204300.p)
- Multiple types of push button switches for sound board trigger

## Power for the Proton Pack
- Armstrong 10,000 Mah USB pack ($20) [Harbor Freight Link](https://www.harborfreight.com/10000-mah-power-bank-64488.html)
- 4 port USB Charger ($13) from Big Lots (Can't locate online)
- 2 pack USB micro cable set ($7) from Big Lots (Can't locate online)
- USB A->B cable for the Arduino Uno

## Consumables used
- Small wire
- Shrink tubing
