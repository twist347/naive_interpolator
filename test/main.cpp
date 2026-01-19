#include "gtest/gtest.h"

#include <vector>

#include "ni/d1/owner.h"
#include "ni/d1/view.h"

TEST(interp, make_view_try) {
    const std::vector<double> x{1, 3, 5}, y{1, 27, 125};
    constexpr auto y_exp = 27.0;

    const auto interp = ni::d1::make_view_try<ni::d1::kind::nearest>(x, y);

    const auto x_view = interp.x(), y_view = interp.y();
    ASSERT_TRUE(std::equal(x_view.cbegin(), x_view.cend(), x.cbegin()));
    ASSERT_TRUE(std::equal(y_view.cbegin(), y_view.cbegin(), y.cbegin()));

    const auto y_res = interp(3);

    ASSERT_EQ(y_res, y_exp);
}

TEST(interp, make_owner_try) {
    const std::vector<double> x{2, 4, 6, 8}, y{4, 16, 36, 64};
    const std::vector<double> y_exp{25, 9, 49};

    const auto interp = ni::d1::make_owner_try<ni::d1::kind::cubic>(x, y);
    const auto x_view = interp.x(), y_view = interp.y();

    ASSERT_TRUE(std::equal(x_view.cbegin(), x_view.cbegin(), x.cbegin()));
    ASSERT_TRUE(std::equal(y_view.cbegin(), y_view.cbegin(), y.cbegin()));

    const std::vector<double> xq{5, 3, 7};
    std::vector<double> y_res(xq.size());

    interp.eval(xq, y_res);

    ASSERT_TRUE(std::equal(y_exp.cbegin(), y_exp.cend(), y_res.cbegin()));
}
