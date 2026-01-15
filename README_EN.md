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

### Terminology

```text
Prime Desert Structure:

    ... large gap ...     p-1   p   p+1     ... large gap ...
                           ↓    ↓    ↓
                        [prime][?][prime]
                           ↑         ↑
                       oasis primes (1-2 actual primes at p±1)

                        <----------->
                        prime oasis (center point p or entire desert)

Where:
- p: Center point (typically even, p = LCM(1,2,3,...n))
- p±1: Positions where oasis primes may exist
- prime oasis: Center point p or the entire prime desert region(2n+1)
- oasis primes: Actual prime numbers found at p-1 and/or p+1
```

### Structure of Prime Deserts

Here, we've prepared a prime desert with its center point at LCM(1,2,3,...701) and size 1403.

```text
For convenience, we denote LCM(1,2,3,...n) as dn (same hereafter).
Example: LCM(1,2,3,...701) -> d701
```

The specific value of the center point is as follows:

```text
2626719802592710061239991829174692172528343457131512513976281060940948726537757144360059535978552440933031373189919061646742591656110407680769241480651946904946211000952247368842571964792534944293239882868728171914661797572466549735970311937966946924655356399900939424721115438589874540587436119564591680000
```

When this value is expressed in factorized product form, it appears as follows:

```text
2^9*3^5*5^4*7^3*11^2*13^2*17^2*19^2*23^2*29^1*31^1*37^1*…677^1*683^1*691^1*701^1
```

Please focus on the end of this product expression.
This shows that from origin 0 to d701, there are 701 prime deserts centered at multiples of d691.  
Similarly, from origin 0 to d691, there are 691 prime deserts centered at multiples of d683.  
Similarly, from origin 0 to d683, there are 683 prime deserts centered at multiples of d677.  
And so on...

Thus, examining a single prime desert reveals that the smaller prime deserts below it form a hierarchical structure similar to matryoshka dolls.  
While a matryoshka doll contains just one slightly smaller doll inside, a prime desert contains multiple smaller prime deserts.  
This means that from origin 0 to d701, there are `701*691=484,391` prime deserts centered at multiples of d683.  
Similarly, from origin 0 to d701, there are `701*691*683=330,839,053` prime deserts centered at multiples of d677.  
Where center points of large and small prime deserts coincide, they form an overlapping state, with the size determined by the larger desert.
However, in this approach, the size of prime deserts is not considered an issue at all.

## Program Components

The program consists of the following six executables:

- **oasis_layer1**: Full-spec version for Layer 1
- **oasis_layer2**: Full-spec version for Layer 2
- **oasis_layer3**: Minor-change version for Layer 3 (adjusted to complete in approximately 11 minutes on Codespace)
- **oasis_divs**: Displays prime factorization information for LCM(1,2,3,...n) format sequentially from 2
- **prime_oasis**: Generic version accepting start/end/step via command-line arguments (added in v1.5.0)
  - Validates argument count and values, displays USAGE for invalid input
  - Accepts 2 or 3 arguments
  - All arguments specify n values (n in LCM(1,2,3,...n))
- **prime_oases**: Generic version accepting desert/no/num via command-line arguments (added in v1.6.0)

### Program Evolution

- **Layer 1-3**: Fixed parameter versions (for demonstration)
  - start/end/step are fixed
  - No user modification allowed

- **prime_oasis**: First-generation generic version (v1.5.0)
  - Accepts start/end/step via command-line arguments
  - All parameters specified in LCM(1,2,3,...n) format
  - Limitation: Cannot specify `end=start*2`, affecting usability

- **prime_oases**: Second-generation generic version (v1.6.0)
  - Intuitive specification using desert and count (num)
  - Supports concise notation like `d701`
  - Eliminates special handling at start/end boundaries, simplifying logic
  - Adds command and parameters to statistics output
  - Adds a feature to search from the middle (v1.6.1)

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

# prime_oases command execution
# Usage: prime_oases d<n> [x<no>] [<num>]
#  - With 1 argument : prime_oases d<n>                 // with defaults 'x1 1'
#  - With 2 arguments: prime_oases d<n> <num>           // with default  'x1'
#                    : prime_oases d<n> x<no>           // with default  '1'
#  - With 3 arguments: prime_oases d<n> x<no> <num>     // without default
#
#   d<n> : Specify a desert in d<number> format (e.g., d701, d683, d677)
#   x<no>: Starting position to search in d<n>*x<no> format (e.g., x701)
#   <num>: Number of deserts to search

# Example 1: Check 701 instances of d691*1
docker run -it prime-oasis /app/build/prime_oases d691 701

# Example 2: Check 701 instances of d691*701
docker run -it prime-oasis /app/build/prime_oases d691 x701 701         // same as oasis_layer1

# Example 3: Check 484391 instances of d683*484391                      // 484391 = 701 * 691
docker run -it prime-oasis /app/build/prime_oases d683 x484391 484391   // same as oasis_layer2

# Example 4: Check 100 instances of d683*1
docker run -it prime-oasis /app/build/prime_oases d683 100

# Example 5: Check 50 instances of d701*1
docker run -it prime-oasis /app/build/prime_oases d701 50

# Example 6: Display USAGE message (with invalid arguments)
docker run -it prime-oasis /app/build/prime_oases
```

## Performance

```text
oasis_layer1: (try=1402, hit=20, twin=0)
real    0m0.933s
user    0m0.016s
sys     0m0.012s

oasis_layer2: (try=968782, hit=16093, twin=143)
real    5m54.973s
user    0m0.217s
sys     0m0.619s

oasis_layer3: (try=1928792, hit=32000, twin=282)
real    11m45.549s
user    0m0.488s
sys     0m1.140s

oasis_divs: lcm(1,2,3,...n) // n=2-1429
real    0m7.564s
user    0m0.017s
sys     0m0.037s

prime_oasis 701 683: (try=968782, hit=16093, twin=143)   // same as oasis_layer2
real    5m54.112s
user    0m0.222s
sys     0m0.605s

prime_oases d683 x484391 484391: (try=968782, hit=16093) // same as oasis_layer2
real    5m54.098s
user    0m0.215s
sys     0m0.606s
```

*Measured on Codespace: 2-Core  
*Without output redirection

## Output Example 1

Output from `oasis_layer2`:

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

The first line indicates that `oasis_layer2` is running and the process can be interrupted by pressing 'q', ESC, or Ctrl+C.

Following the separator line, discovered oasis primes are displayed.

The first and last blocks show lists of oasis primes where only one prime was found in a single prime desert.

The middle block indicates that twin primes were discovered.  
The second prime of a twin prime pair is displayed with the keyword "oasis primes" (plural).  
The first prime of a twin prime pair is displayed with the keyword "oasis prime" (singular) immediately before it.

The last line displays statistical information in parentheses.  
"try" indicates the number of times primality was tested.  
"hit" indicates the number of primes found.  
"twin" indicates the number of twin prime pairs found.

## Output Example 2

Output from `prime_oases`:

```text
Prime Oases - Press 'q', ESC, or Ctrl+C to interrupt
====================================================

d683*484456+1 = 2627072279800520541108487733245775976709705914990377662819691566645970417005220318148142724703856185033686932519587335291395440838811257152578688808732448765114590478925747900600954691097754312058906587230259388083365289208029989861267516201208765727131254172910746710700018581925544158406797303706682880001
d683*484473-1 = 2627164466147178666612555892772059433187908403968850086363352775830360705281326071292718327908935625798473771113500576090766186009671479322128851956076580020850935880857278808906993250285273223628388648370967552312103125481946581894037554239287395260086488975082542462417577865567180827682630228789999039999
...
d683*968761-1 = 5253325727933665967445546520111034192936554427877853210633921804654121214616795417188584503292006749156620181057945296423674262807040540916800046132231731670453407093636133027424743356471075939000648984328346258605588620924212603431459988652424998543895422750060248803202892938563609666187078625790969279999
d683*968763-1 = 5253336573386213982210731009467067540757519426581202907521411358675814189708101976382063986022016095128948044421935089458894350474200567054394182973095747112304741846804548428401924363434313458008823344462547219103087189897614555435315287245140131430125450373845165950463782266050861039043058969918418239999
{ prime_oases d683 x484391 484391: try=968782, hit=16093(1.7%) }
```

The first line indicates that `prime_oases` is running and the process can be interrupted by pressing 'q', ESC, or Ctrl+C.

Following the separator line, discovered oasis primes are displayed.

`d683*484456+1 = 262707...` indicates "the 484456th instance of d683 desert has an oasis prime at center+1, which is 262707..."  
`d683*484473-1 = 262716...` indicates "the 484473th instance of d683 desert has an oasis prime at center-1, which is 262716..."  
`d683*968761-1 = 525332...` indicates "the 968761th instance of d683 desert has an oasis prime at center-1, which is 525332..."  
`d683*968763-1 = 525333...` indicates "the 968763th instance of d683 desert has an oasis prime at center-1, which is 525333..."  

The last line displays statistical information in curly braces.
`prime_oases d683 x484391 484391` shows the command and parameters executed.
"try" indicates the number of times primality was tested.
"hit" indicates the number of primes found.
(1.7%) shows the hit rate calculated as `hit/try*100.0`.

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
