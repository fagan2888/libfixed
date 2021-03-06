//
// The MIT License (MIT)
//
//
// Copyright (c) 2013 OANDA Corporation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include "fixed/FirstBitSet.h"
#include "TestsCommon.h"

#include <functional>
#include <iostream>
#include <vector>

namespace fixed {
namespace test {

class RoundingTest {
  public:
    struct ValResult {
        std::string initialVal;
        unsigned int decimalPlaces;
        std::string expectedRoundedVal;
    };

    RoundingTest (
        Rounding::Mode mode,
        const std::vector<ValResult> valResults
    )
      : mode_ (mode),
        valResults_ (valResults)
    {}

    bool operator() ()
    {
        Number::setDefaultRoundingMode (mode_);

        for (const auto& vr: valResults_)
        {
            Number n (vr.initialVal);
            n.setDecimalPlaces (vr.decimalPlaces);

            std::string name =
                "Initial val " + vr.initialVal + " Decimal places " +
                std::to_string (static_cast<int> (vr.decimalPlaces));

            if (! checkNumber (name, n, Number (vr.expectedRoundedVal)))
            {
                return false;
            }
        }

        return true;
    }

  private:
    const std::string name_;
    const Rounding::Mode mode_;
    const std::vector<ValResult> valResults_;
};

static Test createTest (
    Rounding::Mode mode,
    const std::vector<RoundingTest::ValResult>& valResults
)
{
    return Test (
        RoundingTest (mode, valResults),
        [=] () {
            return "Rounding mode '" + Rounding::modeToString (mode) + "'";
        }
    );
}

std::vector<Test> NumberRoundingTestVec = {
    createTest (
        Rounding::Mode::DOWN,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.4"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.4"},
            {"1234.45000000000001",  1, "1234.4"},
            {"1234.49",              1, "1234.4"},
            {"1234.49999999999999",  1, "1234.4"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.5"},
            {"-1234.45",             1, "-1234.5"},
            {"-1234.44999999999999", 1, "-1234.5"},
            {"-1234.45000000000000", 1, "-1234.5"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.5"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.5"},
            {"1234.55000000000001",  1, "1234.5"},
            {"1234.59",              1, "1234.5"},
            {"1234.59999999999999",  1, "1234.5"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.6"},
            {"-1234.55",             1, "-1234.6"},
            {"-1234.54999999999999", 1, "-1234.6"},
            {"-1234.55000000000000", 1, "-1234.6"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "0"},
            {"0.51",                 0, "0"},
            {"-0.49",                0, "-1"},
            {"-0.5",                 0, "-1"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::UP,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.5"},
            {"1234.45",              1, "1234.5"},
            {"1234.44999999999999",  1, "1234.5"},
            {"1234.45000000000000",  1, "1234.5"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.4"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.4"},
            {"-1234.45000000000001", 1, "-1234.4"},
            {"-1234.49999999999999", 1, "-1234.4"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.6"},
            {"1234.55",              1, "1234.6"},
            {"1234.54999999999999",  1, "1234.6"},
            {"1234.55000000000000",  1, "1234.6"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.5"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.5"},
            {"-1234.55000000000001", 1, "-1234.5"},
            {"-1234.59999999999999", 1, "-1234.5"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "1"},
            {"0.50",                 0, "1"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "0"},
            {"-0.51",                0, "0"}
        }
    ),
    createTest (
        Rounding::Mode::TOWARDS_ZERO,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.4"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.4"},
            {"1234.45000000000001",  1, "1234.4"},
            {"1234.49",              1, "1234.4"},
            {"1234.49999999999999",  1, "1234.4"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.4"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.4"},
            {"-1234.45000000000001", 1, "-1234.4"},
            {"-1234.49999999999999", 1, "-1234.4"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.5"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.5"},
            {"1234.55000000000001",  1, "1234.5"},
            {"1234.59",              1, "1234.5"},
            {"1234.59999999999999",  1, "1234.5"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.5"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.5"},
            {"-1234.55000000000001", 1, "-1234.5"},
            {"-1234.59999999999999", 1, "-1234.5"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "0"},
            {"0.51",                 0, "0"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "0"},
            {"-0.51",                0, "0"}
        }
    ),
    createTest (
        Rounding::Mode::AWAY_FROM_ZERO,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.5"},
            {"1234.45",              1, "1234.5"},
            {"1234.44999999999999",  1, "1234.5"},
            {"1234.45000000000000",  1, "1234.5"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.5"},
            {"-1234.45",             1, "-1234.5"},
            {"-1234.44999999999999", 1, "-1234.5"},
            {"-1234.45000000000000", 1, "-1234.5"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.6"},
            {"1234.55",              1, "1234.6"},
            {"1234.54999999999999",  1, "1234.6"},
            {"1234.55000000000000",  1, "1234.6"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.6"},
            {"-1234.55",             1, "-1234.6"},
            {"-1234.54999999999999", 1, "-1234.6"},
            {"-1234.55000000000000", 1, "-1234.6"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "1"},
            {"0.50",                 0, "1"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "-1"},
            {"-0.5",                 0, "-1"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_UP,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.5"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.5"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.4"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.4"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.6"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.6"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.5"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.5"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "1"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "0"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_DOWN,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.4"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.4"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.5"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.5"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.5"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.5"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.6"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.6"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "0"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "-1"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_AWAY_FROM_ZERO,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.5"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.5"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.5"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.5"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.6"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.6"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.6"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.6"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "1"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "-1"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_TOWARDS_ZERO,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.4"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.4"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.4"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.4"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.5"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.5"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.5"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.5"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "0"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "0"},
            {"-0.51",                0, "-1"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_TO_EVEN,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.4"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.4"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.4"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.4"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.6"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.6"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.6"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.6"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "0"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "0"},
            {"-0.51",                0, "-1"},
            {"0.54",                 1, "0.5"},
            {"0.55",                 1, "0.6"},
            {"0.56",                 1, "0.6"},
            {"-0.54",                1, "-0.5"},
            {"-0.55",                1, "-0.6"},
            {"-0.56",                1, "-0.6"}
        }
    ),
    createTest (
        Rounding::Mode::TO_NEAREST_HALF_TO_ODD,
        {
            {"1234.40",              1, "1234.4"},
            {"1234.41",              1, "1234.4"},
            {"1234.45",              1, "1234.5"},
            {"1234.44999999999999",  1, "1234.4"},
            {"1234.45000000000000",  1, "1234.5"},
            {"1234.45000000000001",  1, "1234.5"},
            {"1234.49",              1, "1234.5"},
            {"1234.49999999999999",  1, "1234.5"},
            {"-1234.40",             1, "-1234.4"},
            {"-1234.41",             1, "-1234.4"},
            {"-1234.45",             1, "-1234.5"},
            {"-1234.44999999999999", 1, "-1234.4"},
            {"-1234.45000000000000", 1, "-1234.5"},
            {"-1234.45000000000001", 1, "-1234.5"},
            {"-1234.49999999999999", 1, "-1234.5"},
            {"1234.50",              1, "1234.5"},
            {"1234.51",              1, "1234.5"},
            {"1234.55",              1, "1234.5"},
            {"1234.54999999999999",  1, "1234.5"},
            {"1234.55000000000000",  1, "1234.5"},
            {"1234.55000000000001",  1, "1234.6"},
            {"1234.59",              1, "1234.6"},
            {"1234.59999999999999",  1, "1234.6"},
            {"-1234.50",             1, "-1234.5"},
            {"-1234.51",             1, "-1234.5"},
            {"-1234.55",             1, "-1234.5"},
            {"-1234.54999999999999", 1, "-1234.5"},
            {"-1234.55000000000000", 1, "-1234.5"},
            {"-1234.55000000000001", 1, "-1234.6"},
            {"-1234.59999999999999", 1, "-1234.6"},
            {"0.00",                 0, "0"},
            {"0.49",                 0, "0"},
            {"0.50",                 0, "1"},
            {"0.51",                 0, "1"},
            {"-0.49",                0, "0"},
            {"-0.5",                 0, "-1"},
            {"-0.51",                0, "-1"},
            {"0.54",                 1, "0.5"},
            {"0.55",                 1, "0.5"},
            {"0.56",                 1, "0.6"},
            {"-0.54",                1, "-0.5"},
            {"-0.55",                1, "-0.5"},
            {"-0.56",                1, "-0.6"}
        }
    ),

    //
    // Some specialty tests to ensure rounding won't cause the number to exceed
    // the MAX_INTEGER_VALUE.  In this case the Number will simply not round
    // up, as opposed to having setDecimalPlaces through an Overflow exception
    // for this one very special case.
    //
    createTest (
        Rounding::Mode::AWAY_FROM_ZERO,
        {
            //
            // This first batch of tests shows the expected behaviour with
            // proper rounding.
            //

            {"9223372036854775806.9",               0, "9223372036854775807"},
            {"9223372036854775806.99",              0, "9223372036854775807"},
            {"9223372036854775806.99",              1, "9223372036854775807.0"},
            {"9223372036854775806.99999999999999",  0, "9223372036854775807"},
            {"9223372036854775806.99999999999999",  1, "9223372036854775807.0"},
            {"9223372036854775806.99999999999999",  2, "9223372036854775807.00"},

            {
                "9223372036854775806.99999999999999",
                13,
                "9223372036854775807.0000000000000"
            },

            {"-9223372036854775806.9",  0, "-9223372036854775807"},
            {"-9223372036854775806.99", 0, "-9223372036854775807"},

            {
                "-9223372036854775806.99",
                1,
                "-9223372036854775807.0"
            },

            {
                "-9223372036854775806.99999999999999",
                0,
                "-9223372036854775807"
            },

            {
                "-9223372036854775806.99999999999999",
                1,
                "-9223372036854775807.0"
            },

            {
                "-9223372036854775806.99999999999999",
                2,
                "-9223372036854775807.00"
            },

            {
                "-9223372036854775806.99999999999999",
                13,
                "-9223372036854775807.0000000000000"
            },

            //
            // These tests all restrict the actual rounding process to ensure
            // no overflow can occur.
            //

            {"9223372036854775806.9",               0, "9223372036854775807"},
            {"9223372036854775806.99",              0, "9223372036854775807"},
            {"9223372036854775806.99",              1, "9223372036854775807.0"},
            {"9223372036854775806.99999999999999",  0, "9223372036854775807"},
            {"9223372036854775806.99999999999999",  1, "9223372036854775807.0"},
            {"9223372036854775806.99999999999999",  2, "9223372036854775807.00"},

            {
                "9223372036854775806.99999999999999",
                13,
                "9223372036854775807.0000000000000"
            },

            {"-9223372036854775806.9",  0, "-9223372036854775807"},
            {"-9223372036854775806.99", 0, "-9223372036854775807"},

            {
                "-9223372036854775806.99",
                1,
                "-9223372036854775807.0"
            },

            {
                "-9223372036854775806.99999999999999",
                0,
                "-9223372036854775807"
            },

            {
                "-9223372036854775806.99999999999999",
                1,
                "-9223372036854775807.0"
            },

            {
                "-9223372036854775806.99999999999999",
                2,
                "-9223372036854775807.00"
            },

            {
                "-9223372036854775806.99999999999999",
                13,
                "-9223372036854775807.0000000000000"
            },

            {
                "9223372036854775807.99999999999999",
                13,
                "9223372036854775807.9999999999999"
            },

            {
                "-9223372036854775807.99999999999999",
                13,
                "-9223372036854775807.9999999999999"
            }
        }
    )
};

} // namespace test
} // namespace fixed
