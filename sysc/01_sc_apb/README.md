# Example Verilator + SystemC

```
01_sc_apb
├ ─ ─  CMakeLists.txt
├ ─ ─  README.md
├ ─ ─  rtl
│    ├ ─ ─  clk_gate.v
│    ├ ─ ─  dut_reg.v
│    └ ─ ─  dut_top.v
├ ─ ─  sc_main.cpp
└ ─ ─  tb
    ├ ─ ─  apb_req.cpp
    └ ─ ─  apb_req.hpp
```

## Build

Setting at some variables.
- `SYSTEMC_INCLUDE`
- `SYSTEMC_LIBDIR`

```sh
$ mkdir build; cd build
$ cmake ..
$ make
```
## Execute

```sh
$ ./run.x

        SystemC 2.3.3-Accellera --- Aug 16 2022 12:13:48
        Copyright (c) 1996-2018 by all Contributors,
        ALL RIGHTS RESERVED
*** Start Simulation
*** Release Reset at 301 ns
*** WakeUp at 601 ns
*** End Simulation at 1051 ns
$ ls
CMakeCache.txt      CMakeFiles          Makefile            cmake_install.cmake coverage.dat        run.x
```

Add `+trace` option.

```sh
$ ./run.x +trace

        SystemC 2.3.3-Accellera --- Aug 16 2022 12:13:48
        Copyright (c) 1996-2018 by all Contributors,
        ALL RIGHTS RESERVED
Enabling waves into vlt_dump.vcd...
*** Start Simulation
*** Release Reset at 301 ns
*** WakeUp at 601 ns
*** End Simulation at 1051 ns
$ ls
CMakeCache.txt      CMakeFiles          Makefile            cmake_install.cmake coverage.dat        run.x               vlt_dump.vcd
```

