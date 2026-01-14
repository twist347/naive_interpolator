#include "gtest/gtest.h"

#include <iostream>
#include <vector>

#include "ni.h"

TEST(interp, A) {
    const std::vector<double> x{1, 3, 5}, y{1, 27, 125};
    const auto interp = ni::d1::make_owner_try<ni::d1::method::nearest>(x, y);
    const auto y_res = interp(3);
    std::cout << y_res << std::endl;
}

TEST(interp, B) {
    const std::vector<double> x{2, 4, 6, 8}, y{4, 16, 36, 64};

    const auto interp = ni::d1::make_view_try<ni::d1::method::cubic>(x, y);

    // auto view = interp.as_view();

    // const auto interp = ni::d1::interp_owner<ni::d1::method::cubic, double>::make_try({x}, {y});

    const std::vector<double> xq{5, 3, 7};
    std::vector<double> y_res(xq.size());

    interp.eval(xq, y_res);

    for (const auto val : y_res) {
        std::cout << val << std::endl;
    }
}
