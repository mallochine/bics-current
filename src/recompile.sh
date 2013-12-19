#!/bin/sh

BICS='/var/bics'
BICS_DEBUG='/var/bics-debug'

make clean
make
sudo rm -rf $BICS_DEBUG
sudo make install
sudo mv ~/chessd $BICS_DEBUG
sudo cp ~/laskerd/bin/timeseal_decoder $BICS_DEBUG/bin/

# Copy over the current database
sudo rm -rf $BICS_DEBUG/players
sudo cp -rf $BICS/players $BICS_DEBUG/players
