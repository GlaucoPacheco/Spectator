// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include <Spectator>

SCENARIO("An empty scenario")
{
    GIVEN("a empty given")
    {
        WHEN("a empty when")
        {
            THEN("a empty then")
            {
            }

            THEN("another empty then")
            {
            }
        }

        WHEN("another empty when")
        {
            THEN("a empty then")
            {
            }

            THEN("another empty when")
            {
                AND_WHEN("a empty and_when")
                {
                    THEN("a empty then")
                    {
                        AND_THEN("a empty and_then")
                        {
                        }
                    }
                }
            }
        }
    }
}
