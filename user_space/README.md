# User space benchmark

To get significant results benchmarking a function with the x86_64 RDTSCP instruction in user space it's necessary to reduce jitter caused by interrupts.

Build the kernel with ```CONFIG_NO_HZ_FULL=y``` and ```CONFIG_RCU_NOCB_CPU=y``` kconfig options. 
The first option tell the kernel to avoid sending scheduling-clock interrupts to CPUs running a single task and the second option tell the kernel to offload RCU callbacks, in both cases we need to specify the interested CPUs to the bootloader.

Open with an editor ```/etc/default/grub``` and insert the command-line parameters ```GRUB_CMDLINE_LINUX_DEFAULT="irqaffinity=0,1 isolcpus=nohz,domain,2,3 nohz=on nohz_full=2,3 rcu_nocbs=2,3 rcu_nocb_poll"```

The option ```irqaffinity``` specify the set of CPUs, core 0 and 1, that can handle interrupts, ```isolcpus``` isolate core 2 and 3 from the general scheduler, ```nohz_full``` specify that core 2 and 3 need to not be interrupted by the scheduling-clock and ```rcu_nocbs``` offload RCU callbacks from core 2 and 3.

To update the bootloader configuration file run 
```# update-grub```
and reboot.
Before boot disable CPU frequency scaling, turbo, hyperthreading and power saving features from the BIOS settings to have less indeterminism.

The only way to run a process in the isolated CPUs is to set the affinity mask of the process to specific CPUs, a convenient way to do it is with ```taskset```.
To pin the benchmarking process to core 2 run 
```$ taskset -c 2 ./cycle_count```

Running the program without an user-provided function to be measured will report the overhead of the measurement itself, the statistics from this run are to be
subtracted from the respective statistics resulted from a run when a function to be measured is provided by the user.

Edit ```cycles_count.c``` to define a function to be measured and call it in ```count_cycles()```.

# References

https://www.kernel.org/doc/html/v4.14/admin-guide/kernel-parameters.html

https://www.kernel.org/doc/Documentation/timers/NO_HZ.txt

https://lore.kernel.org/patchwork/patch/888867/


