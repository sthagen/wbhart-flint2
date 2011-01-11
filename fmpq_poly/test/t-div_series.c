/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "fmpq_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("div_series....");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing q and a */
    for (i = 0; i < 1000; i++)
    {
        fmpq_poly_t a, b, q;
        long n = n_randint(50, state) + 1;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);

        fmpq_poly_randtest(a, state, n_randint(50, state) + 1, 80);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 80);
        fmpq_poly_set_coeff_ui(b, 0, 1);

        fmpq_poly_div_series(q, a, b, n);
        fmpq_poly_div_series(a, a, b, n);

        result = (fmpq_poly_equal(q, a));
        if (!result)
        {
            printf("FAIL (alias q and a):\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            printf("q = "), fmpq_poly_print(q), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Check aliasing q and b */
    for (i = 0; i < 1000; i++)
    {
        fmpq_poly_t a, b, q;
        long n = n_randint(50, state) + 1;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);

        fmpq_poly_randtest(a, state, n_randint(50, state) + 1, 80);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 80);
        fmpq_poly_set_coeff_ui(b, 0, 1);

        fmpq_poly_div_series(q, a, b, n);
        fmpq_poly_div_series(b, a, b, n);

        result = (fmpq_poly_equal(q, b));
        if (!result)
        {
            printf("FAIL (alias q and b):\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            printf("q = "), fmpq_poly_print(q), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Check that Q * B == A */
    for (i = 0; i < 1000; i++)
    {
        fmpq_poly_t a, b, p, q;
        long n = n_randint(50, state) + 1;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(p);
        fmpq_poly_init(q);

        fmpq_poly_randtest(a, state, n_randint(50, state) + 1, 80);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 80);
        fmpq_poly_set_coeff_ui(b, 0, 1);

        fmpq_poly_div_series(q, a, b, n);
        fmpq_poly_mullow(p, q, b, n);

        fmpq_poly_truncate(a, n);

        result = (fmpq_poly_equal(p, a));
        if (!result)
        {
            printf("FAIL (check Q * B = A):\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            printf("p = "), fmpq_poly_print(p), printf("\n\n");
            printf("q = "), fmpq_poly_print(q), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(p);
        fmpq_poly_clear(q);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
