/**
 * @file oasis_layer3.c
 * @brief Find prime numbers from the desert.
 * @author N.Arai
 * @date 2026-01-01
 *
 * This program demonstrates finding prime numbers within prime deserts
 * using the innovative LCM method - a constructive (non-sieve) approach.
 *
 * @note v1.4.1 (2026-01-01): Prime desert boundary handling improvements
 *       1. Fixed twin prime display order (was reversed)
 *       2. Skip primality test for (start - 1) at first desert boundary
 *       3. Skip primality test for (end + 1) at last desert boundary
 *       4. Eliminate duplicate output when previous desert's (+1) matches current desert's (-1)
 *       5. Corrected terminology: "prime oasis" → "oasis prime" in output
 *
 * @note Around 1024-bit version for educational purposes
 * @see Related paper on arXiv (to be published)
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define MAX_HIT_COUNT (32000)

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
 * @note Modified in v1.4.1 (2026-01-01):
 *       - Fixed display order for twin primes (now shows in ascending order)
 *       - Added boundary checks to skip tests at range limits
 *       - Added duplicate detection for overlapping boundary tests
 *
 * @note All mpz_t parameters must be initialized before calling
 * @details Search for primes in the form: pit +- 1, where pit = start + k*step
 */
void find_prime_oasis(mpz_t start, mpz_t end, mpz_t step)
{
	mpz_t pit;
	mpz_t p1;	// plus 1
	mpz_t m1;	// minus 1
	int cnt = 0;

	mpz_init(pit);
	mpz_init(p1);
	mpz_init(m1);

	mpz_set_ui(p1, 0);				// p1 = 0;
	for (mpz_set(pit, start);			// for (pit =  start
	     mpz_cmp(pit, end) < 0;			//      pit <  end
	     mpz_add(pit, pit, step)) {			//      pit += step) {

	   if (mpz_cmp(pit, start) != 0) {		//    if (pit != start) {
	      mpz_sub_ui(m1, pit, 1);			//       m1 = pit - 1;
	      if (mpz_cmp(m1, p1) == 0) {		//       if (m1 == p1) {
	         ;					//          nothing to do.
	      }						//       }
              else {					//       else  {
	         if (mpz_probab_prime_p(m1, 25)) {	//          prime?
		    gmp_printf("oasis prime = %Zd\n", m1);
		    cnt++;
		    if (cnt > MAX_HIT_COUNT) break;
	         }
	      }
	   }

	   if (mpz_cmp(pit, end) != 0) {		//    if (pit != end) {
	      mpz_add_ui(p1, pit, 1);			//       p1 = pit + 1;
	      if (mpz_probab_prime_p(p1, 25)) {		//          prime?
		 gmp_printf("oasis prime = %Zd\n", p1);
		 cnt++;
		 if (cnt > MAX_HIT_COUNT) break;
	      }
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
	make_lcm(step,  677);				// step  = lcm(1,2,3,..,677)
						
        find_prime_oasis(start, end, step);		// display prime oasis.

	mpz_clear(start);
	mpz_clear(end);
	mpz_clear(step);

	return 0;
}
