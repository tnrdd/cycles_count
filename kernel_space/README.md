# Kernel space benchmark

To get significant results benchmarking a function with the x86_64 RDTSCP assembly instruction we need to reduce jitter caused by interrupts,
running the program as a kernel module allow us to disable preemption and hardware interrupts during the measurement.

From the BIOS settings we disable CPU frequency scaling, turbo, hyperthreading and power saving features to have less indeterminism.

We run ```$ make``` to build the program.

Then we run ```# insmod cycles_count.ko``` to insert the module in the kernel and ```# rmmod cycles_count``` to remove it.

The statistics are logged in ```/var/log/syslog```.
If we run ```insmod cycles_count.ko``` and ```rmmod cycles_count``` from a console the statistics are dispayed there.

Running the program without an user-provided function to be measured will report the overhead of the measurement itself, the statistics from this run are to be
subtracted from the respective statistics resulted from a run when a function to be measured is provided by the user.

We edit ```cycles_count.c``` to define a function to be measured and call it in the ```count_cycles``` function.

# Referencies

https://www.kernel.org/doc/Documentation/preempt-locking.txt
