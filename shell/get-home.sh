#!/bin/bash

if test -z "$HOME"; then
    HOME=$(getent passwd $(whoami) | cut -d":" -f6)
fi

echo $HOME
