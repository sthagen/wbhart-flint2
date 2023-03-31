/*
    Copyright (C) 2017 Luca De Feo

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "ulong_extras.h"
#include "nmod_mat.h"
#include "nmod_poly.h"
#include "fmpz.h"
#include "fq_nmod.h"
#include "fq_nmod_poly.h"
#include "fq_nmod_poly_factor.h"
#include "fq_nmod_embed.h"

#ifdef T
#undef T
#endif
#ifdef B
#undef B
#endif

#define T fq_nmod
#define CAP_T FQ_NMOD
#define B nmod

#include "fq_embed_templates/composition_matrix.c"
#include "fq_embed_templates/embed.c"
#include "fq_embed_templates/matrices.c"

#undef B
#undef CAP_T
#undef T
