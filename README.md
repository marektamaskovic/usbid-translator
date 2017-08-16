USB ID translator
=================

# Purpose

Translate numeric USB device ID and interface representations into human readable strings by leveraging information from the USD ID database (http://www.linux-usb.org/usb.ids).

## Vendor & Product ID

Format: Two 16 bit numbers, separated by a colon; `0000:0000` ... `ffff:ffff`

First number represents vendor ID.
Second number represents product ID.

## Interface

Format: Three 8bit numbers
, separated by a colon; `00:00:00` ... `ff:ff:ff`

First number represents interface class.
Second number represents interface subclass.
Third number represents interface protocol.

# API
```c++
 class USBIDs
 {
 public:
   USBIDs(std::istream* input);

   std::string idToString(uint16_t vid, uint16_t pid);
   std::string interfaceToString(uint8_t c, uint8_t s, uint8_t p);
 };
 ```

# Implementation Notes
Latest `usb.ids` file can be downloaded from [http://www.linux-usb.org/](http://www.linux-usb.org/usb.ids).
Alternatively, the file can be installed via packages:

### Fedora
`$ dnf isntall hwdata`
### Arch Linux
`$ pacman -Sy hwids`
### Ubuntu
`$ apt install hwdata`

After successfull installation, the `usb.ids` file will be located at `/usr/share/hwdata/usb.ids`.
