/**
 * @file oasis_divs.c
 * @brief Calculate and display LCM(1,2,3,...n)
 * @author N.Arai
 * @date 2025-12-29
 *
 * This program provides supplementary information when calculating the least common multiple. 
 * It is specialized for the LCM (1,2,3,...n) format.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define XPT_ON
#include "xpt.h"
int xpt_flg = 0;

#define VERSION "v1.6.1"

#define N_MAX (2000)
#define M_MAX (2000)
#define S_MIN (2)	// LCM(1, 2)=2
#define S_MAX (1429)	// LCM(1,2,3,...1429)=<2^2048
#define P_MAX (64)	// n^64

static int divs[N_MAX+1] = { 0 };

/**
 * @brief Execute Sieve of Eratostheness for prime generation
 */
#define do_sieve() {					\
	for (int m = 1; m <= M_MAX; m++) {		\
		for (int n = m; n <= N_MAX; n+= m) {	\
			divs[n]++; 			\
		}					\
	}						\
}


/**
 * @brief Calculate and display LCM(1,2,3,...n)
 *
 * Display the value and prime factors of the LCM (1,2,3,...n). 
 * Let n be a value from 2 to 1429.
 *
 * @return Result status
 * @retval 0 Success
 * @retval -1 Failure
 */
int make_lcm_list()
{
	int s_cnt;	// LCM(1,2,3,...s_cnt)
	int ret = 0;
	int n;		// LCM(1,2,3,...n)
	int x;		// prime x ^ pwr
	int pwr;	//           ^
	int i;		// not display "..."(< 7)

	mpz_t lcm;
	mpz_init(lcm);

	for (s_cnt = S_MIN; s_cnt <= S_MAX; s_cnt++) {
		/*--- calc part ---*/
		mpz_set_ui(lcm, 1);			//    lcm = 1;
		for (n = 1; n <= s_cnt; n++) {		//    find prime	
			if (divs[n] == 2) {		//      in 1,2,3,...s_cnt
				x = n;
				for (pwr = 1; pwr < P_MAX; pwr++) {
					x *= n;					// x * x *...
					if (x > s_cnt) {			//    over ?
						x /= n;				// previos value
						printf("%d^%d=%d\n", n, pwr, x);
						mpz_mul_ui(lcm, lcm, x);	// lcm = lcm * x
						break;
					}
				}
				if (pwr == P_MAX) {
					fprintf(stderr, "ERR: p overflow.\n");	// Impossible
					ret = -1;
				}
			}
		}

		/*--- disp part ---*/
		if (ret == 0) {			// ok?
			if (s_cnt > 6) {
				gmp_printf("lcm(1,2,3,...%d)=%Zd\n\n", s_cnt, lcm);
			}
			else {
				printf("lcm(");
				for (i = 1; i <= s_cnt; i++) {
					printf("%d", i);
					if (i < s_cnt) printf(",");
				}
				gmp_printf(")=%Zd\n\n", lcm);
			}
		}
	}

	mpz_clear(lcm);

	return ret;
}

/**
 * @brief Main entry point
 */
int main()
{
	int ret;

	XPT_INIT();
	XPT_VER(VERSION);

	do_sieve();
	ret = make_lcm_list();

	return ret;
}
