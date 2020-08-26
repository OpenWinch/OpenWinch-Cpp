#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Slog
echo -e "\nInstall slog"
cd $DIR/lib
if [ ! -d "slog" ]
then
  git clone https://github.com/kala13x/slog.git
fi
cd slog/src
make
# sudo make install

# LCDGFX
echo -e "\nInstall LCDGFX"
cd $DIR/lib
if [ ! -d "lcdgfx" ]
then
  git clone https://github.com/lexus2k/lcdgfx.git
fi
cd lcdgfx
make
#sudo cp bld/liblcdgfx.a /usr/local/lib/

# CPP-HTTPlib
echo -e "\nInstall cpp-httplib"
cd $DIR/lib
if [ ! -d "cpp-httplib" ]
then
  git clone https://github.com/yhirose/cpp-httplib.git
fi
cd cpp-httplib
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..


# SpdLog
#echo "Install spdlog"
#cd $DIR/lib
#git clone https://github.com/gabime/spdlog.git
#cd spdlog && mkdir build && cd build
#cmake .. && make -j

