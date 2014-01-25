#!/bin/sh

BICS='/var/bics'
BICS_DEBUG='/var/bics-debug'

# The files we want to update:
rm timeseal.o

make
sudo rm -rf $BICS_DEBUG
sudo make install
sudo mv /var/chessd $BICS_DEBUG
sudo cp ~/space/timeseal/timeseal_decoder $BICS_DEBUG/bin/

# Copy over the current database and scripts
sudo rm -rf $BICS_DEBUG/players
sudo cp -rf $BICS/players $BICS_DEBUG/players
sudo cp -rf ../scripts $BICS_DEBUG/scripts
