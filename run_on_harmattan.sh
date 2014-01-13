#!/bin/bash

mkdir -p build-harmattan

cp -r qtc_packaging/debian_harmattan build-harmattan/debian

cd build-harmattan

/home/micha/QtSDK/Madde/bin/mad -t harmattan_10.2011.34-1_rt1.2 cmake .. -DHARMATTAN_BUILD=1 -DCMAKE_INSTALL_PREFIX=`pwd`/debian/shine/
/home/micha/QtSDK/Madde/bin/mad -t harmattan_10.2011.34-1_rt1.2 make
/home/micha/QtSDK/Madde/bin/mad -t harmattan_10.2011.34-1_rt1.2 dpkg-buildpackage -nc -uc -us

scp ../shine_0.1.0-harmattan0_armel.deb developer@10.10.10.106:/home/developer/

ssh developer@10.10.10.106 "echo rootme | /bin/devel-su -c \"dpkg -i /home/developer/shine_0.1.0-harmattan0_armel.deb\""
ssh developer@10.10.10.106 "DISPLAY=:0 /opt/shine/bin/shine_harmattan"
