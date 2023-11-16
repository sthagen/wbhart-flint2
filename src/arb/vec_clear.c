/*
    Copyright (C) 2014 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "arb.h"

void
_arb_vec_clear(arb_ptr v, slong n)
{
    slong i;
    for (i = 0; i < n; i++)
        arb_clear(v + i);
    flint_free(v);
}
