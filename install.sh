#!/bin/bash
dir=`pwd -P`
# if   [ -f is_sos ] || [ -L is_sos ]
# then
#     echo "File exists."
#     #echo  "Please run uninstall.sh first."
#     #exit 1
#     read -p "Do you want to reinstall?[y/n]" isre
#     if  [ $isre == y ]
#     then
#         chmod +x uninstall
#         ./uninstall
#         echo 
#     else
#         exit 1
#     fi
# fi
if [ -f "${HOME}/mosek/mosek.lic" ]
then
    echo -e "\033[32m Mosek license checked. \033[0m"
else
    echo -e "\033[31m Mosek license not found. \033[0m"    
    echo "Pleasa ensure that the license of mosek is valid.(~/mosek/mosek.lic)"
    echo "You can get a free academic license on this website:https://www.mosek.com/products/academic-licenses/"
fi
echo
echo "Which mosek version you want to use"
echo "  1) mosek 8.1"
echo "  2) mosek 9.0"
read -p  "Enter a selection (default=1):" mosek_v
if [ -z $mosek_v ]
then
    mosek_v=1
fi
if ! [ $mosek_v == 1 ] && ! [ $mosek_v == 2 ]
then
    echo -e "\033[31m Invalid Input. \033[0m"
    exit 1
fi

if [ $mosek_v -eq 1 ] && ! [ -d mosek/8 ]
then
    if [ -f mosektoolslinux64x86.tar.bz2 ]
    then
        rm mosektoolslinux64x86.tar.bz2
    fi
    wget https://download.mosek.com/stable/8.1.0.81/mosektoolslinux64x86.tar.bz2
    echo "tar -xjf mosektoolslinux64x86.tar.bz2" 
    tar -xjf mosektoolslinux64x86.tar.bz2
    make -C mosek/8/tools/platform/linux64x86/src/fusion_cxx install
fi
if [ $mosek_v -eq 2 ] && ! [ -d mosek/9.0 ]
then
    if [ -f mosektoolslinux64x86.tar.bz2 ]
    then
        rm mosektoolslinux64x86.tar.bz2
    fi
    wget https://download.mosek.com/stable/9.0.97/mosektoolslinux64x86.tar.bz2
    tar -xjf mosektoolslinux64x86.tar.bz2
    make -C mosek/9.0/tools/platform/linux64x86/src/fusion_cxx install
fi
if [ $mosek_v -eq 2 ]
then
    make -C ./src/ mosek9=1
else
    make -C ./src/  
fi
if [ -f "src/is_sos" ]
then
    ln -sf src/is_sos ./
    echo  -e "\033[32m Installation Successful. \033[0m"
    echo -e "\033[1m Add is_sos to path 
sudo ln -s ${dir}/src/is_sos /usr/bin/is_sos \033[0m"
    sudo ln -sf ${dir}/src/is_sos /usr/bin/is_sos
else
    echo  -e "\033[31m Installation Failed. \033[0m"
    exit 1
fi
