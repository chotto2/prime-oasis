/**
 * @file oasis_layer2.c
 * @brief Find prime numbers from the desert.
 * @author N.Arai
 * @date 2026-01-02
 *
 * This program demonstrates finding prime numbers within prime deserts
 * using the innovative LCM method - a constructive (non-sieve) approach.
 *
 * @note v1.4.2 (2026-01-02): Enhanced user interaction and twin prime display
 *       1. Keyboard interrupt support: Press 'q', 'Q', ESC, or Ctrl+C to stop
 *       2. Non-blocking keyboard input check during computation
 *       3. Twin prime counter and detection in statistics output
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
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define XPT_ON
#include "xpt.h"
int xpt_flg = 0;

#define make_lcm(A, B) {		\
	mpz_set_ui(A, 1);		\
	for (int i = 2; i <= B; i++) {	\
		mpz_lcm_ui(A, A, i);	\
	}				\
}
	
/* Global variables: interrupt flag and terminal settings */
static volatile sig_atomic_t interrupted = 0;
static struct termios orig_termios;
static int tty_configured = 0;

/**
 * @brief Signal handler for Ctrl+C (SIGINT)
 */
void signal_handler(int sig)
{
	(void)sig; // Suppress unused parameter warning
	interrupted = 1;
}

/**
 * @brief Enable terminal raw mode for non-blocking input
 */
void enable_raw_mode(void)
{
	struct termios raw;

	if (!isatty(STDIN_FILENO)) return;

	tcgetattr(STDIN_FILENO, &orig_termios);
	tty_configured = 1;

	raw = orig_termios;
	raw.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	raw.c_cc[VMIN] = 0;  // Non-blocking
	raw.c_cc[VTIME] = 0; // No timeout

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/**
 * @brief Restore original terminal settings
 */
void disable_raw_mode(void)
{
	if (tty_configured) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
		tty_configured = 0;
	}
}

/**
 * @brief Check for keyboard input (non-blocking)
 * @return Character code of pressed key, or 0 if no input
 */
int check_key_pressed(void)
{
	char c;
	int n = read(STDIN_FILENO, &c, 1);
	if (n == 1) {
		return (int)(unsigned char)c;
	}
	return 0;
}

/**
 * @brief Check if interrupt has been requested
 * @return 1 if should interrupt, 0 to continue
 */
int should_interrupt(void)
{
	if (interrupted) return 1;

	int key = check_key_pressed();
	if (key == 'q' || key == 'Q' || key == 27) { // 27 = ESC
		return 1;
	}
	return 0;
}

/**
 * @brief Find prime numbers around LCM.
 *
 * @param[in] start Upper boundary of the prime gap (top lcm)
 * @param[in] end   Lower boundary of the prime gap (botom lcm)
 * @param[in] step  Search increment (smaller lcm)
 *
 * @note Modified in v1.4.2 (2026-01-02):
 *       - Added keyboard interrupt checking in main loop (every 100 iterations)
 *       - Added twin prime counter and statistics display
 *       - Display "oasis primes" marker for the second prime in twin pairs
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
	int twin_flag;
	int twin_cnt = 0;
	int try_cnt  = 0;
	int hit_cnt  = 0;
	int loop_cnt = 0;

	mpz_t pit;
	mpz_t p1;	// plus 1
	mpz_t m1;	// minus 1

	mpz_init(pit);
	mpz_init(p1);
	mpz_init(m1);

	mpz_set_ui(p1, 0);				// p1 = 0;
	for (mpz_set(pit, start);			// for (pit =  start
	     mpz_cmp(pit, end) <= 0;			//      pit <= end
	     mpz_add(pit, pit, step)) {			//      pit += step) {

	   /* Periodically check for interrupt (every 100 iterations) */
	   if (++loop_cnt % 100 == 0) {
	      loop_cnt = 0;
	      if (should_interrupt()) {
	         printf("\n\n*** Interrupted by user ***\n");
		 printf("Current position: ");
		 gmp_printf("pit = %Zd\n", pit);
		 break;
	      }
	   }

           twin_flag = 0;				//
	   if (mpz_cmp(pit, start) != 0) {		//    if (pit != start) {
	      mpz_sub_ui(m1, pit, 1);			//       m1 = pit - 1;
	      if (mpz_cmp(m1, p1) == 0) {		//       if (m1 == p1) {
	         ;					//          nothing to do.
	      }						//       }
              else {					//       else  {
		 try_cnt++;				//
	         if (mpz_probab_prime_p(m1, 25)) {	//          prime?
		    hit_cnt++;				//
		    gmp_printf("oasis prime  = %Zd\n", m1);
		    twin_flag = 1;			//
	         }
	      }
	   }

	   if (mpz_cmp(pit, end) != 0) {		//    if (pit != end) {
	      mpz_add_ui(p1, pit, 1);			//       p1 = pit + 1;
	      try_cnt++;				//
	      if (mpz_probab_prime_p(p1, 25)) {		//          prime?
		 hit_cnt++;				//
		 gmp_printf("oasis prime%c = %Zd\n", (twin_flag)? 's':' ', p1);
		 if (twin_flag) twin_cnt++;		//
	      }
           }
	}
	printf("(try=%d, hit=%d, twin=%d)\n", try_cnt, hit_cnt, twin_cnt); 

	mpz_clear(pit);
	mpz_clear(p1);
	mpz_clear(m1);
}

/**
 * @brief Main entry point
 */
int main()
{
	mpz_t start;
	mpz_t end;
	mpz_t step;

	/* Setup signal handler and raw mode */
	signal(SIGINT, signal_handler);
	enable_raw_mode();
	atexit(disable_raw_mode); // プログラム終了時に自動復元

	XPT_INIT();

	printf("Prime Oasis Layer 2 - Press 'q', ESC, or Ctrl+C to interrupt\n");
	printf("=============================================================\n\n");

	mpz_init(start);
	mpz_init(end);
	mpz_init(step);

	make_lcm(start, 701);				// start = lcm(1,2,3,..,701), around 2^1024
	mpz_mul_ui(end, start, 2);		        // end   = start*2;
	make_lcm(step,  683);				// step  = lcm(1,2,3,..,683)
						
        find_prime_oasis(start, end, step);		// display prime oasis.

	mpz_clear(start);
	mpz_clear(end);
	mpz_clear(step);

	return 0;
}
