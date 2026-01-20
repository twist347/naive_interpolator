#include "gtest/gtest.h"

#include <vector>

#include "ni/d1/owner.h"
#include "ni/d1/view.h"

TEST(interp, make_view_try) {
    const std::vector<double> x{1, 3, 5}, y{1, 27, 125};
    constexpr auto xq = 3.0;
    constexpr auto y_exp = 27.0;

    const auto view = ni::d1::make_view_try<ni::d1::kind::nearest>(x, y);

    const auto x_view = view.x(), y_view = view.y();
    ASSERT_TRUE(std::equal(x_view.cbegin(), x_view.cend(), x.cbegin()));
    ASSERT_TRUE(std::equal(y_view.cbegin(), y_view.cbegin(), y.cbegin()));

    const auto y_res = view(xq);

    ASSERT_EQ(y_res, y_exp);
}

TEST(interp, make_owner_try) {
    const std::vector<double> x{1, 3, 5, 7}, y{2, 6, 10, 14};
    const std::vector<double> xqs{2, 4, 6};
    const std::vector<double> y_exp{4, 8, 12};
    std::vector<double> y_res(xqs.size());

    const auto owner = ni::d1::make_owner_try<ni::d1::kind::linear>(x, y);
    const auto x_view = owner.x(), y_view = owner.y();

    ASSERT_TRUE(std::equal(x_view.cbegin(), x_view.cbegin(), x.cbegin()));
    ASSERT_TRUE(std::equal(y_view.cbegin(), y_view.cbegin(), y.cbegin()));

    owner.eval(xqs, y_res);

    ASSERT_EQ(y_res, y_exp);
}

TEST(interp, owner_as_view) {
    const std::vector<double> x{1, 3, 5, 7}, y{2, 6, 10, 14};
    const std::vector<double> xqs{2, 4, 6};
    const std::vector<double> y_exp{4, 8, 12};
    std::vector<double> y_res(xqs.size());

    const auto owner = ni::d1::make_owner_try<ni::d1::kind::linear>(x, y);
    const auto view = owner.as_view();

    const auto x_view = view.x(), y_view = view.y();

    ASSERT_TRUE(std::equal(x_view.cbegin(), x_view.cbegin(), x.cbegin()));
    ASSERT_TRUE(std::equal(y_view.cbegin(), y_view.cbegin(), y.cbegin()));

    view.eval(xqs, y_res);

    ASSERT_EQ(y_res, y_exp);
}
