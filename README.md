# SEV STEP Library
This repo contains the userspace library of the SEV STEP framework. To use it, you also need to run
the corresponding kernel. See the main SEV STEP repo to pull and build all required components.


This repo has three main components
- `sev-step-lib`  : The actual library that provides functionality like single stepping, page fault tracking etc
- `end2end-tests` : This contains a test suite, to verify that the different components of the library are working correctly on your system. It has two components: a client and a server. The server is intended to be executed inside as SEV (SNP) VM while the client is intended to run on the host system. The client uses the server to setup scenarios. For example it can request two pages the be written to in an alternating manner, which can then be used to verify the pagefault tracking functionality. The different tests are also a great starting place to explore the functionality of the library.
- `example-apps` : This currently contains one example that shows how we can break KASLR by injecting an NMI and observing pagefaults as presented in [1].




# Build
1) Issue `make dependencies` to pull and build any external dependencies used by this code. You only need to do this once.
2) Issue `make` to build all binaries and the sev step library
3)

After building, the binaries are in `sev-step-userland/build/binaries` and the library is in `sev-step-userland/build/libs`.

# Use
It is assumed, that you have followed the instructions in the main repo to install the correct kernel, tweak bios and kernels settings and followed the instructions to setup a VM.

 Issue `source ./environment.env` to setup up the `LD_LIBRARY_PATH` variable to find the locally build libraries.
 
 Most binaries need to run with root privileges (to do virtual to physical memory translation). In order to use the tweaked `LD_LIBRARY_PATH` variable with sudo, you need to use `sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH <some program>`.

## System Setup
For single stepping to work, your VM must be pinned to a dedicated core that is not used by any other code and is operating with a fixed
frequency. After setting up core pinning and frequency fixing in the next sections, the final sections introduces a handy wrapper script.

### Kernel config options
In this section you will tweak some kernel options.
Under Ubuntu, this can be done by editing
`GRUB_CMDLINE_LINUX_DEFAULT` in `/etc/default/grub`, issuing `update-grub2` and finally rebooting the system.
Use `cat /proc/cmdline` to check the currently configured options.


- For cpu core pinning, we need the following options, where cpu is a comma separated list of cores to isolate. If you system has Hyperthreading, you need to isolate both logical cores.

    `isolcpus=<cpu> nohz_full<cpu> rcu_nocbs=<cpu> rcu_nocb_poll=<cpu>`

- For frequency pinning, we need the following options:
    `processor.max_cstate=0 amd_cpufreq=enable`

- For pagefault tracking, to work, you need the following option
 `transparent_hugepage=never`

- In addition, we also set the following options:
    `log_buf_len=1024MB nosmap nosmep nox2apic dis_ucode_dlr msr.allow_writes=on nmi_watchdog=0 iomem=relaxed no_timer_check`

### Core Pinning
The aforementioned kernel command line options ensure that the core is not used by any other part of the system. To actually pin the VM to a core, you need to e.g. install [2].
### Frequency Fixing
There are two options for frequency pinning. Some systems allow to disable frequency scaling in the bios, such that all cores
run with a fixed frequency. The other alternative is to use Linux's cpu frequency scaling driver to fixate the frequency. 

In the first case, you can skip the rest of this section.

In the latter case, you can query `/sys/devices/system/cpu/<cpu core>/cpufreq/scaling_available_frequencies` to get a list of supported
target frequencies and use `cpufreq-set -r -c <cpu core>-g performance -u <freq> -d <freq>` to set the frequency. Afterwards use
`/sys/devices/system/cpu/<cpu core>/cpufreq/scaling_cur_freq` to query the current frequency to ensure that the pinning part actually worked.

### Putting it all together
- If you run without frequency scaling, issue `./post-startup-script.sh <isolated cpu> NULL` to prepare the VM for single stepping.
- If you run with frequency scaling AND have determined that `cpufreq-set`reliably pins some core the the frequency `freq`, use
`./post-startup-script.sh <isolated cpu> <freq>` to prepare the VM for single stepping.

## End2End Tests

Contains a server binary for the VM an a client binary for the host system.
Copy the server binary to the VM and start it. Afterwards start the client binary on the host.
The client will run a several tests  tests to ensure that the communication channel, page tracking and single stepping and cache attacks work properly.
If you want to skip some test, you have to edit `end2end-tests/host-client/main.c` and set the test's `.skip` attribute to `true`.

For the single stepping tests (e.g. `test_single_step_simple_long`) to work
you have to tweak the timer value at the start of the main function until you don't get any multisteps but still make some progress with single steps.



## References / Citations

[1] https://arxiv.org/pdf/2105.13824.pdf

[2] https://github.com/zegelin/qemu-affinity