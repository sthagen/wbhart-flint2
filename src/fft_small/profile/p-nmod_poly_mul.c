#include "nmod.h"
#include "nmod_poly.h"
#include "fft_small.h"
#include "profiler.h"

/*

speed up over _nmod_poly_mul:

    ---> log2(degree of product)

|
| nbits(mod.n) with ** denoting a special fft prime
v

1 thread:

     9.0  9.6 10.1 10.7 11.3 11.9 12.5 13.1 13.6 14.2 14.8 15.4 16.0 16.6 17.2 17.7 18.3 18.9 19.5 20.1 20.7 21.3 21.8 22.4 23.0
**:  4.8  5.3  5.3  6.7  7.7  8.8  9.4  9.3 11.2 10.1 14.1 13.8 13.6 13.9 14.7 15.7 14.1 14.2 14.7 14.6 16.0 14.1 16.9 14.1 11.9
 2:  0.5  0.5  0.5  0.6  0.7  0.8  0.9  1.0  1.2  1.4  1.5  1.6  1.8  2.1  2.2  2.5  2.2  2.4  2.4  2.2  2.5  2.1  3.2  3.3  2.8
 4:  0.6  0.6  0.6  0.7  0.9  1.1  1.1  1.1  1.5  1.6  1.9  2.0  2.4  2.5  2.2  2.9  2.8  3.1  2.9  2.3  2.7  3.1  3.2  3.4  3.9
 6:  0.7  0.8  0.7  0.9  1.0  1.2  1.3  1.3  1.7  1.9  2.3  2.4  2.9  3.3  2.7  3.0  3.2  3.1  2.9  3.0  3.3  3.5  4.6  4.3  4.3
 8:  0.8  0.8  0.9  1.1  1.3  1.5  1.4  1.4  1.9  2.2  2.6  2.8  3.5  3.9  3.0  3.5  3.2  3.9  3.6  3.4  3.3  3.5  5.2  4.3  5.0
10:  0.9  1.0  1.0  1.3  1.4  1.7  1.6  1.8  2.2  2.7  3.0  3.4  3.4  3.5  3.7  4.0  3.9  3.9  3.7  3.5  4.4  4.6  5.0  4.4  5.0
12:  1.0  1.2  1.2  1.4  1.6  2.0  1.8  2.1  2.5  2.7  3.4  3.4  4.4  3.7  3.7  4.4  4.1  5.0  4.3  3.6  4.5  4.6  4.9  6.0  6.2
14:  1.2  1.3  1.3  1.6  1.7  2.1  2.1  2.3  2.6  3.4  3.8  4.3  4.8  4.3  4.1  5.0  4.3  4.7  4.8  4.5  4.9  4.4  5.4  6.2  6.4
16:  1.3  1.4  1.4  1.7  1.9  2.2  2.4  2.5  3.1  3.7  4.3  4.7  5.9  4.8  4.3  5.0  5.2  6.0  2.3  2.1  2.6  2.2  2.5  3.3  2.6
18:  1.4  1.7  1.6  2.0  2.2  2.6  2.5  2.7  3.6  4.0  4.5  2.3  2.4  2.3  2.1  2.4  2.6  2.7  2.6  2.0  2.6  2.9  3.1  3.6  3.2
20:  1.5  1.8  1.7  2.1  2.4  1.2  1.2  1.4  1.6  2.1  2.3  2.5  2.6  2.5  2.5  2.7  2.4  2.6  2.6  2.9  3.3  3.3  3.1  3.5  3.7
22:  0.7  0.8  0.8  1.0  1.1  1.2  1.4  1.5  1.8  2.3  2.2  2.8  2.6  2.6  2.5  2.9  3.1  2.6  2.6  2.9  3.1  3.4  3.3  4.0  3.9
24:  0.8  0.8  0.9  1.1  1.2  1.4  1.5  1.6  1.9  2.3  2.7  2.6  3.0  3.0  2.9  3.0  2.9  3.5  3.4  3.2  3.5  3.6  4.4  4.5  4.4
26:  0.8  0.9  0.9  1.2  1.3  1.4  1.5  1.7  2.2  2.6  3.1  2.9  3.2  3.1  2.9  3.1  3.0  3.5  3.4  3.2  3.3  3.6  4.5  4.4  4.5
28:  0.9  1.1  1.1  1.3  1.5  1.7  1.9  2.0  2.5  2.7  3.3  3.5  3.4  3.2  3.3  3.8  3.7  3.9  3.4  3.7  3.5  4.4  4.9  4.8  5.0
30:  1.0  1.2  1.1  1.5  1.6  1.9  2.1  2.3  2.6  3.1  3.5  3.5  3.5  3.5  3.3  4.0  3.7  3.9  3.4  3.8  4.6  4.5  5.2  4.9  5.3
32:  1.1  1.2  1.2  1.5  1.7  2.1  2.0  2.1  2.6  2.8  3.3  3.5  3.9  3.4  3.4  4.3  3.9  4.3  3.4  3.7  4.6  4.5  4.9  5.0  5.1
34:  1.1  1.3  1.3  1.6  1.8  2.2  2.3  2.2  2.9  3.3  4.0  3.7  4.1  4.1  3.9  4.1  3.8  4.3  3.9  3.7  4.6  4.6  5.7  4.6  5.2
36:  1.2  1.4  1.4  1.8  1.8  2.2  2.5  2.5  3.1  3.6  4.2  3.8  4.2  4.2  4.0  4.1  3.8  4.3  5.0  4.6  5.3  4.6  5.6  5.9  6.0
38:  1.3  1.4  1.4  1.8  1.9  2.5  2.6  2.4  3.4  3.7  4.4  4.0  4.2  4.3  4.3  5.1  3.9  4.3  4.8  4.6  5.5  4.5  6.1  6.0  6.0
40:  1.3  1.5  1.6  1.9  2.2  2.5  2.6  2.8  3.5  3.8  4.5  4.3  4.8  4.6  4.3  4.8  4.5  5.1  4.9  4.8  3.5  2.9  4.0  4.0  3.6
42:  1.3  1.6  1.6  2.0  2.4  2.4  2.6  3.2  3.9  4.3  5.1  4.6  4.7  2.9  2.7  3.0  3.1  3.5  3.2  3.2  3.6  4.1  4.6  4.5  4.2
44:  1.4  1.6  1.7  2.1  2.4  2.6  1.9  2.0  2.3  2.5  3.2  3.0  3.1  3.0  2.7  3.0  3.2  3.5  3.3  3.0  3.5  4.0  4.7  4.6  4.5
46:  1.0  1.1  1.1  1.4  1.6  1.9  2.1  1.9  2.4  2.6  2.9  3.1  3.1  3.3  2.8  3.2  3.4  3.4  4.0  3.0  3.5  4.0  4.5  4.5  4.5
48:  1.1  1.2  1.2  1.4  1.6  2.0  2.1  2.0  2.5  3.1  3.1  3.0  3.4  3.3  3.6  3.8  3.6  3.4  3.8  3.0  3.7  4.5  4.9  5.1  4.4
50:  1.1  1.2  1.2  1.5  1.8  2.2  2.0  2.1  2.8  3.1  3.3  3.3  3.3  3.4  3.6  3.8  3.4  3.5  3.9  4.2  4.4  4.5  4.8  5.0  4.4
52:  1.1  1.2  1.2  1.5  1.8  2.0  2.3  2.5  2.7  3.6  3.3  3.4  3.3  3.3  3.5  3.8  3.5  3.4  3.7  4.5  4.5  4.6  4.9  5.0  5.0
54:  1.1  1.3  1.3  1.6  1.8  2.0  2.1  2.5  3.0  3.5  3.5  3.5  3.5  3.6  3.6  3.8  3.8  3.4  4.0  4.6  4.5  4.5  5.0  5.2  5.1
56:  1.2  1.3  1.4  1.6  2.0  2.1  2.2  2.6  2.8  3.6  3.5  3.6  3.9  3.9  3.6  3.9  3.8  4.1  3.8  4.2  4.7  5.4  5.1  5.5  5.5
58:  1.2  1.3  1.3  1.8  1.9  2.2  2.5  2.6  3.2  4.0  3.6  3.7  4.0  4.0  3.8  3.9  3.9  5.1  5.2  4.9  4.7  5.4  4.8  5.3  5.5
60:  1.1  1.4  1.4  1.7  2.0  2.4  2.9  3.0  3.2  3.8  3.9  3.8  4.2  4.2  3.7  3.9  3.9  4.9  4.9  4.9  4.6  5.3  6.0  6.2  5.9
62:  1.3  1.5  1.6  2.0  2.3  2.6  2.8  2.9  3.7  4.0  3.9  4.1  4.2  4.6  4.5  3.9  3.9  5.0  4.7  5.0  4.7  5.3  6.1  6.1  5.9
64:  1.4  1.6  1.6  2.0  2.2  2.8  2.9  3.0  3.5  3.7  4.1  4.3  4.5  4.4  4.6  4.0  4.0  4.9  5.1  5.1  4.7  5.8  6.3  6.1  6.2

8 threads:

     9.0  9.6 10.1 10.7 11.3 11.9 12.5 13.1 13.6 14.2 14.8 15.4 16.0 16.6 17.2 17.7 18.3 18.9 19.5 20.1 20.7 21.3 21.8 22.4 23.0
**:  4.9  5.3  5.3  6.7  7.6  8.7  9.5  9.3  8.5  8.6 14.9 14.3 16.1 18.3 19.3 21.7 20.0 20.2 19.8 19.7 21.1 19.1 22.9 20.3 16.6
 2:  0.5  0.5  0.5  0.6  0.7  0.8  0.9  1.0  1.0  1.6  1.7  1.8  2.4  2.9  3.3  3.9  3.5  3.7  3.5  3.0  3.6  2.9  4.6  4.7  4.0
 4:  0.6  0.6  0.6  0.7  0.9  1.1  1.2  1.1  1.3  1.8  2.2  2.4  3.1  3.5  3.3  4.3  4.2  4.7  4.2  3.2  3.7  4.4  4.6  5.0  5.5
 6:  0.7  0.8  0.7  0.9  1.0  1.2  1.3  1.3  1.5  1.9  2.6  3.0  3.6  4.4  4.0  4.7  4.7  4.8  4.2  4.2  4.7  5.0  6.5  6.3  6.3
 8:  0.8  0.8  0.9  1.1  1.3  1.5  1.5  1.5  1.6  1.8  3.0  3.3  4.4  5.3  4.4  5.1  4.9  5.8  5.2  4.8  4.5  4.9  7.4  6.1  7.3
10:  0.9  1.1  1.0  1.3  1.4  1.6  1.6  1.8  1.8  2.7  3.4  4.0  4.3  4.7  5.4  6.2  5.7  6.1  5.4  4.9  6.2  6.7  7.0  6.2  7.2
12:  1.0  1.2  1.2  1.4  1.6  2.0  1.8  2.1  2.1  2.8  3.8  3.7  5.6  5.2  5.3  6.7  6.3  7.6  6.2  5.0  6.5  6.5  7.0  8.4  8.6
14:  1.2  1.3  1.3  1.6  1.8  2.1  2.1  2.2  2.3  3.4  4.1  4.9  6.1  5.9  6.1  7.6  6.5  7.3  6.9  6.3  6.9  6.2  7.7  8.7  9.0
16:  1.3  1.4  1.4  1.7  1.9  2.3  2.5  2.5  2.7  4.3  4.7  5.3  7.5  6.7  6.2  7.5  7.8  9.3  4.5  3.8  5.1  4.2  4.9  6.2  4.9
18:  1.4  1.6  1.6  2.0  2.2  2.6  2.5  2.6  3.0  4.4  5.1  4.4  4.6  4.7  4.3  5.2  5.2  5.2  5.0  3.9  5.0  5.5  6.2  7.1  6.1
20:  1.5  1.8  1.8  2.1  2.3  1.9  1.8  1.8  2.8  3.5  4.0  4.8  5.1  5.2  5.1  5.8  4.8  5.2  5.0  5.4  6.4  6.2  6.2  6.8  7.2
22:  0.5  0.7  0.9  1.3  1.6  1.9  2.1  2.0  3.0  3.6  3.8  5.0  5.1  5.3  5.2  5.9  6.0  5.2  5.0  5.5  6.2  6.5  6.5  7.6  7.4
24:  0.5  0.8  1.1  1.4  1.7  2.2  2.2  2.2  3.2  3.7  4.6  4.7  5.9  6.1  5.9  6.2  6.0  6.7  6.6  5.8  6.8  6.8  8.6  8.8  8.4
26:  0.5  0.8  1.1  1.5  1.8  2.3  2.2  2.2  3.9  4.3  5.1  5.4  6.3  6.4  6.0  6.3  5.9  7.0  6.6  5.8  6.4  6.8  8.8  8.4  8.5
28:  0.5  1.0  1.3  1.7  2.1  2.6  3.0  2.5  4.3  4.5  5.6  6.5  6.5  6.3  6.8  7.9  7.3  7.5  6.5  7.0  6.7  8.2  9.6  9.3  9.4
30:  0.6  1.0  1.3  1.9  2.3  3.1  3.1  3.0  4.5  4.9  5.9  6.3  6.8  7.0  6.9  8.3  7.3  7.6  6.7  7.1  8.9  8.5 10.2  9.5 10.0
32:  0.7  1.1  1.5  2.0  2.4  3.3  3.1  2.7  4.6  4.7  5.8  6.4  7.8  7.1  7.2  9.0  7.6  8.3  6.7  6.8  8.8  8.6  9.7  9.6  9.7
34:  0.7  1.2  1.5  2.1  2.6  3.5  3.5  2.8  5.2  5.3  6.9  6.7  8.0  8.3  8.1  8.5  7.6  8.3  7.6  6.7  8.8  8.6 11.1  9.0  9.9
36:  0.8  1.2  1.6  2.3  2.6  3.5  3.8  3.2  5.3  5.7  7.5  6.6  8.5  8.6  8.2  8.6  7.7  8.4  9.5  8.4 10.1  8.6 11.1 11.4 11.4
38:  0.8  1.3  1.7  2.4  2.8  4.0  3.7  3.1  6.2  5.8  7.5  7.6  8.2  8.5  8.7 10.3  7.9  8.3  9.2  8.7 10.6  8.5 12.2 11.7 11.5
40:  0.8  1.4  1.8  2.5  3.1  4.0  4.0  3.6  5.9  6.2  7.7  8.0  9.3  9.3  8.7  9.9  9.0 10.1  9.5  8.8  8.5  6.7  9.8  9.1  8.4
42:  0.9  1.5  1.9  2.6  3.4  3.9  4.0  4.1  6.6  7.1  9.1  8.2  9.4  8.7  7.8  8.4  7.9  9.0  7.7  7.2  8.6  9.4 11.1 10.5  9.9
44:  0.9  1.5  2.0  2.8  3.5  4.2  4.5  3.9  5.4  6.2  8.6  8.5  9.0  9.1  7.7  8.6  8.1  8.8  8.0  6.7  8.5  9.3 11.5 10.6 10.5
46:  1.0  1.3  1.7  2.6  3.6  4.1  4.7  3.9  6.0  6.5  7.6  8.5  9.0  9.9  8.2  8.7  8.3  8.6  9.6  6.8  8.6  9.2 11.1 10.6 10.5
48:  1.0  1.4  1.8  2.7  3.4  4.3  4.7  3.9  6.1  7.9  8.2  8.7  9.9 10.1 10.2 10.2  9.0  8.6  9.1  6.8  9.0 10.4 12.0 12.0 10.2
50:  1.1  1.5  1.9  2.9  3.8  4.8  4.3  4.1  6.4  8.0  8.7  9.4  9.9 10.4 10.2 10.5  8.6  8.6  9.4  9.5 10.7 10.4 11.6 12.0 10.1
52:  1.1  1.4  2.0  3.0  3.9  4.4  5.1  5.1  5.9  9.2  8.7  9.5  9.7 10.3 10.2 10.4  8.8  8.6  9.1 10.3 11.0 10.5 12.0 11.9 11.8
54:  1.2  1.6  2.1  3.2  4.1  4.4  4.9  4.9  6.5  9.0  9.3 10.1 10.3 10.9 10.3 10.5  9.5  8.5  9.7 10.6 10.9 10.5 12.1 12.4 11.8
56:  1.2  1.6  2.3  3.2  4.4  4.5  5.0  5.2  6.8  9.2  9.5 10.1 11.4 12.3 10.4 10.8  9.6 10.1  9.1  9.5 11.3 12.4 12.4 12.8 12.5
58:  1.3  1.6  2.1  3.4  4.3  4.8  5.5  5.9  8.1 10.4  9.3 10.6 11.7 11.9 10.6 11.0  9.7 12.9 12.5 11.1 11.3 12.6 11.8 12.5 12.7
60:  1.2  1.7  2.3  3.4  4.4  5.4  6.1  6.1  7.4  9.6 10.4 10.7 12.3 12.7 10.6 11.1  9.9 12.3 11.6 11.2 11.1 12.1 14.7 14.5 13.7
62:  1.4  1.9  2.6  3.9  5.0  5.7  6.4  5.9  8.5 10.3 10.3 11.3 12.2 13.5 12.6 10.7  9.6 12.5 11.4 11.5 11.0 12.4 15.0 14.5 13.5
64:  1.4  1.8  2.5  4.0  4.9  6.0  6.8  6.1  8.0  9.5 10.6 12.0 12.9 12.8 13.0 11.3  9.6 12.6 12.3 11.4 11.5 13.4 15.3 14.4 14.3

*/

int main(void)
{
    flint_bitcnt_t nbits;
    mpn_ctx_t R;
    nmod_t mod;
    flint_rand_t state;
    timeit_t timer;
    double time1, time2;
    ulong * a, * b, * c, * d;
    ulong an, bn, zn, i, nreps;
    ulong zn_max = 10000000;

    flint_randinit(state);
    mpn_ctx_init(R, UWORD(0x0003f00000000001));

    //flint_set_num_threads(8);

    flint_printf("   ");
    for (zn = 500; zn < zn_max; zn += 1 + zn/2)
        flint_printf("%5.1f", log2(zn));
    flint_printf("\n");

    for (nbits = 0; nbits <= FLINT_BITS; nbits += 2)
    {
        if (nbits == 0)
        {
            nmod_init(&mod, R->ffts[1].mod.n);
            flint_printf("**:");
        }
        else
        {
            nmod_init(&mod, n_randbits(state, nbits));
            flint_printf("%2wu:", nbits);
        }

        a = FLINT_ARRAY_ALLOC(zn_max, ulong);
        b = FLINT_ARRAY_ALLOC(zn_max, ulong);
        c = FLINT_ARRAY_ALLOC(zn_max, ulong);
        d = FLINT_ARRAY_ALLOC(zn_max, ulong);

        for (zn = 500; zn < zn_max; zn += 1 + zn/2)
        {
            an = zn/2 + n_randint(state, 1 + zn/4);
            bn = 1 + zn - an;

            if (an < bn)
                ULONG_SWAP(an, bn);

            nreps = 1 + 100000000/(zn*n_nbits(zn));

            for (i = 0; i < an; i++)
                a[i] = n_randint(state, mod.n);

            for (i = 0; i < bn; i++)
                b[i] = n_randint(state, mod.n);

            timeit_start(timer);
            for (i = 0; i < nreps; i++)
                _nmod_poly_mul(c, a, an, b, bn, mod);
            timeit_stop(timer);
            time1 = timer->wall*1e6/(nreps*log2(zn)*zn);

            timeit_start(timer);
            for (i = 0; i < nreps; i++)
                _nmod_poly_mul_mid_mpn_ctx(d, 0, an+bn-1, a, an, b, bn, mod, R);
            timeit_stop(timer);
            time2 = timer->wall*1e6/(nreps*log2(zn)*zn);

            flint_printf("%5.1f", /*time2*/time1/time2);
            fflush(stdout);

            for (i = 0; i < zn; i++)
            {
                if (c[i] != d[i])
                {
                    flint_printf("error at index %wu\n", i);
                    flint_printf("mod: %wu\n", mod.n);
                    flint_printf("zn: %wu, an: %wu, bn: %wu\n", zn, an, bn);
                    flint_abort();
                }
            }
        }

        flint_printf("\n");

        flint_free(a);
        flint_free(b);
        flint_free(c);
        flint_free(d);
    }

    mpn_ctx_clear(R);
    flint_randclear(state);

    return 0;
}

