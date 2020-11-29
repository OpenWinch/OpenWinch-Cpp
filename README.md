# OpenWinch - C/C++
_Open-source Winch project._

this repository contains the firmware for RaspberryPI board.

[![GitHub version](https://img.shields.io/github/release/OpenWinch/OpenWinch-Cpp.svg)](https://github.com/OpenWinch/OpenWinch-Cpp/releases/latest)
[![packagecloud.io](https://img.shields.io/badge/deb-packagecloud.io-844fec.svg)](https://packagecloud.io/Theosakamg/openwinch)
[![GitHub download](https://img.shields.io/github/downloads/OpenWinch/OpenWinch-Cpp/total.svg)](https://github.com/OpenWinch/OpenWinch-Cpp/releases/latest)
[![License](https://img.shields.io/github/license/OpenWinch/OpenWinch-Cpp.svg)](LICENSE)

If you like **OpenWinch**, give it a star, or fork it and contribute!

[![GitHub stars](https://img.shields.io/github/stars/OpenWinch/OpenWinch-Cpp.svg?style=social&label=Star)](https://github.com/OpenWinch/OpenWinch-Cpp/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/OpenWinch/OpenWinch-Cpp.svg?style=social&label=Fork)](https://github.com/OpenWinch/OpenWinch-Cpp/network)


## Installation

### From APT package (DEB)

curl -s https://packagecloud.io/install/repositories/Theosakamg/openwinch/script.deb.sh | sudo bash

OR

curl -L https://packagecloud.io/Theosakamg/openwinch/gpgkey | sudo apt-key add -
sudo echo "deb https://packagecloud.io/Theosakamg/openwinch/ubuntu/ trusty main" > /etc/apt/source.list.d/openwinch.list
apt update
apt install openwinch-bin

### From Source

first install [VCStool](https://github.com/dirk-thomas/vcstool)

git clone https://github.com/OpenWinch/OpenWinch-Cpp.git
cd OpenWinch-Cpp
vcs import ./lib < openwinch.repo
cp -r cmake/extra/* ./lib
cd script
./build_<arch>.sh

Arhitecture can be :
- amd64
- eabi
- eabihf
- aarch64

## Documentation

for start controller :
```./openwinch```

connect to http://<ip>
