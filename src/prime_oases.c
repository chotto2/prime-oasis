/**
 * @file prime_oases.c
 * @brief Find prime numbers from the desert.
 * @author N.Arai
 * @date 2026-01-09
 *
 * This program demonstrates finding prime numbers within prime deserts
 * using the innovative LCM method - a constructive (non-sieve) approach.
 *
 * @note v1.6.0 (2026-01-09): Add prime_oases command
 *       1. Specify desert/num as arguments to the command
 *       2. Display USAGE message
 *       3. Check numbers and value of arguments
 *       4. Added typedef of statistics structure
 *
 * @note v1.5.0 (2026-01-03): Add prime_oasis command
 *       1. Specify start/end/step as arguments to the command
 *       2. Display USAGE message
 *       3. Check numbers and value of arguments
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
#include <stdint.h>
#include <gmp.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define make_lcm(A, B) {		\
	mpz_set_ui(A, 1);		\
	for (int i = 2; i <= B; i++) {	\
		mpz_lcm_ui(A, A, i);	\
	}				\
}
	
typedef struct {
	int		desert;
	int		num;
	uint64_t	time;
	uint64_t	try_cnt;
	uint64_t	hit_cnt;
	float		hit_per;	// hit_cnt/try_cnt*100.0
} PO_STAT;

static PO_STAT po_stat[1] = { 0 };

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
 * @param[in] desert The central coordinates of the first desert.
 * @param[in] num    Number of deserts to search.
 *
 * @note Modified in v1.4.2 (2026-01-02):
 *       - Chaged function name (find_prime_oasis->find_prime_oases)
 *       - Chaged statistics format.
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
void find_prime_oases(mpz_t desert, mpz_t num)
{
	mpz_t pit;
	mpz_t p1;	// plus 1
	mpz_t m1;	// minus 1

	mpz_init(pit);
	mpz_init(p1);
	mpz_init(m1);

	mpz_set_ui(p1, 0);				// p1 = 0;
	mpz_set(pit, desert);				// pit = desert;
	uint64_t n = mpz_get_ui(num);			// n = num;
	for (uint64_t i = 0; i < n; i++,		// for (i = 0; i < n; i++,
	     mpz_add(pit, pit, desert)) {		//      next desert) {

	   /* Periodically check for interrupt (every 100 iterations) */
	   if (i % 100 == 0) {
	      if (should_interrupt()) {
	         printf("\n\n*** Interrupted by user ***\n");
		 printf("Current position: ");
		 gmp_printf("pit = %Zd\n", pit);
		 break;
	      }
	   }

	   /*--- m1 ---*/
	   mpz_sub_ui(m1, pit, 1);			//       m1 = pit - 1;
	   if (mpz_cmp(m1, p1) != 0) {			//       if (m1 != p1) {
	      po_stat->try_cnt++;			//          try++;
	      if (mpz_probab_prime_p(m1, 25)) {		//          prime?
		 po_stat->hit_cnt++;			//		hit++;
		 gmp_printf("d%d*%llu-1 = %Zd\n", po_stat->desert, i+1, m1);
	      }
	   }

	   /*--- p1 ---*/
	   mpz_add_ui(p1, pit, 1);			//       p1 = pit + 1;
	   po_stat->try_cnt++;				//       try++;
	   if (mpz_probab_prime_p(p1, 25)) {		//       prime?
	      po_stat->hit_cnt++;			//          hit++;
	      gmp_printf("d%d*%llu+1 = %Zd\n", po_stat->desert, i+1, p1);
	   }
	}
	printf("{ prime_oases d%d %d: try=%lu, hit=%lu(%2.1f%%) }\n",
		po_stat->desert,
		po_stat->num,
		po_stat->try_cnt,
		po_stat->hit_cnt, 
		(float)po_stat->hit_cnt / (float)po_stat->try_cnt * 100.0);

	mpz_clear(pit);
	mpz_clear(p1);
	mpz_clear(m1);
}

/**
 * @brief Display usage information for the prime_oases command
 * 
 * @details Prints command syntax, parameter descriptions, and usage notes:
 *          - Command format: prime_oases d<n> [<num>]
 *          - Parameter meanings (n values for LCM(1,2,3,...,n))
 *          - Optional <num> parameter behavior
 *          - Reference to results/resultd.txt for value selection
 */
static void disp_usage()
{
	printf("---< USAGE:\n");
	printf("       prime_oases d<n> [<num>]\n\n");
	printf("---< DESCRIPTION:\n");
	printf("       d<n>	The central coordinates of the desert that can be calculated by LCM(1,2,3,...,n)\n");
	printf("       <num>    Number of deserts to search.(optional)\n");
	printf("---< CAUTION:\n");
	printf("       1) Since d<n> is a least common multiple, it may be the same value even if n changes.\n");
	printf("          The value refers to results/resultd.txt.\n");
	printf("       2) If you omit <num>, 1 is specified as the default value.\n");
	printf("---< EXAMPLES:\n");
	printf("       prime_oases d3\n");
	printf("       prime_oases d691 701\n");
	printf("---\n");
}

/**
 * @brief Parse and validate command line parameters
 * 
 * @param[in]  argc   Argument count from main()
 * @param[in]  argv   Argument vector from main()
 * @param[out] desert The central coordinates of the desert(will be set to LCM(1,2,3,...n))
 * @param[out] num    Number of deserts to search.
 * 
 * @return 0 on success, negative error code on failure:
 *         -1: Invalid number of arguments
 *         -2: Invalid parameter range 		(2 arguments case)
 *         -3: Invalid desert parameter values	(2 arguments case)
 *         -4: Invalid parameter range		(1 arguments case)
 *         -5: Invalid desert parameter values	(1 arguments case)
 * 
 * @details Supports two usage patterns:
 *          - 1 args: prime_oases d<n>
 *            (num is set to 1)
 *          - 2 args: prime_oases d<n> <num> 
 * 
 * @note All mpz_t parameters must be initialized before calling
 * @note Validates that start >= 2, end >= 3, step >= 2
 */
static int check_param(int argc, char *argv[], mpz_t desert, mpz_t num)
{
	int ret = 0;
	char *cp;

	switch(argc) {
	case 2:
		cp = argv[1];
		if (*cp != 'd') {
			ret = -5;
		}
		else {
			make_lcm(desert, atoi(&cp[1]));
			mpz_set_ui(num, 1);
			if (mpz_cmp_ui(desert, 2) >= 0) {
				;	// ok.
			}
			else {
				ret = -4;
			}
		}
		break;
	case 3:
		cp = argv[1];
		if (*cp != 'd') {
			ret = -3;
		}
		else {
			make_lcm(desert, atoi(&cp[1]));
			mpz_set_ui(num,  atoi(argv[2]));
			if ((mpz_cmp_ui(desert, 2) >= 0)
			&&  (mpz_cmp_ui(num,    1) >= 0)) {
				;	// ok.
				po_stat->desert = atoi(&cp[1]);
				po_stat->num    = atoi(argv[2]);
			}
			else {
				ret = -2;
			}
		}
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

/**
 * @brief Main entry point
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	mpz_t desert;
	mpz_t num;

	/* Setup signal handler and raw mode */
	signal(SIGINT, signal_handler);
	enable_raw_mode();
	atexit(disable_raw_mode); // Restore terminal on exit

	printf("Prime Oases - Press 'q', ESC, or Ctrl+C to interrupt\n");
	printf("====================================================\n\n");

	mpz_init(desert);
	mpz_init(num);

	ret = check_param(argc, argv, desert, num);
	if (ret) {	// err?
		disp_usage();
	}
	else {
		find_prime_oases(desert, num);
	}

	mpz_clear(desert);
	mpz_clear(num);

	return ret;
}
