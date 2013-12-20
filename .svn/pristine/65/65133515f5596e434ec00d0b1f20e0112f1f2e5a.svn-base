#cd /usr/local/chessd/src
svn up --username chessd --password 12345
make clean
make
cp -f chessd /usr/local/chessd/
cp -f chessd_addplayer /usr/local/chessd/
cp -f makerank /usr/local/chessd/
cp -f chessd.so /usr/local/chessd/lib/ && (
cd /usr/local/chessd/
 ./start_chessd )
