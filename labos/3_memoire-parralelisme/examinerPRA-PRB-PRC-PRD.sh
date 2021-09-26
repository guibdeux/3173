#!/bin/bash
sudo apt-get install screen
# crtl-D ou exit pour sortir de screen
echo "lancement de ./pra"
./pra &
wait 1
echo "lancement de ./prb"
./prb &
wait 1
echo "lancement de ./prc"
./prc &
wait 1
echo "lancement de ./prd"
./prd &




kill -s 9 `pgrep pra` &>> /dev/null
kill -s 9 `pgrep prb` &>> /dev/null
kill -s 9 `pgrep prc` &>> /dev/null
kill -s 9 `pgrep prd` &>> /dev/null




