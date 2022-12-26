///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MATH_TEST_QEMU_DETAIL_2022_12_26_H
  #define BOOST_MATH_TEST_QEMU_DETAIL_2022_12_26_H

  #include <cmath>
  #include <limits>

  namespace boost { namespace math { namespace test { namespace qemu { namespace detail {

  template<typename NumericType>
  auto is_close_fraction(const NumericType a,
                         const NumericType b,
                         const NumericType tol = NumericType(std::numeric_limits<NumericType>::epsilon() * NumericType(100))) -> bool
  {
    using std::fabs;

    const NumericType ratio     = fabs(NumericType((NumericType(1) * a) / b));

    const NumericType closeness = fabs(NumericType(1 - ratio));

    return (closeness < tol);
  }

  } } } } } // namespace boost::math::test::qemu::detail

#endif // BOOST_MATH_TEST_QEMU_DETAIL_2022_12_26_H
