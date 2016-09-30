#!/bin/bash

# disable the discrete gpu
echo "disabling gpu"
./disablegpu

# turn off the discrete gpu
echo "powering down gpu"
bash -c "echo OFF > /sys/kernel/debug/vgaswitcheroo/switch"
