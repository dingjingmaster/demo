#!/bin/bash

bash << __END
ss="$(pacman -Qi $(pacman -Qo /usr/bin/bash | awk '{print $5}') | grep 'Install Date' | head -1 | awk -F':' '{print $2}')"
echo "$ss"
__END

#| date '+%s'
#| date '+%s'
#[[ x"" -eq x"$ss" ]] && echo $ss 
