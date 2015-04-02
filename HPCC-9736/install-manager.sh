#!/bin/bash
################################################################################
#    HPCC SYSTEMS software Copyright (C) 2012 HPCC Systems.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
################################################################################
#
# Usage: install-manager.sh <deploy/redeploy/undeploy> <package file path>
#

REMOTE_INSTALL="/tmp/remote_install"

checkUser(){
    USER=$1
    id ${USER} 2>&1 > /dev/null
    if [ $? -eq 0 ];
    then
        return 1
    else
        return 0
    fi
}

pkgCmd(){
    if [ "$1" == "deb" ]; then
        PKGCMD="dpkg -i $PKG &> $outputFile; apt-get install -f -y &> $outputFile;"
    elif [ "$1" == "rpm" ]; then
        if [ "$2" == "install" ] || [[ "$2" != "reinstall" ]]; then
            PKGCMD="yum install --nogpgcheck -y $PKG &> $outputFile"
        elif [ "$2" == "reinstall" ]; then
            PKGCMD="yum reinstall --nogpgcheck -y $PKG &> $outputFile"
        fi
    else
        echo "BAD Package type."
        exit 1
    fi
}

installPkg(){
    checkInstall
    if [ "$COMMAND" == "deploy" ]; then
        pkgCmd $OSPKG "install"
    elif [ "$COMMAND" == "redeploy" ]; then
        pkgCmd $OSPKG "reinstall"
    elif [ "$COMMAND" == "undeploy" ]; then
        pkgCmd $OSPKG "remove"
    fi
    eval $PKGCMD
    if [ $? -ne 0 ]; then
      echo "FAILED on Package Command: ${PKGCMD}"
      exit 1
    fi
}

if [ $# -eq 0 ]; then
    exit 1
fi

COMMAND=$1
PKG=$2
outputFile="${REMOTE_INSTALL}/remote-install.log"
if [ -e ${outputFile} ]; then
    rm -rf ${outputFile}
fi

pkgtype=`echo "$PKG" | grep -i rpm`
if [ -z $pkgtype ]; then
    OSPKG="deb"
else
    OSPKG="rpm"
    WITH_PLUGINS=0
    basename $PKG | grep -i with-plugins
    [ $? -eq 0 ] && WITH_PLUGINS=1
fi

installPkg
exit 0
