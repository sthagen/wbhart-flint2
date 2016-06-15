/*
    Copyright (C) 2015 Jonathan Bober
    Copyright (C) 2016 Fredrik Johansson
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef ACB_DIRICHLET_H
#define ACB_DIRICHLET_H

#ifdef ACB_DIRICHLET_INLINES_C
#define ACB_DIRICHLET_INLINE
#else
#define ACB_DIRICHLET_INLINE static __inline__
#endif

#include "acb.h"
#include "dlog.h"

#ifdef __cplusplus
extern "C" {
#endif

void _acb_dirichlet_euler_product_real_ui(arb_t res, ulong s,
    const signed char * chi, int mod, int reciprocal, slong prec);

void acb_dirichlet_eta(acb_t res, const acb_t s, slong prec);

typedef struct
{
    ulong q;                /* modulus */
    nmod_t mod;             /* modulus with precomputed inverse */
    ulong q_even;           /* even part of modulus */
    ulong q_odd;            /* odd part of modulus */
    ulong phi_q;            /* phi(q) = group size */
    ulong expo;             /* group exponent = lcm(phi(q_even), phi(p[k]^e[k]) ) */
    slong neven;            /* number of even components (in 0,1,2)*/
    slong num;              /* number of prime components (even + odd) */
    ulong * primes;         /* primes p[k] */
    ulong * exponents;      /* exponents e[k] */
    ulong * primepowers;    /* powers p[k]^[k] */
    ulong * generators;     /* generator for each prime p[k] lifted mod q */
    ulong * phi;            /* phi(k) = phi(p[k]^e[k])       */
    ulong * PHI;            /* PHI(k) = expo / phi(k)        */
    dlog_precomp_t * dlog;  /* precomputed data for discrete log mod p^e */
}
acb_dirichlet_group_struct;

typedef acb_dirichlet_group_struct acb_dirichlet_group_t[1];

ACB_DIRICHLET_INLINE ulong
acb_dirichlet_group_size(const acb_dirichlet_group_t G)
{
    return G->phi_q;
}

void acb_dirichlet_group_init(acb_dirichlet_group_t G, ulong q);
void acb_dirichlet_group_clear(acb_dirichlet_group_t G);
void acb_dirichlet_group_dlog_precompute(acb_dirichlet_group_t G, ulong num);

/* properties of elements without log */

ulong acb_dirichlet_number_primitive(const acb_dirichlet_group_t G);
ulong acb_dirichlet_ui_conductor(const acb_dirichlet_group_t G, ulong a);
int acb_dirichlet_ui_parity(const acb_dirichlet_group_t G, ulong a);
ulong acb_dirichlet_ui_order(const acb_dirichlet_group_t G, ulong a);

/* elements of the group, keep both number and log */
typedef struct
{
  ulong n;           /* number */
  ulong * log;       /* s.t. prod generators[k]^log[k] = number */
}
acb_dirichlet_conrey_struct;

typedef acb_dirichlet_conrey_struct acb_dirichlet_conrey_t[1];

void acb_dirichlet_conrey_init(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G);
void acb_dirichlet_conrey_clear(acb_dirichlet_conrey_t x);
void acb_dirichlet_conrey_print(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t x);

int acb_dirichlet_conrey_parity(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t x);
ulong acb_dirichlet_conrey_conductor(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t x);
void acb_dirichlet_conrey_log(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G, ulong m);

void acb_dirichlet_conrey_one(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G);
void acb_dirichlet_conrey_first_primitive(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G);

int acb_dirichlet_conrey_next(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G);
int acb_dirichlet_conrey_next_primitive(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G);

void acb_dirichlet_conrey_mul(acb_dirichlet_conrey_t c, const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t a, const acb_dirichlet_conrey_t b);
void acb_dirichlet_conrey_pow(acb_dirichlet_conrey_t c, const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t a, ulong n);

#define ACB_DIRICHLET_CHI_NULL UWORD_MAX

ulong acb_dirichlet_ui_pairing_conrey(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t a, const acb_dirichlet_conrey_t b);
ulong acb_dirichlet_ui_pairing(const acb_dirichlet_group_t G, ulong m, ulong n);

void acb_dirichlet_pairing_conrey(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t a, const acb_dirichlet_conrey_t b, slong prec);
void acb_dirichlet_pairing(acb_t res, const acb_dirichlet_group_t G, ulong m, ulong n, slong prec);

/* introducing character type */

/* character = reduced exponents, keep order, number and conductor */
typedef struct
{
    ulong q;           /* modulus */
    /*? acb_dirichet_conrey_struct n; */
    ulong n;           /* number */
    ulong order;       /* order */
    ulong * expo;      /* reduced exponents ( order * log[k] / gcd( ) ) */
    int parity;        /* 0 for even char, 1 for odd */
    ulong conductor;
}
acb_dirichlet_char_struct;

typedef acb_dirichlet_char_struct acb_dirichlet_char_t[1];

ACB_DIRICHLET_INLINE ulong
acb_dirichlet_char_order(const acb_dirichlet_char_t chi)
{
    return chi->order;
}

ACB_DIRICHLET_INLINE int
acb_dirichlet_char_parity(const acb_dirichlet_char_t chi)
{
    return chi->parity;
}

void acb_dirichlet_char_init(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G);
void acb_dirichlet_char_clear(acb_dirichlet_char_t chi);
void acb_dirichlet_char_print(const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi);

void acb_dirichlet_char(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G, ulong n);
void acb_dirichlet_char_conrey(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t x);
void acb_dirichlet_char_normalize(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G);
void acb_dirichlet_char_denormalize(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G);

ulong acb_dirichlet_char_conductor(const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi);

void acb_dirichlet_char_one(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G);
void acb_dirichlet_char_first_primitive(acb_dirichlet_char_t chi, const acb_dirichlet_group_t G);

ulong acb_dirichlet_ui_chi_conrey(const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, const acb_dirichlet_conrey_t x);
ulong acb_dirichlet_ui_chi(const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, ulong n);

void acb_dirichlet_ui_vec_set_null(ulong *v, const acb_dirichlet_group_t G, slong nv);
void acb_dirichlet_ui_chi_vec_loop(ulong *v, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong nv);
void acb_dirichlet_ui_chi_vec_primeloop(ulong *v, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong nv);
void acb_dirichlet_ui_chi_vec_sieve(ulong *v, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong nv);
void acb_dirichlet_ui_chi_vec(ulong *v, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong nv);

/* precompute powers of a root of unity */
typedef struct
{
    ulong order;
    acb_ptr z;
    ulong m;
    ulong M;
    acb_ptr Z;
}
acb_dirichlet_powers_struct;

typedef acb_dirichlet_powers_struct acb_dirichlet_powers_t[1];

void acb_dirichlet_powers_init(acb_dirichlet_powers_t t, ulong order, slong num, slong prec);
void acb_dirichlet_powers_clear(acb_dirichlet_powers_t t);
void acb_dirichlet_power(acb_t z, const acb_dirichlet_powers_t t, ulong n, slong prec);

void acb_dirichlet_nth_root(acb_t res, ulong order, slong prec);

void acb_dirichlet_chi(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, ulong n, slong prec);
void acb_dirichlet_chi_vec(acb_ptr v, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong nv, slong prec);

void acb_dirichlet_arb_quadratic_powers(arb_ptr v, slong nv, const arb_t x, slong prec);

ulong acb_dirichlet_theta_length_d(ulong q, double x, slong prec);
ulong acb_dirichlet_theta_length(ulong q, const arb_t x, slong prec);
void acb_dirichlet_arb_theta_naive(acb_t res, const arb_t x, int parity, const ulong *a, const acb_dirichlet_powers_t z, slong len, slong prec);
void acb_dirichlet_arb_theta_smallorder(acb_t res, const arb_t x, int parity, const ulong *a, const acb_dirichlet_powers_t z, slong len, slong prec);
void acb_dirichlet_chi_theta_arb(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, const arb_t x, slong prec);

void acb_dirichlet_gauss_sum_naive(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong prec);
void acb_dirichlet_gauss_sum_theta(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong prec);
void acb_dirichlet_gauss_sum(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, slong prec);

#ifdef __cplusplus
}
#endif

#endif
