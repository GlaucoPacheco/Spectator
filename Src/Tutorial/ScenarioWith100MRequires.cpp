// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <Spectator>

SCENARIO("A scenario with 100M requires")
{
    // Let's do 50M requires outside any given/when/then.
    for (auto i = 0; i < 50000000; ++i)
    {
        REQUIRE(i < 50000000);
    }

    GIVEN("a empty given")
    {
        WHEN("a empty when")
        {
            THEN("we run require for 10M times")
            {
                // Let's do another 50M requires inside a section.
                for (auto i = 0; i < 50000000; ++i)
                {
                    REQUIRE(i < 50000000);
                }
            }
        }
    }
}
