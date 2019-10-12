#!/bin/zsh
TIMEFMT='%J   %U  user %S system %P cpu %*E total'$'\n'\
'avg shared (code):         %X KB'$'\n'\
'avg unshared (data/stack): %D KB'$'\n'\
'total (sum):               %K KB'$'\n'\
'max memory:                %M MB'$'\n'\
'page faults from disk:     %F'$'\n'\
'other page faults:         %R'

for i in $(seq 1 18)  
do
echo "i="$i   
(time ../is_sos F$i.txt) >F$i.txt.sp
done   
