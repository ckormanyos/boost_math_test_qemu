#include <iomanip>
#include <iostream>

#include <boost_math_test_qemu.h>

int main()
{
  auto result_is_ok = true;

  result_is_ok = (boost::math::test::qemu::run_cbrt_tgamma       () && result_is_ok);
  result_is_ok = (boost::math::test::qemu::run_cbrt_cpp_dec_float() && result_is_ok);

  {
    auto flg = std::cout.flags();

    std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

    std::cout.flags(flg);
  }
}
