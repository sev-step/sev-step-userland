#!/bin/bash

if [[ "$(cat /sys/module/kvm/parameters/tdp_mmu)" == "Y" ]]; then
    echo 'Error: tdp_mmu param of kvm module must be set to "N"'
    exit 1
fi

#kvm_amd sev-snp=1 sev=1 sev-es=1
if [[ "$(cat /sys/module/kvm_amd/parameters/sev_snp)" != "Y" ]]; then
    echo 'Error: sev_snp param of kvm_amd module must be set to "Y"'
    exit 1
fi

if [[ "$(cat /sys/module/kvm_amd/parameters/sev)" != "Y" ]]; then
    echo 'Error: sev param of kvm_amd module must be set to "Y"'
    exit 1
fi

if [[ "$(cat /sys/module/kvm_amd/parameters/sev_es)" != "Y" ]]; then
    echo 'Error: sev_es param of kvm_amd module must be set to "Y"'
    exit 1
fi