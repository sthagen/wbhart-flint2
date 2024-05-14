/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <math.h>
#include "flint.h"
#include "ulong_extras.h"

ulong n_sqrt(ulong a)
{
    ulong is;

    is = (ulong) sqrt((double) a);

    is -= (is*is > a);
#if FLINT64
    if (is == UWORD(4294967296)) is--;
#endif
    return is;
}
