#!/bin/bash
CARBOX2D_RESTORE=NO ./carbox2d
while :
do
    CARBOX2D_RESTORE=YES ./carbox2d
done
