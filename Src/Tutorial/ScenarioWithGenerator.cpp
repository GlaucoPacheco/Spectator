// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include <Spectator>
#include <QString>

using namespace Qt::StringLiterals;

SCENARIO("Scenario with generator")
{
    // the whole scenario will be repeated twice
    const auto scenarioData = GENERATE(AS(QString), u"Hello"_s, u"World!"_s);

    GIVEN("a given with generate range")
    {
        // this given will be repeated three times with givenData={1,2,3}
        const auto givenData = GENERATE_RANGE(AS(int), 1, 3);
        REQUIRE(givenData == 1 || givenData == 2 || givenData == 3)

        WHEN("when with generate range with step")
        {
            // this when will be repeated four times with whenData={1,3,5,7}
            const auto whenData = GENERATE_RANGE_WITH_STEP(AS(int64_t), 1, 7, 2);
            REQUIRE(whenData == 1 || whenData == 3 || whenData == 5 || whenData == 7);
        }
    }
    // Let's generate some data at the scenario end just to test if Spectator
    // will correctly repeat the whole scenario even without entering any section.
    const auto scenarioEndData = GENERATE(AS(QString), u"a"_s, u"b"_s);
}
