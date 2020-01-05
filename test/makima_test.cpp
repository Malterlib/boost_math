/*
 * Copyright Nick Thompson, 2020
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "math_unit_test.hpp"
#include <numeric>
#include <utility>
#include <random>
#include <boost/math/interpolators/makima.hpp>
#ifdef BOOST_HAS_FLOAT128
#include <boost/multiprecision/float128.hpp>
using boost::multiprecision::float128;
#endif


using boost::math::interpolators::makima;

template<typename Real>
void test_constant()
{

    std::vector<Real> x{0,1,2,3, 9, 22, 81};
    std::vector<Real> y(x.size());
    for (auto & t : y) {
        t = 7;
    }

    auto x_copy = x;
    auto y_copy = y;
    auto akima = makima(std::move(x_copy), std::move(y_copy));

    for (Real t = x[0]; t <= x.back(); t += 0.25) {
        CHECK_ULP_CLOSE(7, akima(t), 2);
    }
}

template<typename Real>
void test_linear()
{
    std::vector<Real> x{0,1,2,3};
    std::vector<Real> y{0,1,2,3};

    auto x_copy = x;
    auto y_copy = y;
    auto akima = makima(std::move(x_copy), std::move(y_copy));

    CHECK_ULP_CLOSE(y[0], akima(x[0]), 0);
    CHECK_ULP_CLOSE(Real(1)/Real(2), akima(Real(1)/Real(2)), 10);
    CHECK_ULP_CLOSE(y[1], akima(x[1]), 0);
    CHECK_ULP_CLOSE(Real(3)/Real(2), akima(Real(3)/Real(2)), 10);
    CHECK_ULP_CLOSE(y[2], akima(x[2]), 0);
    CHECK_ULP_CLOSE(Real(5)/Real(2), akima(Real(5)/Real(2)), 10);
    CHECK_ULP_CLOSE(y[3], akima(x[3]), 0);

    x.resize(45);
    y.resize(45);
    for (size_t i = 0; i < x.size(); ++i) {
        x[i] = i;
        y[i] = i;
    }

    x_copy = x;
    y_copy = y;
    akima = makima(std::move(x_copy), std::move(y_copy));
    for (Real t = 0; t < x.back(); t += 0.5) {
        CHECK_ULP_CLOSE(t, akima(t), 0);
    }
}

template<typename Real>
void test_interpolation_condition()
{
    for (size_t n = 4; n < 50; ++n) {
        std::vector<Real> x(n);
        std::vector<Real> y(n);
        std::default_random_engine rd;
        std::uniform_real_distribution<Real> dis(0,1);
        Real x0 = dis(rd);
        x[0] = x0;
        y[0] = dis(rd);
        for (size_t i = 1; i < n; ++i) {
            x[i] = x[i-1] + dis(rd);
            y[i] = dis(rd);
        }

        auto x_copy = x;
        auto y_copy = y;
        auto s = makima(std::move(x_copy), std::move(y_copy));
        for (size_t i = 0; i < x.size(); ++i) {
            CHECK_ULP_CLOSE(y[i], s(x[i]), 2);
        }

    }
}

int main()
{
    test_constant<float>();
    test_linear<float>();
    test_interpolation_condition<float>();

    test_constant<double>();
    test_linear<double>();
    test_interpolation_condition<double>();

    test_constant<long double>();
    test_linear<long double>();
    test_interpolation_condition<long double>();


    return boost::math::test::report_errors();
}
