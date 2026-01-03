# Prime Oasis - Primes Like an Oasis in the Desert

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/chotto2/prime-oasis)

In intervals where prime numbers do not appear consecutively (hereafter referred to as "prime deserts"), there is approximately a 1% probability that a prime number (hereafter referred to as "oasis prime") may exist.

## Overview

This is a somewhat unconventional project: "Let's search for primes in the universally disliked prime deserts."
99% of prime deserts do not contain oasis primes.
So why are we fixated on prime deserts? Because prime deserts exist at regular intervals, making them suitable for systematic searching.
It's like using a sniper rifle scope to precisely target and shoot primes.
The intriguing question is: with a 99% miss rate, what does the time-to-hit feel like?
We've made this experience tangible, so please give it a try.

## Location and Size of Prime Deserts

LCM(1,2,3,...n) indicates the center point of a prime desert with size 2n+1.  
*Note: LCM() denotes the least common multiple (same hereafter).*

### Positions for Prime Testing

Two points: LCM(1,2,3,...n)±1

### Structure of Prime Deserts

Here, we've prepared a prime desert with its center point at LCM(1,2,3,...701) and size 1403.
The specific value of the center point is as follows:

```text
2626719802592710061239991829174692172528343457131512513976281060940948726537757144360059535978552440933031373189919061646742591656110407680769241480651946904946211000952247368842571964792534944293239882868728171914661797572466549735970311937966946924655356399900939424721115438589874540587436119564591680000
```

When this value is expressed in factorized product form, it appears as follows:

```text
2^9*3^5*5^4*7^3*11^2*13^2*17^2*19^2*23^2*29^1*31^1*37^1*…677^1*683^1*691^1*701^1
```

Please focus on the end of this product expression.
This shows that from origin 0 to LCM(1,2,3,...701), there are 701 prime deserts centered at multiples of LCM(1,2,3,...691).  
Similarly, from origin 0 to LCM(1,2,3,...691), there are 691 prime deserts centered at multiples of LCM(1,2,3,...683).  
Similarly, from origin 0 to LCM(1,2,3,...683), there are 683 prime deserts centered at multiples of LCM(1,2,3,...677).  
And so on...

Thus, examining a single prime desert reveals that the smaller prime deserts below it form a hierarchical structure similar to matryoshka dolls.  
While a matryoshka doll contains just one slightly smaller doll inside, a prime desert contains multiple smaller prime deserts.  
This means that from origin 0 to LCM(1,2,3,...701), there are `701*691=484,391` prime deserts centered at multiples of LCM(1,2,3,...683).  
Similarly, from origin 0 to LCM(1,2,3,...701), there are `701*691*683=330,839,053` prime deserts centered at multiples of LCM(1,2,3,...677).  
Where center points of large and small prime deserts coincide, they form an overlapping state, with the size determined by the larger desert.
However, in this approach, the size of prime deserts is not considered an issue at all.

### Program Structure

The program consists of the main() function, make_lcm() macro, and find_prime_oasis() function.

The main() function specifies the search start position (start), end position (end), and granularity (step).  
All of start, end, and step are specified as prime desert center points.
Note that start is set to LCM(1,2,3,...701) rather than origin 0.  
Normally, when searching for multiples of an integer n from an intermediate position m, division, multiplication, and position adjustment are required.  
By setting start to LCM(1,2,3,...701), all multiples from 1 to 701 become equivalent to searching from origin 0, eliminating the need for position adjustment.  
Additionally, this standardizes the bit count of output results.  
We set end as `start*2`. This provides the same size as searching from origin 0 to LCM(1,2,3,...701), which we judged to be more intuitive.  
For step, we introduced the concept of "layers" as follows:

- **Layer 1**: LCM(1,2,3,...691)   // step for oasis_layer1 command
- **Layer 2**: LCM(1,2,3,...683)   // step for oasis_layer2 command
- **Layer 3**: LCM(1,2,3,...677)   // step for oasis_layer3 command

Layer 1 has the coarsest granularity and is fastest, while Layer 3 has the finest granularity and is slowest.  
All content from Layer 1 is included in Layer 2 in an overlapping form.

The make_lcm(A, B) macro calculates the value of LCM(1,2,3,...B) and assigns it to mpz_t type variable A.

The find_prime_oasis() function searches for oasis primes in prime deserts from start to end in step increments.  
All of start, end, and step are mpz_t type variables with values in LCM(1,2,3,...n) format.

## Program Components

The program consists of the following five executables:

- **oasis_layer1**: Full-spec version for Layer 1
- **oasis_layer2**: Full-spec version for Layer 2
- **oasis_layer3**: Minor-change version for Layer 3 (adjusted to complete in approximately 11 minutes on Codespace)
- **oasis_divs**: Displays prime factorization information for LCM(1,2,3,...n) format sequentially from 2
- **prime_oasis**: Generic version accepting start/end/step via command-line arguments (added in v1.5.0)
  - Validates argument count and values, displays USAGE for invalid input
  - Accepts 2 or 3 arguments
  - All arguments specify n values (n in LCM(1,2,3,...n))

## Features

- 🎯 **Non-Sieve Approach** - Actively narrows down prime candidate regions instead of elimination
- 🔢 **LCM Method** - Innovative algorithm utilizing least common multiple
- 🏜️ **Prime Desert Search** - Efficient discovery of primes within large prime deserts
- 🐳 **Docker Support** - Reproducible build environment
- 📊 **~1024 bits** - Appropriately sized for educational and research purposes

## Requirements

- Docker Desktop
- Git

## Build and Run

```bash
# Clone the repository
git clone https://github.com/chotto2/prime-oasis.git
cd prime-oasis

# Build Docker image
docker build -t prime-oasis .

# Run each layer
docker run -it prime-oasis /app/build/oasis_layer1
docker run -it prime-oasis /app/build/oasis_layer2
docker run -it prime-oasis /app/build/oasis_layer3
docker run -it prime-oasis /app/build/oasis_divs

# Run prime_oasis command
# Usage: prime_oasis <start> [<end>] <step>
#   - With 2 arguments: prime_oasis <start> <step> (end will be set to start*2)
#   - With 3 arguments: prime_oasis <start> <end> <step>

# Example 1: Equivalent to oasis_layer2 (start=701, end=701*2, step=683)
docker run -it prime-oasis /app/build/prime_oasis 701 683

# Example 2: Explicitly specify start/end/step
docker run -it prime-oasis /app/build/prime_oasis 701 709 683

# Example 3: Display USAGE message (with invalid arguments)
docker run -it prime-oasis /app/build/prime_oasis
```

## Performance

```text
oasis_layer1:
real    0m0.756s
user    0m0.016s
sys     0m0.008s

oasis_layer2:
real    5m34.832s
user    0m0.061s
sys     0m0.157s

oasis_layer3:
real    11m4.516s
user    0m0.087s
sys     0m0.284s
```

*Measured on Codespace: 2-Core  
*Without output redirection

## Output Example

```text
Prime Oasis Layer 2 - Press 'q', ESC, or Ctrl+C to interrupt
=============================================================

oasis prime  = 2627072279800520541108487733245775976709705914990377662819691566645970417005220318148142724703856185033686932519587335291395440838811257152578688808732448765114590478925747900600954691097754312058906587230259388083365289208029989861267516201208765727131254172910746710700018581925544158406797303706682880001
oasis prime  = 2627164466147178666612555892772059433187908403968850086363352775830360705281326071292718327908935625798473771113500576090766186009671479322128851956076580020850935880857278808906993250285273223628388648370967552312103125481946581894037554239287395260086488975082542462417577865567180827682630228789999039999
...
oasis prime  = 2628943120365053088102812147161528475826168191318200375911639635388008620255601779023353495630468365260243362807826633866860563423915765887567293857775112484469835400477404569164678392256226340968983710379925073901868437119866710526306523444569188601811019275808954613203427573476405976063406665691628479999
oasis primes = 2628943120365053088102812147161528475826168191318200375911639635388008620255601779023353495630468365260243362807826633866860563423915765887567293857775112484469835400477404569164678392256226340968983710379925073901868437119866710526306523444569188601811019275808954613203427573476405976063406665691628480001
...
oasis prime  = 5253325727933665967445546520111034192936554427877853210633921804654121214616795417188584503292006749156620181057945296423674262807040540916800046132231731670453407093636133027424743356471075939000648984328346258605588620924212603431459988652424998543895422750060248803202892938563609666187078625790969279999
oasis prime  = 5253336573386213982210731009467067540757519426581202907521411358675814189708101976382063986022016095128948044421935089458894350474200567054394182973095747112304741846804548428401924363434313458008823344462547219103087189897614555435315287245140131430125450373845165950463782266050861039043058969918418239999
(try=968782, hit=16093, twin=143)
```

The first line indicates that oasis_layer2 is running and the process can be interrupted by pressing 'q', ESC, or Ctrl+C.

Following the separator line, discovered oasis primes are displayed.

The first and last blocks show lists of oasis primes where only one prime was found in a single prime desert.

The middle block indicates that twin primes were discovered.  
The second prime of a twin prime pair is displayed with the keyword "oasis primes" (plural).  
The first prime of a twin prime pair is displayed with the keyword "oasis prime" (singular) immediately before it.

The last line displays statistical information in parentheses.  
"try" indicates the number of times primality was tested.  
"hit" indicates the number of primes found.  
"twin" indicates the number of twin prime pairs found.

## Technical Details

- **Language**: C
- **Library**: GMP (GNU Multiple Precision Arithmetic Library)
- **Build System**: CMake
- **LCM Range**: 1..701 (approximately 1020 bits)

## Important Notice

⚠️ **Important**: This version is an implementation for educational and research purposes. It handles approximately 1024-bit primes and does not affect modern cryptographic systems (such as RSA-4096).

## Future Plans

- 📝 Paper submission to arXiv planned
- 📚 Detailed theoretical background of the algorithm

## License

MIT License

## Author

N.Arai

## Citation

Paper in preparation. Proper citation format will be provided after publication.

## Related Links

- Paper: arXiv (in preparation)
- Repository: <https://github.com/chotto2/prime-oasis>
