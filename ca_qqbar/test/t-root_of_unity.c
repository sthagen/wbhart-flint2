/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of Calcium.

    Calcium is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "ca_qqbar.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("root_of_unity....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        ca_qqbar_t x, y;
        slong p, p2;
        ulong q, q2;
        int ok;

        ca_qqbar_init(x);
        ca_qqbar_init(y);

        q = 1 + n_randint(state, 30);
        p = n_randint(state, 1000);
        p -= 500;

        ca_qqbar_root_of_unity(x, p, q);
        ok = ca_qqbar_is_root_of_unity(&p2, &q2, x);
        ca_qqbar_root_of_unity(y, p2, q2);

        if (!ok || !ca_qqbar_equal(x, y) || n_gcd(FLINT_ABS(p2), q2) != 1 || !(0 <= p2 && p2 < (slong) 2 * q2))
        {
            flint_printf("FAIL!\n");
            flint_printf("x = "); ca_qqbar_print(x); flint_printf("\n\n");
            flint_printf("y = "); ca_qqbar_print(y); flint_printf("\n\n");
            flint_printf("p, p2 = %wd %wd\n\n", p, p2);
            flint_printf("q, q2 = %wu %wu\n\n", q, q2);
            flint_abort();
        }

        if (q <= 10)
        {
            ca_qqbar_pow_ui(y, x, q);

            if (!ca_qqbar_is_one(y))
            {
                flint_printf("FAIL!\n");
                flint_printf("x = "); ca_qqbar_print(x); flint_printf("\n\n");
                flint_printf("y = "); ca_qqbar_print(y); flint_printf("\n\n");
                flint_printf("p, p2 = %wd %wd\n\n", p, p2);
                flint_printf("q, q2 = %wu %wu\n\n", q, q2);
                flint_abort();
            }
        }

        ca_qqbar_clear(x);
        ca_qqbar_clear(y);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

