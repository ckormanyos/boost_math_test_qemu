///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MATH_TEST_QEMU_2022_12_26_H
  #define BOOST_MATH_TEST_QEMU_2022_12_26_H

  #include <cmath>
  #include <limits>

  namespace boost { namespace math { namespace test { namespace qemu {

  auto run_cbrt_tgamma       () -> bool;
  auto run_cbrt_cpp_dec_float() -> bool;

  } } } } // namespace boost::math::test::qemu

#endif // BOOST_MATH_TEST_QEMU_2022_12_26_H
