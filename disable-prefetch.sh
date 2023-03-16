#!/bin/bash
sudo modprobe msr
sudo wrmsr -a 0xc001102b  0x00000000200fcc16
sudo wrmsr -a  0xc0011022 0xc000000002512000
echo "Disabled prefetch"

