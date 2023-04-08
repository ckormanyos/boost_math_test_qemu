///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2023.
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
#include <boost/math/special_functions/bessel.hpp>

using my_float_type = boost::float64_t;

extern my_float_type cyj;
extern unsigned      xn;
extern my_float_type v;

auto boost::math::test::qemu::run_cyl_bessel_j() -> bool
{
  static_assert(std::numeric_limits<my_float_type>::digits >= 53,
                "Error: Incorrect float64_t type definition");

  // Table[N[BesselJ[123/100, n/10], 36], {n, 1, 10, 1}]

  using my_float_control_array_type = std::array<my_float_type, 10U>;

  constexpr my_float_control_array_type boost_math_test_qemu_control =
  {{
    static_cast<my_float_type>(BOOST_FLOATMAX_C(1.022384094800424078635130529844793803)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.052329252391407531307435184345226877)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.085683437351809687785091267647371436)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.121101214488587461397609703216026929)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.157737296526501682321434739413437594)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.194945869633552745026958864087027370)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.232184726291181620217067657303383795)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.268975614426188781953361485754411729)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.304885613485023234627936201325996814)),
    static_cast<my_float_type>(BOOST_FLOATMAX_C(0.339517879133361150735582864456735830)),
  }};

  static unsigned boost_math_test_qemu_n_index      = 0U;
  static bool     boost_math_test_qemu_result_is_ok = true;

  const auto x =
    static_cast<my_float_type>
    (
      static_cast<my_float_type>(xn) / static_cast<my_float_type>(BOOST_FLOATMAX_C(10))
    );

  cyj = boost::math::cyl_bessel_j(v, x);

  const auto& cyj_ctrl = boost_math_test_qemu_control.at(boost_math_test_qemu_n_index);

  boost_math_test_qemu_result_is_ok =
    (
         detail::is_close_fraction(cyj, cyj_ctrl)
      && boost_math_test_qemu_result_is_ok
    );

  ++xn;

  ++boost_math_test_qemu_n_index;

  if(boost_math_test_qemu_n_index == static_cast<unsigned>(std::tuple_size<my_float_control_array_type>::value))
  {
    boost_math_test_qemu_n_index = 0U;
    xn                    = 1U;
  }

  return boost_math_test_qemu_result_is_ok;
}

my_float_type cyj;
unsigned      xn = static_cast<unsigned>(UINT8_C(1));
my_float_type v  = static_cast<my_float_type>(BOOST_FLOATMAX_C(1.23));

#if defined(BOOST_MATH_TEST_QEMU_STANDALONE_MAIN)

constexpr auto boost_math_test_qemu_standalone_foodcafe = static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE));

extern "C"
{
  extern volatile std::uint32_t qemu_standalone_result;

  auto qemu_run_standalone       (void) -> bool;
  auto qemu_get_standalone_result(void) -> bool;

  auto qemu_run_standalone(void) -> bool
  {
    auto result_is_ok = true;

    for(unsigned i = 0U; i < 64U; ++i)
    {
      result_is_ok &= boost::math::test::qemu::run_cyl_bessel_j();
    }

    qemu_standalone_result =
      static_cast<std::uint32_t>
      (
        result_is_ok ? boost_math_test_qemu_standalone_foodcafe : static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF))
      );

    return result_is_ok;
  }

  auto qemu_get_standalone_result(void) -> bool
  {
    volatile auto result_is_ok =
      (qemu_standalone_result == static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE)));

    return result_is_ok;
  }
}

int main()
{
  auto result_is_ok = true;

  result_is_ok = (::qemu_run_standalone() && result_is_ok);
  result_is_ok = (::qemu_get_standalone_result() && result_is_ok);

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
