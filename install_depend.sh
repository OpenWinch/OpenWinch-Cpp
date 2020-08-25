#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Slog
echo "Install slog"
cd $DIR/lib
git clone https://github.com/kala13x/slog.git
cd slog/src
make
sudo make install

# LCDGFX
echo "Install LCDGFX"
cd $DIR/lib
git clone https://github.com/lexus2k/lcdgfx.git
cd lcdgfx
make
sudo cp bld/liblcdgfx.a /usr/local/lib/

# CPP-HTTPlib
echo "Install cpp-httplib"
cd $DIR/lib
git clone https://github.com/yhirose/cpp-httplib.git
cd cpp-httplib

# SpdLog
echo "Install spdlog"
cd $DIR/lib
git clone https://github.com/gabime/spdlog.git
cd spdlog && mkdir build && cd build
cmake .. && make -j

