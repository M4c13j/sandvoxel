# Generowanie Flamegraph'ów
```bash
    sudo perf record -F 99 -a -g -- [command]
    perf script > out.perf
    ./stackcollapse-perf.pl out.perf > out.folded #folding stack (ONLY linux perf command)
    ./flamegraph.pl out.folded > kernel.svg
```
