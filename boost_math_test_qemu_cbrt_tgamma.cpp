///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <cstdint>

#include <boost_math_test_qemu.h>
#include <boost_math_test_qemu_detail.h>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

#if !defined(BOOST_MATH_STANDALONE)
#define BOOST_MATH_STANDALONE
#endif

#if !defined(BOOST_NO_RTTI)
#define BOOST_NO_RTTI
#endif

#if !defined(BOOST_DISABLE_THREADS)
#define BOOST_DISABLE_THREADS
#endif

#if !defined(BOOST_NO_EXCEPTIONS)
#define BOOST_NO_EXCEPTIONS
#endif

#if !defined(BOOST_MATH_MAX_SERIES_ITERATION_POLICY)
#define BOOST_MATH_MAX_SERIES_ITERATION_POLICY 1000U
#endif

#define BOOST_MATH_PROMOTE_FLOAT_POLICY false

#include <boost/cstdfloat.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/special_functions/gamma.hpp>

using my_float_type = boost::float64_t;

extern my_float_type cb;
extern my_float_type x;

auto boost::math::test::qemu::run_cbrt_tgamma() -> bool
{
  static_assert(std::numeric_limits<my_float_type>::digits >= 53,
                "Error: Incorrect float64_t type definition");

  // Table[N[Gamma[((456 n)/(100 Pi))^(1/3)], 21], {n, 1, 101, 10}]

  constexpr std::array<my_float_type, 11U> boost_math_test_qemu_control =
  {{
    my_float_type(BOOST_FLOATMAX_C(0.939131814550229181011151980662907952)),
    my_float_type(BOOST_FLOATMAX_C(1.34645417009670049450881257836426513)),
    my_float_type(BOOST_FLOATMAX_C(2.24865197861464813284549389915695163)),
    my_float_type(BOOST_FLOATMAX_C(3.54010862139328036868593771662409929)),
    my_float_type(BOOST_FLOATMAX_C(5.32678290474027222700118887035805285)),
    my_float_type(BOOST_FLOATMAX_C(7.74480814145513084404751762515242577)),
    my_float_type(BOOST_FLOATMAX_C(10.9608033153333194319415514453787809)),
    my_float_type(BOOST_FLOATMAX_C(15.1764307769968425576636827673821899)),
    my_float_type(BOOST_FLOATMAX_C(20.6341757772658373762859051310361056)),
    my_float_type(BOOST_FLOATMAX_C(27.6240717359003789929215625751284983)),
    my_float_type(BOOST_FLOATMAX_C(36.4914014381246411895171472115548616))
  }};

  static unsigned boost_math_test_qemu_n_index      = 0U;
  static unsigned boost_math_test_qemu_n_value      = 1U;
  static bool     boost_math_test_qemu_result_is_ok = true;

  cb = boost::math::tgamma(boost::math::cbrt(x * my_float_type(boost_math_test_qemu_n_value)));

  boost_math_test_qemu_result_is_ok &= detail::is_close_fraction(cb, boost_math_test_qemu_control[boost_math_test_qemu_n_index]);

  boost_math_test_qemu_n_value += 10U;

  ++boost_math_test_qemu_n_index;

  if(boost_math_test_qemu_n_index == boost_math_test_qemu_control.size())
  {
    boost_math_test_qemu_n_index = 0U;
    boost_math_test_qemu_n_value = 1U;
  }

  return boost_math_test_qemu_result_is_ok;
}

my_float_type cb;
my_float_type x = my_float_type(BOOST_FLOATMAX_C(4.56)) / boost::math::constants::pi<my_float_type>();

#if defined(BOOST_MATH_TEST_QEMU_STANDALONE_MAIN)

constexpr auto boost_math_test_qemu_standalone_foodcafe = static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE));

extern "C"
{
  extern volatile std::uint32_t qemu_standalone_result;

  auto boost_math_test_qemu_run_standalone       (void) -> bool;
  auto get_qemu_standalone_result(void) -> bool;

  auto boost_math_test_qemu_run_standalone(void) -> bool
  {
    auto result_is_ok = true;

    for(unsigned i = 0U; i < 64U; ++i)
    {
      result_is_ok &= boost::math::test::qemu::run_cbrt_tgamma();
    }

    qemu_standalone_result =
      static_cast<std::uint32_t>
      (
        result_is_ok ? boost_math_test_qemu_standalone_foodcafe : static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF))
      );

    return result_is_ok;
  }

  auto get_qemu_standalone_result(void) -> bool
  {
    volatile auto result_is_ok =
      (qemu_standalone_result == static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE)));

    return result_is_ok;
  }
}

int main()
{
  auto result_is_ok = true;

  result_is_ok = (::boost_math_test_qemu_run_standalone       () && result_is_ok);
  result_is_ok = (::get_qemu_standalone_result() && result_is_ok);

  return (result_is_ok ? 0 : -1);
}

extern "C"
{
  volatile std::uint32_t qemu_standalone_result;
}

#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
