#!/bin/bash

winID=$(xdotool getactivewindow)

winInfo=$(xdotool getwindowgeometry --shell $winID)

winX=0
winY=0
winW=0
winH=0

winPX=0
winPY=0

if [[ $winInfo =~ X=([0-9]+) ]]; then
    winX=${BASH_REMATCH[1]}
fi

if [[ $winInfo =~ Y=([0-9]+) ]]; then
    winY=${BASH_REMATCH[1]}
fi

if [[ $winInfo =~ WIDTH=([0-9]+) ]]; then
    winW=${BASH_REMATCH[1]}
fi

if [[ $winInfo =~ HEIGHT=([0-9]+) ]]; then
    winH=${BASH_REMATCH[1]}
fi

winPX=$(echo "$winW/2" | bc)
winPY=$(echo "$winH/2" | bc)

xdotool mousemove --sync $winPX $winPY
