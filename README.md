boost_math_test_qemu
==================

[![Build Status](https://github.com/ckormanyos/boost_math_test_qemu/actions/workflows/boost_math_test_qemu.yml/badge.svg)](https://github.com/ckormanyos/boost_math_test_qemu/actions)

This repository tests certain small parts of
[Boost.Math](https://github.com/boostorg/math)
and
[Boost.Multiprecision](https://github.com/boostorg/multiprecision)
on a bare-metal ARM(R)-based embedded microcontroller system.

The emulated target is ST Microelectronics STM32F429ZI,
having a 32-bit ARM(R) Cortex(R)-M4F core.
The emulation uses 64-bit software floating-point (without FPU).

CI includes tests that verify both proper compilation
as well as correct numerical result-evaluation on the
embedded target using QEMU.

## Test Cases

The tests include:
  - [`boost_math_test_qemu_cbrt_cpp_bin_float.cpp`](./boost_math_test_qemu_cbrt_cpp_bin_float.cpp) which computes a $101$ digit cube root using Boost.Math's `cbrt()` function and Boost.Multiprecision's `cpp_bin_float` class.
  - [`boost_math_test_qemu_cbrt_cpp_dec_float.cpp`](./boost_math_test_qemu_cbrt_cpp_dec_float.cpp) which does the same cube root calculation, this time with the `cpp_dec_float` class.
  - [`boost_math_test_qemu_cbrt_tgamma.cpp`](./boost_math_test_qemu_cbrt_tgamma.cpp) performs various 64-bit floating-point calculations involving the Gamma function of cube root values.
  - [`boost_math_test_qemu_cyl_bessel_j.cpp`](./boost_math_test_qemu_cyl_bessel_j.cpp) tests a small selection of cylindrical Bessel function of small order.

## QEMU Test Cycle

The QEMU test cycle involves the following steps:
  - Compile and link the code to absolute ELF-File.
  - Use `gcc-arm-none-eabi` for compilation (switch off RTTI, exceptions, etc.).
  - Compile for STM32F429 ARM(R)-based Cortex(R) M4F core.
  - Start QEMU for the target.
  - In another core (i.e., a different Bash-instance), load the ELF-File for execution with debug via GDB controlled by Python script.

Clear and easy-to-understand steps can be found in the
`ubuntu-boost-math-test-qemu` jobs in CI.

This repository provides keen insight on running and testing
small parts of
[Boost.Math](https://github.com/boostorg/math)
and
[Boost.Multiprecision](https://github.com/boostorg/multiprecision)
on a _bare_ _metal_ ARM(R) controller.

## Tools Used

The following tools are used:
  - [`gcc-arm-none-eabi 10.3-2021.10-x86_64-linux`](https://developer.arm.com/downloads/-/gnu-rm) from ARM(R) LTD for compiling/linking the embedded code.
  - QEMU distribution found in [xpack-qemu-arm-7.1.0-1](https://github.com/xpack-dev-tools/qemu-arm-xpack/releases).
  - GDB controlled by Python 2.7 script (i.e., [boost_math_test_qemu.py](./target/build/boost_math_test_qemu.py)), where Python 2.7 itself needs an installed `libncurses5`.
  - [Boost.Math](https://github.com/boostorg/math) and [Boost.Multiprecision](https://github.com/boostorg/multiprecision) submodules in their latest states at GitHub.

## Details on the Application

The application boots from a tiny, self-written
[startup code](./target/micros/stm32f429/make/single/crt.cpp)
located at the mocrocontroller's hardware start address.
Addresses are defined in a specialized
[Linker-definition file](./target/micros/stm32f429/make/stm32f429.ld).

Following static initialization (zero-clear, data-initialization
and static constructors), the program jumps to
the `main()` subroutine. Here the test runs and provides
a skinny logic to be used in a GDB breakpoint when running
in QEMU.

In each test case, the local static variable `qemu_standalone_result`
is set to the self-invented, special key-value `0xF00DCAFE`
to indicate test-pass (fail sets the value to `0xFFFFFFFF`).
The key-value `0xF00DCAFE` is explicitly checked by the Python-GDB script
in order to provide a strong determination of test-pass.

## Building the Application

Build on `*nix*` is easy using the tools and commands
shown in CI.

### Prerequisites: The Embedded Compiler and Boost Math/Multiprecision

  - For build and link, the main prerequisite is to get (via `wget` or similar)
  the embedded compiler. Let's place this, for example, in a self-created
  directory called `emu_env` within the root directory of the repo
  `boost_math_test_qemu`.
  - A second prerequisite is to get and provide the include path of
  the Boost Math and Multiprecision headers. In the command below,
  the relevant include paths are provided by the command-fragment `-I../boost-root`.
  You can, of course, use a different location of your choice for the root of Boost.

Possible commands for setting up the prerequisites are listed below.

```sh
cd boost_math_test_qemu
mkdir -p emu_env && cd emu_env
wget --no-check-certificate https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
tar -xvf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
```

### Compile the Embedded Code

Below are sample commands for building and linking one of the tests.
Let's take, for instance, the test case `boost_math_test_qemu_cbrt_tgamma.cpp`.

Build and link in Bash.

```sh
cd boost_math_test_qemu
emu_env/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++ -std=c++14 -Wall -Wextra -Wpedantic -O2 -g -gdwarf-2 -ffunction-sections -fdata-sections -x c++ -fno-rtti -fno-use-cxa-atexit -fno-exceptions -fno-nonansi-builtins -fno-threadsafe-statics -fno-enforce-eh-specs -ftemplate-depth=128 -mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -mfloat-abi=soft -mno-unaligned-access -mno-long-calls -DBOOST_MATH_TEST_QEMU_STANDALONE_MAIN -I. -I../boost-root ./boost_math_test_qemu_cbrt_tgamma.cpp ./target/micros/stm32f429/make/single/crt.cpp -nostartfiles -Wl,--gc-sections -Wl,-Map,./bin/boost_math_test_qemu.map -T ./target/micros/stm32f429/make/stm32f429.ld -Wl,--print-memory-usage --specs=nano.specs --specs=nosys.specs -o ./bin/boost_math_test_qemu.elf
```

This super-long command compiles the C++ source file
`./boost_math_test_qemu_cbrt_tgamma.cpp`
together with its startup code
`./target/micros/stm32f429/make/single/crt.cpp`.

The command compiles and subsequently performs a full-link
to absolute object ELF-File.

A bunch of command-line switches are used.

True fans of bare-metal efficiency might note the
self-written
[startup code](./target/micros/stm32f429/make/single/crt.cpp),
DIY
[linker definition file](./target/micros/stm32f429/make/stm32f429.ld),
compilation with RTT/exceptions switched off,
and linking with `nano`/`nosys` specs with respect
to the standard libraries.

The result of this command is: `./bin/boost_math_test_qemu.elf`.
This command assumes that the embedded compiler has been retrieved
with `wget` (or similar) and unpacked as shown above
in the self-created directory `emu_env`.

### Running the Simulated Target on QEMU

Once the ELF-file (`./bin/boost_math_test_qemu.elf`) has been created,
as shown above. It can run in QEMU on the simulated target.

We will now run on QEMU.

In a command shell such as a bash session, run QEMU.

```sh
./emu_env/xpack-qemu-arm-7.1.0-1/bin/qemu-system-gnuarmeclipse --verbose --mcu STM32F429ZI --nographic --gdb tcp::9999 -d unimp,guest_errors &
```

In a second command shell such as a bash session, run the provided Python script
for controlling GDB.

```sh
./emu_env/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gdb-py ./bin/boost_math_test_qemu.elf -x ./target/build/boost_math_test_qemu.py
```

The GDB session loads the ELF-file, runs it and
assesses the value of a success/failure variable in a break-point
in a key location on the embedded target.

The internal magic number `0xF00DCAFE` is used to indicate
success or failure, which is _caught_ for each numerical test
in a break point.
An example of _catching_ success based on the magic number
`0xF00DCAFE` is shown, for instance,
[here](https://github.com/ckormanyos/boost_math_test_qemu/blob/6be4ed755d56408925e2d8c40d9230a5c3fd3076/boost_math_test_qemu_cyl_bessel_j.cpp#L141).

Based on the capture of success/failure, the value zero (for success)
or non-zero (for fail) is returned as the result of the Python script.
The LINUX operating system subsequently obtains this as pass/fail
reason in each QEMU run in CI.

### Extract HEX and Demangled Symbols/Sizes

The following (optional) commands extract the HEX-file and provide a text-based listing
of the demangled symbols and their sizes within the object file.

These commands are optional and do not need to be executed in order to run
the simulated target on QEMU.


```sh
cd boost_math_test_qemu
emu_env/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-objcopy ./bin/boost_math_test_qemu.elf -O ihex ./bin/boost_math_test_qemu.hex
emu_env/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-nm --numeric-sort --print-size ./bin/boost_math_test_qemu.elf | emu_env/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-c++filt > ./bin/boost_math_test_qemu_cppfilt.txt
```

The results of these commands are: `./bin/boost_math_test_qemu.hex`
and `./bin/boost_math_test_qemu_cppfilt.txt`. The latter text-based
list file contains full demangled names and sizes of the components
in the compiled executable on the metal.
