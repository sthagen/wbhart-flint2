/*
    Copyright (C) 2023 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz.h"
#include "gr.h"

TEST_FUNCTION_START(gr_fq_nmod, state)
{
    gr_ctx_t Fq;
    fmpz_t p;
    slong d;
    slong iter;
    int flags = GR_TEST_ALWAYS_ABLE;

    fmpz_init(p);

    for (iter = 0; iter < 30; iter++)
    {
        fmpz_randprime(p, state, 2 + n_randint(state, FLINT_BITS - 1), 0);
        d = 1 + n_randint(state, 5);
        gr_ctx_init_fq_nmod(Fq, p, d, "a");
        gr_test_ring(Fq, 100, flags);
        gr_ctx_clear(Fq);
    }

    fmpz_clear(p);

    TEST_FUNCTION_END(state);
}
