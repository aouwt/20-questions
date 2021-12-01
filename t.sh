#!/bin/sh
f=$(cpp -Drelease ./20.c)
echo "$f" | cobfusc -abdentx -i random -c random -w 80 -o ./20-o.c
echo "$f" | cobfusc -en -w 80 -o ./20-min.c
