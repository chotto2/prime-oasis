/**
 * @file prime_oases.c
 * @brief Find prime numbers from the desert.
 * @author N.Arai
 * @date 2026-01-15
 *
 * This program demonstrates finding prime numbers within prime deserts
 * using the innovative LCM method - a constructive (non-sieve) approach.
 *
 * @note v1.6.1 (2026-01-15): Add a feature to search from the middle
 *       1. Specify desert/no/num as arguments to the command
 *       2. Display USAGE message
 *       3. Check numbers and value of arguments
 *       4. Added a member(no) in statistics structure
 * 
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
#include <string.h>
#include <ctype.h>

#define ERR_OK		(0)
#define ERR_PNUM	(-1)
#define ERR_NOND	(-2)
#define ERR_NONX	(-3)
#define ERR_TSML	(-4)
#define ERR_INVL	(-5)	// Invalid value

#define make_lcm(A, B) {			\
	mpz_set_ui(A, 1);			\
	for (int ii = 2; ii <= B; ii++) {	\
		mpz_lcm_ui(A, A,  ii);		\
	}					\
}
	
typedef struct {
	int		desert;
	uint64_t	no;
	uint64_t	num;
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
 * @param[in] no     Starting position to search.
 * @param[in] num    Number of deserts to search.
 *
 * @note Modified in v1.6.1 (2026-01-15):
 *       - Added 'no' parameters.
 *
 * @note Modified in v1.6.0 (2026-01-10):
 *       - Chaged function name (find_prime_oasis->find_prime_oases)
 *       - Chaged statistics format.
 *       - Deleted start/end/step parameters.
 *       - Added desert/num parameters.
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
 * @details Search for primes in the form: pit +- 1, where pit = desert * no + i
 */
void find_prime_oases(mpz_t desert, mpz_t no, mpz_t num)
{
	mpz_t pit;
	mpz_t p1;	// plus 1
	mpz_t m1;	// minus 1
	mpz_t i;
	mpz_t r;

	mpz_init(pit);
	mpz_init(p1);
	mpz_init(m1);
	mpz_init(i);
	mpz_init(r);

	mpz_set_ui(p1, 0);				// p1 = 0;
	mpz_set(pit, desert);				// pit = desert;
	mpz_mul(pit, pit, no);				// pit *= no;	// Starting position to search
							//
	for (mpz_set_ui(i, 0);				// for (i = 0;
	     mpz_cmp(i, num) < 0;			//      i < n; 
	     mpz_add_ui(i, i, 1),			//      i++,
	     mpz_add(pit, pit, desert)) {		//      pit += desert) {

	   /* Periodically check for interrupt (every 100 iterations) */
	   mpz_mod_ui(r, i, 100); 			// r = i % 100;
	   if (mpz_cmp_ui(r, 0) == 0) {			// if (r == 0) {
	      if (should_interrupt()) {
	         printf("\n\n*** Interrupted by user ***\n");
		 printf("Current position: ");
		 gmp_printf("pit = %Zd\n", pit);
		 break;
	      }
	   }

	   /*--- m1 ---*/
	   mpz_sub_ui(m1, pit, 1);			//    m1 = pit - 1;
	   if (mpz_cmp(m1, p1) != 0) {			//    if (m1 != p1) {
	      po_stat->try_cnt++;			//       try++;
	      if (mpz_probab_prime_p(m1, 25)) {		//       if (m1 == prime) {
		 po_stat->hit_cnt++;			//          hit++;
		 mpz_add(r, no, i);			//          r = po_stat->no + i;
		 gmp_printf("d%d*%Zd-1 = %Zd\n", po_stat->desert, r, m1);
	      }
	   }

	   /*--- p1 ---*/
	   mpz_add_ui(p1, pit, 1);			//       p1 = pit + 1;
	   po_stat->try_cnt++;				//       try++;
	   if (mpz_probab_prime_p(p1, 25)) {		//       prime?
	      po_stat->hit_cnt++;			//          hit++;
	      mpz_add(r, no, i);			//          r = po_stat->no + i;
	      gmp_printf("d%d*%Zd+1 = %Zd\n", po_stat->desert, r, p1);
	   }
	}
	printf("{ prime_oases d%d x%lu %lu: try=%lu, hit=%lu(%2.1f%%) }\n",
		po_stat->desert,
		po_stat->no,
		po_stat->num,
		po_stat->try_cnt,
		po_stat->hit_cnt, 
		(float)po_stat->hit_cnt / (float)po_stat->try_cnt * 100.0);

	mpz_clear(pit);
	mpz_clear(p1);
	mpz_clear(m1);
	mpz_clear(i);
	mpz_clear(r);
}

/**
 * @brief Validate that a string contains only digits
 * @param[in] str String to validate
 * @return 1 if valid (all digits), 0 otherwise
 */
static int is_valid_number_string(const char *str)
{
	if (!str || *str == '\0') return 0;

	while (*str) {
		if (!isdigit((unsigned char)*str)) {
			return 0;
		}
		str++;
	}
	return 1;
}

/**
 * @brief Display usage information for the prime_oases command
 * 
 * @details Prints command syntax, parameter descriptions, and usage notes:
 *          - Command format: prime_oases d<n> [<num>] / prime_oases d<n> x<no> [<num>]
 *          - Parameter meanings (n values for LCM(1,2,3,...,n))
 *          - Optional x<no> parameter specifies the position to start the search
 *          - Optional <num> parameter is the number of searches
 *          - Reference to results/resultd.txt for value selection
 */
static void disp_usage()
{
	printf("---< USAGE:\n");
	printf("       prime_oases d<n> [<num>]\n");
	printf("       prime_oases d<n> x<no> [<num>]\n\n");
	printf("---< DESCRIPTION:\n");
	printf("       d<n>     Central coordinates of the desert that can be calculated by LCM(1,2,3,...,n)\n");
	printf("       x<no>    Starting position from the middle (optional, defaults to x1)\n");
	printf("       <num>    Number of deserts to search (optional, defaults to 1)\n");
	printf("---< CAUTION:\n");
	printf("       1) Since d<n> is a least common multiple, it may be the same value even if n changes.\n");
	printf("          The value refers to results/resultd.txt.\n");
	printf("       2) If you omit x<no>, x1 is specified as the default value.\n");
	printf("       3) If you omit <num>,  1 is specified as the default value.\n");
	printf("       4) When using two arguments, second argument without 'x' prefix is treated as <num>.\n");
	printf("          Example: 'prime_oases d691 100' means search from x1 for 100 deserts.\n");
	printf("---< EXAMPLES:\n");
	printf("       prime_oases d3              # Search d3*1±1 for 1 desert\n");
	printf("       prime_oases d691 100        # Search d691*1±1 for 100 deserts\n");
	printf("       prime_oases d691 x701       # Search d691*701±1 for 1 desert\n");
	printf("       prime_oases d691 x701 701   # Search d691*701±1 for 701 deserts\n");
	printf("---\n");
}

/**
 * @brief Parse and validate command line parameters
 * 
 * @param[in]  argc   Argument count from main()
 * @param[in]  argv   Argument vector from main()
 * @param[out] desert Central coordinates of the desert(will be set to LCM(1,2,3,...n))
 * @param[out] no     Position to start the search.
 * @param[out] num    Number of deserts to search.
 * 
 * @return 0 on success, negative error code on failure:
 *         ERR_PNUM: Invalid number of arguments
 *         ERR_TSML: Invalid parameter range 
 *         ERR_NOND: 'd' is not exist.
 *         ERR_NONX: 'x' is not exist.
 *         ERR_INVL: Invalid value format
 * 
 * @details Supports four usage patterns:
 *          - 2 args: prime_oases d<n>              (x1, num=1)
 *          - 3 args: prime_oases d<n> <num>        (x1=1)
 *          - 3 args: prime_oases d<n> x<no>        (num=1)
 *          - 4 args: prime_oases d<n> x<no> <num>
 *
 * @note All mpz_t parameters must be initialized before calling
 * @note Validates that desert >= 2, no >= 1, num >= 1
 * @note Display an error message whenever possible in case of an error.
 */
static int check_param(int argc, char *argv[], mpz_t desert, mpz_t no, mpz_t num)
{
	int   ret = ERR_OK;
	char *dp;
	char *xp;
	char *nump;
	int   d_val;

	switch(argc) {
	case 2:
		/*--- d<n> ---*/
		dp = argv[1];
		if (*dp != 'd') {					// d<n> omit?
			printf("ERR: First argument must start with 'd' (e.g., d691)\n");
			ret = ERR_NOND;
		}
		else {
			if (!is_valid_number_string(&dp[1])) { 		// d<n> str ok?
				printf("ERR: Invalid number format in '%s'\n", dp);
				ret = ERR_INVL;
			}
			else {
				d_val = atoi(&dp[1]);			// n = str2val(d<n>)
				if (d_val < 2) {
					printf("ERR: d<n> must be >= 2, got %d\n", d_val);
					ret = ERR_TSML;
				}
				else {
					make_lcm(desert, d_val);	// desert = lcm(1,2,3,...,n)
					mpz_set_ui(no,   1);		// no  = default value
					mpz_set_ui(num,  1);		// num = default value
									//
					po_stat->desert = d_val;
					po_stat->no     = 1;
					po_stat->num    = 1;
				}
			}
		}
		break;

	case 3:
		/*--- d<n> ---*/
		dp = argv[1];
		if (*dp != 'd') {					// d<n> omit?
			printf("ERR: First argument must start with 'd' (e.g., d691)\n");
			ret = ERR_NOND;
		}
		else {
			if (!is_valid_number_string(&dp[1])) { 		// d<n> str ok?
				printf("ERR: Invalid number format in '%s'\n", dp);
				ret = ERR_INVL;
			}
			else {
				d_val = atoi(&dp[1]);			// n = str2val(d<n>)
				if (d_val < 2) {
					printf("ERR: d<n> must be >= 2, got %d\n", d_val);
					ret = ERR_TSML;
				}
				else {
					make_lcm(desert,  d_val);	// desert = lcm(1,2,3,...,n)
					po_stat->desert = d_val;
				}
			}
		}

		/*--- <num> ---*/
		xp = argv[2];
		if (*xp != 'x') {				// x<no> omit? 
			nump = xp; 				// <num> = x<no>
			if (!is_valid_number_string(nump)) {	// <num> str ok?
				printf("ERR: Invalid number format in '%s'\n", nump);
				ret = ERR_INVL;
			}
			else {
				if (mpz_set_str(num, nump, 10) != 0) {	// num = str2val(<num>)
					printf("ERR: Failed to parse number '%s'\n", nump);
					ret = ERR_INVL;
				}
				else {
					if (mpz_cmp_ui(num, 1) < 0) {
						printf("ERR: <num> must be >= 1, got %s\n", nump);
						ret = ERR_TSML;
					}
					else {
						po_stat->num = mpz_get_ui(num);
						mpz_set_ui(no, 1);	// x<no> = default
						po_stat->no =  1;
					}
				}
			}
		}

		/*--- x<no> ---*/
		else {	
			if (!is_valid_number_string(&xp[1])) {	// x<no> str ok?
				printf("ERR: Invalid number format in '%s'\n", xp);
				ret = ERR_INVL;
			}
			else {
				if (mpz_set_str(no, &xp[1], 10) != 0) {	// no = str2val(x<no>)
					printf("ERR: Failed to parse number '%s'\n", xp);
					ret = ERR_INVL;
				}
				else {
					if (mpz_cmp_ui(no, 1) < 0) {
						printf("ERR: x<no> must be >= 1, got %s\n", xp);
						ret = ERR_TSML;
					}
					else {
						po_stat->no  = mpz_get_ui(no);
						mpz_set_ui(num, 1);	// num = default
						po_stat->num =  1;
					}
				}
			}

		}
		break;

	case 4:
		/*--- d<n> ---*/
		dp = argv[1];
		if (*dp != 'd') {			// d<n> omit?
			printf("ERR: First argument must start with 'd' (e.g., d691)\n");
			ret = ERR_NOND;
		}
		else {
			if (!is_valid_number_string(&dp[1])) { 		// d<n> str ok?
				printf("ERR: Invalid number format in '%s'\n", dp);
				ret = ERR_INVL;
			}
			else {
				d_val = atoi(&dp[1]);			// n = str2val(d<n>)
				if (d_val < 2) {
					printf("ERR: d<n> must be >= 2, got %d\n", d_val);
					ret = ERR_TSML;
				}
				else {
					make_lcm(desert,  d_val);	// desert = lcm(1,2,3,...,n)
					po_stat->desert = d_val;
				}
			}
		}

		/*--- x<no> ---*/
		if (ret == ERR_OK) {
			xp = argv[2];			// may be x<no> str
			if (*xp != 'x') {		// x<no> omit?
				printf("ERR: Second argument must start with 'x' (e.g., x701)\n");
				ret = ERR_NONX;
			}
			else {
				if (!is_valid_number_string(&xp[1])) {	// x<no> str ok?
					printf("ERR: Invalid number format in '%s'\n", xp);
					ret = ERR_INVL;
				}
				else {
					if (mpz_set_str(no, &xp[1], 10) != 0) {	// no = str2val(x<no>)
						printf("ERR: Failed to parse number '%s'\n", xp);
						ret = ERR_INVL;
					}
					else {
						if (mpz_cmp_ui(no, 1) < 0) {
							printf("ERR: x<no> must be >= 1, got %s\n", xp);
							ret = ERR_TSML;
						}
						else {
							po_stat->no = mpz_get_ui(no);
						}
					}
				}
			}
		}

		/*--- <num> ---*/
		if (ret == ERR_OK) {
			nump = argv[3];		// may be <num> str
			if (!is_valid_number_string(nump)) { 	// <num> str ok?
				printf("ERR: Invalid number format in '%s'\n", nump);
				ret = ERR_INVL;
			}
			else {
				if (mpz_set_str(num, nump, 10) != 0) {	// num = str2val(<num>)
					printf("ERR: Failed to parse number '%s'\n", nump);
					ret = ERR_INVL;
				}
				else {
					if (mpz_cmp_ui(num, 1) < 0) {
						printf("ERR: <num> must be >= 1, got %s\n", nump);
						ret = ERR_TSML;
					}
					else {
						po_stat->num = mpz_get_ui(num);
					}
				}
			}
		}
		break;

	default:
		if (argc > 1) printf("ERR: Too many parameters\n");
		ret = ERR_PNUM;
		break;
	}

	return ret;
}

/**
 * @brief Main entry point
 */
int main(int argc, char *argv[])
{
	int ret = ERR_OK;
	mpz_t desert;
	mpz_t num;
	mpz_t no;

	/* Setup signal handler and raw mode */
	signal(SIGINT, signal_handler);
	enable_raw_mode();
	atexit(disable_raw_mode); // Restore terminal on exit

	printf("Prime Oases - Press 'q', ESC, or Ctrl+C to interrupt\n");
	printf("====================================================\n\n");

	mpz_init(desert);
	mpz_init(no);
	mpz_init(num);

	ret = check_param(argc, argv, desert, no, num);
	if (ret) {	// err?
		disp_usage();
	}
	else {
		find_prime_oases(desert, no, num);
	}

	mpz_clear(desert);
	mpz_clear(num);
	mpz_clear(no);

	return ret;
}
