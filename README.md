boost_math_test_qemu
==================

[![Build Status](https://github.com/ckormanyos/boost_math_test_qemu/actions/workflows/boost_math_test_qemu.yml/badge.svg)](https://github.com/ckormanyos/boost_math_test_qemu/actions)

This repository tests certain small parts of Boost.Math
on a bare-metal ARM(R)-based embedded microcontroller system.

CI includes tests that verify the correct operation on the embedded target
using QEMU.

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
  - In another thread (another core/Bash-session), load the ELF-File in a QEMU-debug session controlled by Python GDB script.

Clear and easy-to-understand steps can be found in the
`ubuntu-boost-math-test-qemu` jobs in CI.

This repository provides keen insight on running and testing
small parts of Boost.Math (and Boost.Multiprecision)
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
