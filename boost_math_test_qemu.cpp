///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdint>
#include <iomanip>
#include <iostream>

#include <boost_math_test_qemu.h>

namespace local
{
  auto runner =
  [](bool(*pfn)(void)) -> bool
  {
    auto result_is_ok = true;

    for(auto   i = static_cast<unsigned>(UINT8_C(0));
               i < static_cast<unsigned>(UINT8_C(64));
             ++i)
    {
      result_is_ok = (pfn() && result_is_ok);
    }

    return result_is_ok;
  };
}

int main()
{
  auto result_is_ok = true;

  result_is_ok = (local::runner(boost::math::test::qemu::run_cbrt_tgamma       ) && result_is_ok);
  result_is_ok = (local::runner(boost::math::test::qemu::run_cbrt_cpp_dec_float) && result_is_ok);

  {
    auto flg = std::cout.flags();

    std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

    std::cout.flags(flg);
  }

  return (result_is_ok ? 0 : -1);
}
