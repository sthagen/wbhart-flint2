/*
    Copyright (C) 2024 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "gr.h"
#include "gr_mat.h"
#include "nfloat.h"

int
nfloat_complex_mat_mul_block1(gr_mat_t C, const gr_mat_t A, const gr_mat_t B, gr_ctx_t ctx)
{
    return nfloat_complex_mat_mul_block(C, A, B, 2, ctx);
}

int
nfloat_complex_mat_mul_fixed1(gr_mat_t C, const gr_mat_t A, const gr_mat_t B, gr_ctx_t ctx)
{
    return nfloat_complex_mat_mul_fixed(C, A, B, 1000, ctx);
}

TEST_FUNCTION_START(complex_mat_mul, state)
{
    gr_ctx_t ctx;
    slong prec;
    slong iter;
    gr_ptr tol;

    for (iter = 0; iter < 10 * flint_test_multiplier(); iter++)
    {
        if (n_randint(state, 5))
            prec = FLINT_BITS * (1 + n_randint(state, 4));
        else
            prec = FLINT_BITS * (1 + n_randint(state, NFLOAT_MAX_LIMBS));

        nfloat_complex_ctx_init(ctx, prec, 0);

        tol = gr_heap_init(ctx);
        GR_MUST_SUCCEED(gr_one(tol, ctx));
        GR_MUST_SUCCEED(gr_mul_2exp_si(tol, tol, -prec + 2, ctx));

        gr_mat_test_approx_mul_max_norm(
            (gr_method_mat_binary_op) nfloat_complex_mat_mul_reorder,
            tol, state, 10, 4, ctx);

        gr_mat_test_approx_mul_max_norm(
            (gr_method_mat_binary_op) nfloat_complex_mat_mul_fixed1,
            tol, state, (prec <= 256) ? 100 : 1, 10, ctx);

        gr_mat_test_approx_mul_max_norm(
            (gr_method_mat_binary_op) nfloat_complex_mat_mul_block1,
            tol, state, (prec <= 256) ? 10 : 1,
                        (prec <= 256) ? 40 : 20, ctx);

        gr_mat_test_approx_mul_max_norm(
            (gr_method_mat_binary_op) nfloat_complex_mat_mul_fixed1,
            tol, state, (prec <= 256) ? 10 : 1,
                        (prec <= 256) ? 40 : 20, ctx);

        gr_mat_test_approx_mul_max_norm(
            (gr_method_mat_binary_op) nfloat_complex_mat_mul,
            tol, state, 1, 120, ctx);

        gr_heap_clear(tol, ctx);
        gr_ctx_clear(ctx);
    }

    TEST_FUNCTION_END(state);
}
