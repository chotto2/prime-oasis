/**
 * @file oasis_layer1.c
 * @brief Find prime numbers from the desert.
 * @author N.Arai
 * @date 2025-12-22
 *
 * This program demonstrates finding prime numbers within prime deserts
 * using the innovative LCM method - a constructive (non-sieve) approach.
 *
 * @note Around 1024-bit version for educational purposes
 * @see Related paper on arXiv (to be published)
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define make_lcm(A, B) {		\
	mpz_set_ui(A, 1);		\
	for (int i = 2; i <= B; i++) {	\
		mpz_lcm_ui(A, A, i);	\
	}				\
}
	
/**
 * @brief Find prime numbers around LCM.
 *
 * @param[in] start Upper boundary of the prime gap (top lcm)
 * @param[in] end   Lower boundary of the prime gap (botom lcm)
 * @param[in] step  Search increment (smaller lcm)
 *
 * @note All mpz_t parameters must be initialized before calling
 * @details Search for primes in the form: pit +- 1, where pit = start + k*step
 */
void find_prime_oasis(mpz_t start, mpz_t end, mpz_t step)
{
	mpz_t pit;
	mpz_t p1;	// plus 1
	mpz_t m1;	// minus 1

	mpz_init(pit);
	mpz_init(p1);
	mpz_init(m1);

	for (mpz_set(pit, start);			// for (pit =  start
	     mpz_cmp(pit, end) < 0;			//      pit <  end
	     mpz_add(pit, pit, step)) {			//      pit += step) {

		mpz_add_ui(p1, pit, 1);			// pit+1 is
		if (mpz_probab_prime_p(p1, 25)) {	//  prime?
			gmp_printf("prime oasis = %Zd\n", p1);
		}
		mpz_sub_ui(m1, pit, 1);			// pit-1 is
		if (mpz_probab_prime_p(m1, 25)) {	//  prime?
			gmp_printf("prime oasis = %Zd\n", m1);
		}
	}

	mpz_clear(pit);
	mpz_clear(p1);
	mpz_clear(m1);
}

/**
 * @brief Main entry point
 */
int main()
{
	int ret = 0;
	mpz_t start;
	mpz_t end;
	mpz_t step;

	mpz_init(start);
	mpz_init(end);
	mpz_init(step);

	make_lcm(start, 701);				// start = lcm(1,2,3,..,701), around 2^1024
	mpz_mul_ui(end, start, 2);		        // end   = start*2;
	make_lcm(step,  691);				// step  = lcm(1,2,3,..,691)
						
        find_prime_oasis(start, end, step);		// display prime oasis.

	mpz_clear(start);
	mpz_clear(end);
	mpz_clear(step);

	return 0;
}
