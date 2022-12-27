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

#if !defined(BOOST_MP_STANDALONE)
#define BOOST_MP_STANDALONE
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

#include <boost/config.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace local_cpp_dec_float
{
  using big_float_backend_type =
    boost::multiprecision::cpp_dec_float<101, std::int32_t, void>;

  using big_float_type =
    boost::multiprecision::number<big_float_backend_type, boost::multiprecision::et_off>;
}

extern const local_cpp_dec_float::big_float_type big_float_arg;

auto boost::math::test::qemu::run_cbrt_cpp_dec_float() -> bool
{
  // Compute a cube root.
  const local_cpp_dec_float::big_float_type big_float_result = boost::math::cbrt(big_float_arg);

  // N[(123456/100)^(1/3), 111]
  // 10.7276369432283170454869317373527647801772956394047834686224956433128028534945259441672192774907629718402457465
  static const local_cpp_dec_float::big_float_type
    control
    {
      "10.7276369432283170454869317373527647801772956394047834686224956433128028534945259441672192774907629718402457465"
    };

  // Compare the calculated result with the known control value.
  const bool boost_math_test_qemu_result_is_ok = detail::is_close_fraction(big_float_result, control);

  return boost_math_test_qemu_result_is_ok;
}

const local_cpp_dec_float::big_float_type
  big_float_arg
  {
    local_cpp_dec_float::big_float_type(static_cast<std::uint32_t>(UINT32_C(123456))) / static_cast<unsigned>(UINT8_C(100))
  };

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
      result_is_ok &= boost::math::test::qemu::run_cbrt_cpp_dec_float();
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

  result_is_ok = (::qemu_run_standalone       () && result_is_ok);
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
