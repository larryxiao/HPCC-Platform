#! /bin/bash

DISTRO=$(lsb_release -d | awk '{print $2}')

if [ "$DISTRO" == "Ubuntu"  ]; then
    PREFIX="sudo service "
fi;
if [ "$DISTRO" == "Debian"  ]; then
    PREFIX="sudo /etc/init.d/"
fi;
#Red Hat Enterprise Linux
if [ "$DISTRO" == "Red"  ]; then
    PREFIX="sudo /sbin/service "
fi;
if [ "$DISTRO" == "CentOS"  ]; then
    PREFIX="sudo /sbin/service "
fi;

SERVICE="hpcc-init"
eval $PREFIX$SERVICE $1

if [ "$1" == "stop" ]; then
    SERVICE="dafilesrv"
    eval $PREFIX$SERVICE $1
fi;

