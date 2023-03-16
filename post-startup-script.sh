#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "This script pins the victims qemu vcpu thread to the given cpu core and also fixates the clock frequency on that core."
	echo "If your system is configured to run at a fixed frequency and does thus not support cpufreq-set, pass \"NULL\" for the frequency"
	echo "It is assumed that only one instance of qemu-system-x86_64 is running"
	echo "Usage: post-startup-script <cpu to pin to> <{target cpu freq,NULL}>"
	echo "On most machines the lowest frequency in /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies works well"
	exit
fi
CPU=$1
FREQ=$2
sudo qemu-affinity $(pidof qemu-system-x86_64 ) -k $CPU || exit 1
echo "Pinned vcpu thread to CPU $CPU"

if [[ ${FREQ} == "NULL" ]]; then
	echo "FREQ=NULL provied, assuming cpu frequency is already fixed"
else
	sudo cpufreq-set -r -c $CPU -g performance -u "${FREQ}" -d "${FREQ}" || exit 1
	echo "Set frequency"
fi



echo "No disable prefetch"
./disable-prefetch.sh || exit 1
echo "Check KVM params"
./check-kvm-params.sh || exit 1

