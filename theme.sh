#!/bin/bash

if [ $1 ]; then 
    T=$1
elif [ -f ./THEMES ]; then
    T="THEMES"
else
    echo "You need to specifiy a theme file."
    exit 1
fi

cat $T | sort -R | head -n 1  
