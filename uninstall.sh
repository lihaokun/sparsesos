#!/bin/bash

if [ -f "is_sos" ] || [ -L "is_sos" ] 
then
    rm is_sos
fi
make -C ./src clean 
if [ -d "./mosek/8" ]
then
    read -p "Do you want to delete mosek 8 at ./mosek/8?[y/n](default=n)" var
    if [ -z $var ]
    then
        var=n
    fi
    if [ $var == y ]
    then
        echo "rm -r ./mosek/8"
        rm -r ./mosek/8
    fi
fi
if [ -d "./mosek/9.0" ]
then
    read -p "Do you want to delete mosek 9.0 at ./mosek/9.0?[y/n](default=n)" var
    if [ -z $var ]
    then
        var=n
    fi
    if [ $var == y ]
    then
        echo "rm -r ./mosek/9.0"
        rm -r ./mosek/9.0
    fi
fi


if ! [ -f "/usr/bin/is_sos" ] && [ -L "/usr/bin/is_sos" ] 
then 
    echo -e "\033[1m Remove is_sos from path
sudo rm /usr/bin/is_sos \033[0m"
    sudo rm /usr/bin/is_sos
fi
echo -e "\033[32m Uninstallation Successful. \033[0m"