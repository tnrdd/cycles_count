# User space benchmark

To get significant results benchmarking a function with the x86_64 RDTSCP instruction in user space we need to reduce jitter caused by interrupts.

We build the kernel with ```CONFIG_NO_HZ_FULL=y``` and ```CONFIG_RCU_NOCB_CPU=y``` kconfig options. 
The first option tell the kernel to avoid sending scheduling-clock interrupts to CPUs running a single task and the second option tell the kernel to offload RCU callbacks, in both cases we need to specify the interested CPUs to the bootloader.

We open with an editor ```/etc/default/grub``` and insert the command-line parameters ```GRUB_CMDLINE_LINUX_DEFAULT="irqaffinity=0,1 isolcpus=nohz,domain,2,3 nohz=on nohz_full=2,3 rcu_nocbs=2,3 rcu_nocb_poll"```

With ```irqaffinity``` we specify the set of CPUs, core 0 and 1, that can handle interrupts, we isolate core 2 and 3 from the general scheduler with ```isolcpus```, specify that we want core 2 and 3 to not be interrupted by the scheduling-clock with ```nohz_full``` and offload RCU callbacks from core 2 and 3 with ```rcu_nocbs```.

To update the bootloader configuration file we run 
```# update-grub```
and reboot.
Before boot we disable CPU frequency scaling, turbo, hyperthreading and power saving features from the BIOS settings to have less indeterminism.

The only way to run a process in the isolated CPUs is to set the affinity mask of the process to specific CPUs, a convenient way to do it is with ```taskset```.
To pin the benchmarking process to core 2 we run 
```$ taskset -c 2 ./cycle_count```

Running the program without an user-provided function to be measured will report the overhead of the measurement itself, the statistics from this run are to be
subtracted from the respective statistics resulted from a run when a function to be measured is provided by the user.

We edit ```cycles_count.c``` to define a function to be measured and call it in the ```count_cycles``` function.

# Referencies

https://www.kernel.org/doc/html/v4.14/admin-guide/kernel-parameters.html

https://www.kernel.org/doc/Documentation/timers/NO_HZ.txt

https://lore.kernel.org/patchwork/patch/888867/


