# Profiling cheatheet
## With gpof (GNU profiler)
Executable must be compiled with `-pg` flag.
After running code, file `gmon.out` contains data gathered from that run.
Use command to generate readable file:
```bash
gprof EXECUTABLE_NAME > output.txt
```
voila!


## With perf
Use following comands:
```bash
    sudo perf record -g ./path/to/exec
    sudo perf report -g 'graph,.5,caller'
'''

