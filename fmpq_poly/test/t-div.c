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

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpq_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("div....");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing of q and a */
    for (i = 0; i < 2000; i++)
    {
        fmpq_poly_t a, b, q;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);
        fmpq_poly_randtest(a, state, n_randint(50, state), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 200);

        fmpq_poly_div(q, a, b);
        fmpq_poly_div(a, a, b);

        result = (fmpq_poly_equal(q, a));
        if (!result)
        {
            printf("FAIL:\n");
            printf("q = "), fmpq_poly_print(q), printf("\n\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Check aliasing of q and b */
    for (i = 0; i < 2000; i++)
    {
        fmpq_poly_t a, b, q;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);
        fmpq_poly_randtest(a, state, n_randint(50, state), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 200);

        fmpq_poly_div(q, a, b);
        fmpq_poly_div(b, a, b);

        result = (fmpq_poly_equal(q, b));
        if (!result)
        {
            printf("FAIL:\n");
            printf("q = "), fmpq_poly_print(q), printf("\n\n");
            printf("a = "), fmpq_poly_print(a), printf("\n\n");
            printf("b = "), fmpq_poly_print(b), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Compare with divrem */
    for (i = 0; i < 2000; i++)
    {
        fmpq_poly_t a, b, q, q2, r;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);
        fmpq_poly_init(q2);
        fmpq_poly_init(r);
        fmpq_poly_randtest(a, state, n_randint(50, state), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(50, state) + 1, 200);

        fmpq_poly_divrem(q, r, a, b);
        fmpq_poly_div(q2, a, b);

        result = fmpq_poly_equal(q, q2);
        if (!result)
        {
            printf("FAIL:\n");
            printf("a  = "), fmpq_poly_print(a), printf("\n\n");
            printf("b  = "), fmpq_poly_print(b), printf("\n\n");
            printf("q  = "), fmpq_poly_print(q), printf("\n\n");
            printf("r  = "), fmpq_poly_print(r), printf("\n\n");
            printf("q2 = "), fmpq_poly_print(q2), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
        fmpq_poly_clear(q2);
        fmpq_poly_clear(r);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
