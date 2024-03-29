#!/bin/bash
echo plc > /etc/hostname
userdel -r vagrant
cp db.sqlite3 funciones.py plc updateAngle.py /root
cp plc.service /etc/systemd/system
cp updateAngle.service /etc/systemd/system
ln -s /root/plc /usr/local/bin/plc
pip install apscheduler
systemctl enable plc
systemctl enable updateAngle
cp eth0.network /etc/systemd/network
cd ..
rm -rf tfg-plc
shutdown now
