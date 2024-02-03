/*
    Copyright (C) 2017 Luca De Feo

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "fmpz.h"
#include "fmpz_mod_mat.h"

void fmpz_mod_mat_randtest(fmpz_mod_mat_t mat, flint_rand_t state, const fmpz_mod_ctx_t ctx)
{
    fmpz_mat_randtest(mat, state, fmpz_bits(ctx->n));
    _fmpz_mod_mat_reduce(mat, ctx);
}
