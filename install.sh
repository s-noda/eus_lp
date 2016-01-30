#!/usr/bin/env bash

function check_libs(){
    echo -e "\e[33minstall libs ... \e[m";
    if [ ! -d "/usr/include/lpsolve" ];
    then
	echo -e "\e[33mlpsolve installing ... \e[m";
	sudo aptitude install lp-solve;
    fi
    if [ ! -d "/usr/lib/lp_solve" ];
    then
	echo -e "\e[33mlpsolve_lib installing ... \e[m";
	sudo aptitude install liblpsolve55-dev;
    fi
    if [ ! -d "/usr/include/coin" ];
    then
	echo -e "\e[33mclp installing ... \e[m";
	sudo aptitude install coinor-libclp-dev
    fi
    # if [ ! -d "/usr/lib/mumps" ];
    # then
    # 	echo -e "\e[33mmumps_lib installing ... \e[m";
    # 	sudo aptitude install libmumps-dev;
    # fi
    echo -e "\e[33mdone\e[m";
}

check_libs;
