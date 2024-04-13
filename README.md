## [Pixel Reader](https://github.com/ealang/pixel-reader)

An ebook reader app for the Miyoo Mini and MiyooCFW. Supports epub and txt formats.

![Screenshot](resources/demo.gif)

## MiyooCFW Installation

*It is presumed you are using the latest [MiyooCFW](https://github.com/TriForceX/MiyooCFW/) custom firmware on your device - version at least 2.0.0 BETAv3.*

1. [Download the latest release package](https://github.com/Apaczer/pixel-reader/releases).
1. Put IPK-package in your MAIN partition.
2. **IPK:** Launch ``reader.ipk`` from GMenu2X's Explorer.

The default location for book files is `/mnt/books`.

## Miyoo Mini Installation

Supports Onion, MiniUI, and the default/factory OS.

1. [Download the latest release](https://github.com/ealang/pixel-reader/releases). Make sure to get the correct zip file for your OS. For Onion or default/factory OS: `pixel_reader_onion_xxx.zip`. For MiniUI: `pixel_reader_miniui_xxx.zip`. 
2. Extract the zip into the root of your SD card.
3. Boot your device, and the app should now show up in the apps/tools list.

The default location for book files is `Media/Books`.

## Development Reference

### Desktop Build

Install dependencies (Ubuntu):
```
apt install make g++ libxml2-dev libzip-dev libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev
```

Build:
```
make -j
```

Find app in `build/reader`.

### MiyooCFW Cross-Compile
If you're trying to build for uClibc compatible image, use one of these two methods:
- Docker  

```sh
git clone https://github.com/Apaczer/pixel-reader
cd pixel-reader/
docker pull miyoocfw/toolchain-shared-uclibc
docker run --volume ./:/src/ -it miyoocfw/toolchain-shared-uclibc
cd /src
make CPPFLAGS="-DMIYOO -DUSER_FONTS"
```
- Local (*)

```sh
make CROSS_COMPILE=/opt/miyoo/usr/bin/arm-miyoo-linux-uclibcgnueabi- SYSROOT=/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot CPPFLAGS="-DMIYOO -DUSER_FONTS"
```

*Note: To relocate default SDK location (`/opt/miyoo`) run "relocate-sdk.sh" script from new placement.

### Miyoo Mini Cross-Compile

Cross-compile env is provided by [shauninman/union-miyoomini-toolchain](https://github.com/shauninman/union-miyoomini-toolchain). Docker is required.

Fetch git submodules:
```
git submodule init && git submodule update
```

Start shell:
```
make miyoo-mini-shell
```

Create app packages:
```
./cross-compile/miyoo-mini/create_packages.sh <version num>
```

### Run Tests

[Install gtest](https://github.com/google/googletest/blob/main/googletest/README.md).

```
make test
```
