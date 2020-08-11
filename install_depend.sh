#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Slog
cd $DIR/lib
git clone https://github.com/kala13x/slog.git
cd slog/src
make
sudo make install

# LCDGFX
cd $DIR/lib
git clone https://github.com/lexus2k/lcdgfx.git
cd 
