// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_students_t.cpp

// http://en.wikipedia.org/wiki/Student%27s_t_distribution

// Basic sanity test for Student's t probability (quintile) (0. < p < 1).
// and Student's t probability Quintile (0. < p < 1).

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  if !(defined _SCL_SECURE_NO_DEPRECATE) || (_SCL_SECURE_NO_DEPRECATE == 0)
#    pragma warning(disable: 4996) // 'std::char_traits<char>::copy' was declared deprecated.
     // #define _SCL_SECURE_NO_DEPRECATE = 1 // avoid C4996 warning.
#  endif
//#  pragma warning(disable: 4244) // conversion from 'double' to 'float', possible loss of data.
#endif

#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>

#include <boost/math/dist/students_t.hpp>
	 using boost::math::students_t_distribution;
#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/math/tools/test.hpp> // for real_concept

#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;
#include <limits>
  using std::numeric_limits;

template <class RealType>
RealType naive_pdf(RealType v, RealType t)
{
   // Calculate the pdf of the students t in a deliberately
   // naive way, using equation (5) from 
   // http://mathworld.wolfram.com/Studentst-Distribution.html
   // This is equivalent to, but a different method
   // to the one in the actual implementation, so can be used as
   // a very basic sanity check.  However some published values
   // would be nice....

   using namespace std;  // for ADL
   using boost::math::beta;

   //return pow(v / (v + t*t), (1+v) / 2) / (sqrt(v) * beta(v/2, RealType(0.5f)));
   RealType result = boost::math::tgamma_ratio((v+1)/2, v/2);
   result /= sqrt(v * boost::math::constants::pi<RealType>());
   result /= pow(1 + t*t/v, (v+1)/2);
   return result;
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic santity checks
	RealType tolerance = static_cast<RealType>(std::pow(10., -(6-2))); // 1e-6 (as %)
	// Some tests only pass at 1e-5 because probability value is less accurate,
	// a digit in 6th decimal place, although calculated using 
	// a t-distribution generator (claimed 6 decimal digits) at
   // http://faculty.vassar.edu/lowry/VassarStats.html
	// http://faculty.vassar.edu/lowry/tsamp.html
	// df = 5, +/-t = 2.0, 1-tailed = 0.050970, 2-tailed = 0.101939

	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

	// http://en.wikipedia.org/wiki/Student%27s_t_distribution#Table_of_selected_values
   // Using tabulated value of t = 3.182 for 0.975, 3 df, one-sided.

	// http://www.mth.kcl.ac.uk/~shaww/web_page/papers/Tdistribution06.pdf refers to:

	// A lookup table of quantiles of the RealType distribution
   // for 1 to 25 in steps of 0.1 is provided in CSV form at:
   // www.mth.kcl.ac.uk/~shaww/web_page/papers/Tsupp/tquantiles.csv
	// gives accurate t of -3.1824463052837 and 3 degrees of freedom.
	// Values below are from this source, saved as tquantiles.xls.
	// DF are across the columns, probabilities down the rows
	// and the t- values (quantiles) are shown.
	// These values are probably accurate to nearly 64-bit double
   // (perhaps 14 decimal digits).

   RealType big = std::numeric_limits<RealType>::has_infinity ?
      std::numeric_limits<RealType>::infinity() :
      boost::math::tools::max_value<RealType>();

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(2),       // degrees_of_freedom
         static_cast<RealType>(-6.96455673428326)),  // t
         static_cast<RealType>(0.01),                // probability.
			tolerance); // % 

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(5),       // degrees_of_freedom
         static_cast<RealType>(-3.36492999890721)),  // t
         static_cast<RealType>(0.01),                // probability.
			tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(1),      // degrees_of_freedom
         static_cast<RealType>(-31830.988607907)),  // t
         static_cast<RealType>(0.00001),            // probability.
			tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(25.),    // degrees_of_freedom
         static_cast<RealType>(-5.2410429995425)),  // t
         static_cast<RealType>(0.00001),            // probability.
			tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(1),   // degrees_of_freedom
         static_cast<RealType>(-63661.97723)),   // t
         static_cast<RealType>(0.000005),        // probability.
			tolerance);

    BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(-17.89686614)),   // t
         static_cast<RealType>(0.000005),        // probability.
			tolerance);

    BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(25.),  // degrees_of_freedom
         static_cast<RealType>(-5.510848412)),    // t
         static_cast<RealType>(0.000005),         // probability.
			tolerance);

  BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(10.),  // degrees_of_freedom
         static_cast<RealType>(-1.812461123)),    // t
         static_cast<RealType>(0.05),             // probability.
			tolerance);

  BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(10),  // degrees_of_freedom
         static_cast<RealType>(1.812461123)),    // t
         static_cast<RealType>(0.95),            // probability.
			tolerance);

  BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(
            students_t_distribution<RealType>(10),  // degrees_of_freedom
            static_cast<RealType>(1.812461123))),    // t
         static_cast<RealType>(0.05),            // probability.
			tolerance);

  BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(10),  // degrees_of_freedom
         static_cast<RealType>(9.751995491)),    // t
         static_cast<RealType>(0.999999),        // probability.
			tolerance);

  BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(10.),  // degrees_of_freedom - for ALL degrees_of_freedom!
         static_cast<RealType>(0.)),              // t
         static_cast<RealType>(0.5),              // probability.
			tolerance);

	// Student's t Inverse function tests.
  // Special cases

  BOOST_CHECK_EQUAL(boost::math::quantile(
         students_t_distribution<RealType>(1.),  // degrees_of_freedom (ignored).
         static_cast<RealType>(0)),  //  probability == half - special case.
         -big); // t == -infinity.
			
  BOOST_CHECK_EQUAL(boost::math::quantile(
         students_t_distribution<RealType>(1.),  // degrees_of_freedom (ignored).
         static_cast<RealType>(1)),  //  probability == half - special case.
         +big); // t == +infinity.

  BOOST_CHECK_EQUAL(boost::math::quantile(
         students_t_distribution<RealType>(1.),  // degrees_of_freedom (ignored).
         static_cast<RealType>(0.5)),  //  probability == half - special case.
         static_cast<RealType>(0)); // t == zero.

  BOOST_CHECK_EQUAL(boost::math::quantile(
         complement(
            students_t_distribution<RealType>(1.),  // degrees_of_freedom (ignored).
            static_cast<RealType>(0.5))),  //  probability == half - special case.
         static_cast<RealType>(0)); // t == zero.

  BOOST_CHECK_CLOSE(boost::math::quantile(
         students_t_distribution<RealType>(1.),  // degrees_of_freedom (ignored).
         static_cast<RealType>(0.5)),  //  probability == half - special case.
         static_cast<RealType>(0), // t == zero.
			tolerance);

   BOOST_CHECK_CLOSE( // Tests of p middling.
      ::boost::math::cdf(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(-0.559429644)),  // t
         static_cast<RealType>(0.3), // probability.
			tolerance);
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(0.3)),  // probability.
         static_cast<RealType>(-0.559429644), // t
			tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         complement(
            students_t_distribution<RealType>(5.),  // degrees_of_freedom
            static_cast<RealType>(0.7))),  // probability.
         static_cast<RealType>(-0.559429644), // t
			tolerance);

   BOOST_CHECK_CLOSE( // Tests of p high.
      ::boost::math::cdf(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(1.475884049)),  // t
         static_cast<RealType>(0.9), // probability.
			tolerance);
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(0.9)),  // probability.
         static_cast<RealType>(1.475884049), // t
			tolerance);

   BOOST_CHECK_CLOSE( // Tests of p low.
      ::boost::math::cdf(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(-1.475884049)),  // t
         static_cast<RealType>(0.1), // probability.
			tolerance);
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         students_t_distribution<RealType>(5.),  // degrees_of_freedom
         static_cast<RealType>(0.1)),  // probability.
         static_cast<RealType>(-1.475884049), // t
			tolerance);

   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         students_t_distribution<RealType>(2.),  // degrees_of_freedom
         static_cast<RealType>(-6.96455673428326)),  // t
         static_cast<RealType>(0.01), // probability.
			tolerance);
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         students_t_distribution<RealType>(2.),  // degrees_of_freedom
         static_cast<RealType>(0.01)),  // probability.
         static_cast<RealType>(-6.96455673428326), // t
			tolerance);

   //
   // PDF checks, use 100eps tolerance expressed as a persent:
   //
   tolerance = boost::math::tools::epsilon<RealType>() * 10000; 
   for(unsigned i = 1; i < 20; i += 3)
   {
      for(RealType r = -10; r < 10; r += 0.125)
      {
         //std::cout << "df=" << i << " t=" << r << std::endl;
         BOOST_CHECK_CLOSE(
            boost::math::pdf(
               students_t_distribution<RealType>(i), 
               r),
            naive_pdf<RealType>(i, r),
            tolerance);
      }
   }

} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{
	 // Basic sanity-check spot values.
	// (Parameter value, arbitrarily zero, only communicates the floating point type).
  test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
  test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
  test_spots(0.0L); // Test long double.
  test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.

   return 0;
} // int test_main(int, char* [])

/*

Output:

Compiling...
test_students_t.cpp
Linking...
Embedding manifest...
Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_students_t.exe"
Running 1 test case...
tolerance = 100 %
*** No errors detected



*/

