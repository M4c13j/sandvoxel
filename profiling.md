# Profiling cheatheet
## With gpof (GNU profiler)
Executable must be compiled with `-pg` flag.
After running code, file `gmon.out` contains data gathered from that run.
Use command to generate readable file:
```bash
gprof EXECUTABLE_NAME > output.txt
```
voila!
