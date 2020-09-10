# Cycles Count
Read the TSC register with the x86_64 RDTSCP instruction to benchmark the execution of a function in CPU clock cycles granularity
from both kernel and user space.
LFENCE is used to avoid other instructions being executed before the reading is complete.

# References

https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf

https://stackoverflow.com/a/51907627/13024639

