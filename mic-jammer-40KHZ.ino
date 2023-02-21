/*
 * Antispy ultrasonic device for preventing voice recording 
 * with ultrasonic waves generation based on ARDUINO DIGISPARK,
 * connections : 
 * PAM8403 module AUDIO LEFT INPUT connected to Port 2 of DIGISPARK
 * PAM8403 module AUDIO RIGHT INPUT connected to Port 1 of DIGISPARK
 * PAM8403 module AUDIO GROUND INPUT connected to Port 0 of DIGISPARK
 * 10 piezzo-electric ultrasonic transducers connected in parallel to PAM8403 OUTPUT LEFT
 * 10 piezzo-electric ultrasonic transducers connected in parallel to PAM8403 OUTPUT RIGHT
 * THIS IS 40 KHZ VERSION !!!
 * (C) Adam Loboda 2021, adam.loboda@wp.pl
 */

#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// PIN - PORT ASSIGNMENT DEFINITION FOR OUTPUT
#define  OUTPUTPORT        PORTB
#define  OUTPUTPORTCTRL    DDRB

// created in Excell / Libreoffice - will randomly 
// swing pulse width to achieve noise effect around the center 40 KHz frequrency
const uint8_t  randomized[] PROGMEM = 
  {
9,36,31,23,9,11,18,15,34,23,34,42,40,39,25,16,39,10,44,34,13,37,15,14,26,
10,17,39,37,22,8,35,23,21,40,17,20,9,23,35,38,20,26,40,22,25,7,36,15,28,
29,15,9,32,7,39,20,35,24,12,18,36,14,38,42,42,24,34,16,25,36,12,12,44,44,
27,32,30,38,38,15,37,30,12,29,27,7,15,26,27,21,14,19,27,42,15,24,10,26,28,
33,14,22,21,21,37,14,25,16,38,39,25,37,28,38,27,43,38,7,44,17,12,29,18,28,
8,17,14,38,39,29,39,43,17,16,29,10,26,11,27,35,16,8,13,44,17,36,14,22,12,
22,25,23,27,25,20,11,35,11,20,21,37,18,23,39,18,33,25,9,39,35,8,36,24,26,
35,37,28,9,31,42,10,10,29,24,30,43,41,9,18,38,25,11,10,7,8,35,20,26,15,12,
22,16,21,17,33,11,30,27,44,34,31,14,23,26,37,18,26,15,23,32,8,39,39,27,28,
43,23,14,40,44,36,25,13,26,32,18,24,10,41,43,38,24,25,44,15,24,33,15,40,31,
34,29,9,20,10,37,16,15,38,22,36,37,31,16,26,17,29,23,12,42,44,19,16,8,18,
23,38,35,27,33,8,8,26,38,37,40,24,19,9,22,15,26,10,9,19,25,42,7,20,43,35,
12,12,24,23,22,36,7,20,20,8,42,30,10,43,16,20,24,23,7,41,16,40,41,31,44,24,
32,24,32,10,44,14,16,27,35,10,14,15,10,20,42,9,11,18,8,24,13,30,9,31,16,41,
32,38,16,29,10,44,40,14,13,38,12,32,36,32,32,29,24,31,30,37,22,23,19,41,43,
33,37,31,36,34,20,20,10,19,18,30,12,41,14,35,33,34,17,13,34,19,21,39,29,27,
22,28,7,27,14,33,34,43,7,11,9,11,33,12,9,26,43,9,35,22,21,15,19,43,14,19,
15,28,16,13,38,34,38,30,15,42,17,14,18,13,26,35,23,29,31,37,11,23,15,20,10,
18,41,21,9,35,28,16,44,25,17,19,9,37,11,7,9,21,28,20,24,28,26,41,15,30,19,9,
34,43,42,22,44,43,44,8,32,31,12,22,10,13,41,11,9,35,25,14,7,29,44,44,25,10,
42,19,16,13,17,39,33,10,7,7,27,17,25,21,11,14,39,44,18,44,21,23,30,8,31,24,
44,28,38,9,22,33,43,39,11,21,13,11,41,8,24,26,29,39,33,39,28,17,25,39,37,38,
8,42,43,13,30,35,37,13,21,32,36,17,19,10,30,35,27,33,40,38,35,23,23,13,20,
34,18,7,18,19,36,7,32,13,22,13,44,37,31,15,11,11,7,14,20,37,18,10,10,15,14,
8,34,21,40,30,40,13,19,24,12,33,12,24,9,19,9,11,13,11,15,29,28,28,20,15,29,
20,42,40,31,44,19,43,38,43,37,35,42,43,24,25,41,30,29,33,43,24,12,22,14,18,
38,13,8,16,31,8,27,26,18,23,27,25,20,41,10,42,27,24,28,17,11,13,17,16,21,28,
39,32,17,16,22,31,29,24,26,23,21,43,42,40,38,30,22,32,27,24,34,30,15,34,30,
22,12,22,44,44,8,7,10,44,20,39,35,9,8,10,31,19,33,27,39,43,13,19,12,23,16,42,
25,44,33,22,29,35,32,36,26,16,26,39,23,41,40,39,19,38,10,24,10,13,16,25,43,29,
19,42,20,23,11,27,29,43,28,30,33,12,44,8,34,16,20,13,28,17,26,8,39,31,7,28,10,
25,28,14,33,38,24,44,30,30,35,26,38,44,15,44,22,8,20,28,38,24,36,21,41,16,15,
25,27,27,36,43,41,33,35,26,16,7,37,38,19,38,25,41,40,18,30,19,7,42,12,36,9,30,
9,32,14,40,19,13,18,28,34,41,37,22,12,24,36,29,37,34,10,39,23,19,38,27,23,33,
13,32,20,12,20,22,13,44,26,26,23,18,24,23,18,40,25,31,12,11,27,24,28,33,12,20,
22,14,36,36,34,9,14,41,20,22,13,41,23,42,16,9,10,28,24,36,21,21,31,24,22,39,
15,35,16,43,22,37,18,28,36,21,34,42,16,17,41,28,36,27,24,13,38,38,30,12,20,32,
20,44,14,26,33,28,32,37,41,11,30,33,18,29,18,11,16,8,38,7,13,30,10,16,38,33,
29,28,16,21,14,42,27,32,25,18,15,16,29,8,24,12,24,24,23,10,37,35,12,10,35,29,
30,8,13,43,22,11,14,36,20,28,11,41,15,38,39,15,34,20,20,15,28,38,36,33,36,41,
21,14,41,34,25,13,33,31,12,33,22,39,12,22,34,15,18,29,12,37,9,37,19,15,7,24,
10,13,19,27,18,43,39,9,30,19,40,15,30,39,20,44,11,44,20,22,23,18,31,20,38,20,
22,29,17,36,25,17,13,32,15,32,7,38,40,11,37,7,43,33,19,30,33,32,27,12,40,13,
32,42,41,19,20,11,20,21,18,39,32,38,19,14,25,11,26,22,7,16,30,38,27,41,12,17,
24,41,27,33,20,43,27,39,34,42,38,42,39,29,30,24,43,32,34,33,28,41,20,30,14,21,
18,34,22,19,32,35,30,35,26,19,11,24,31,44,22,30,24,8,28,30,44,21,12,32,29,31,
25,42,8,36,24,11,10,35,43,36,14,18,35,9,36,18,20,44,18,29,13,17,42,22,15,37,36,
36,41,10,28,43,31,7,40,19,24,11,7,44,38,30,23,8,17,14,36,44,39,32,43,8,35,17,
25,15,18,35,26,9,31,37,7,13,31,26,10,38,23,30,23,44,41,16,41,30,38,44,35,17,44,
37,10,26,21,26,14,20,11,13,21,27,32,16,36,18,31,8,32,16,44,32,32,39,24,22,33,
26,20,8,26,19,21,34,19,39,11,34,32,29,18,16,8,25,16,16,36,38,31,26,13,32,32,39,
37,19,7,37,38,41,26,17,13,41,7,43,28,32,18,8,10,44,22,31,39,8,28,12,37,7,13,19,
22,44,11,16,40,33,38,39,32,44,20,26,12,43,42,16,21,30,13,27,14,12,17,37,35,42,
39,18,22,37,37,7,9,12,9,14,41,26,37,37,33,18,20,40,16,40,36,39,12,14,23,18,22,
21,30,42,8,9,27,13,30,8,29,8,34,17,32,20,17,20,30,33,13,29,33,25,42,20,8,24,40,
9,9,25,26,20,16,30,24,27,8,15,35,34,40,18,37,27,33,11,41,26,7,32,43,30,27,19,35,
31,16,37,32,30,44,41,41,19,17,9,15,19,36,43,28,9,42,7,13,32,16,15,33,25,15,24,
42,15,12,22,43,35,32,37,32,18,32,35,10,37,34,44,40,11,29,39,38,9,20,31,36,38,12,
18,22,36,28,23,19,30,14,28,14,40,14,41,25,38,17,16,40,25,10,19,40,35,38,16,16,43,
7,28,16,40,31,38,13,31,19,11,36,7,21,39,15,30,25,27,30,36,34,18,19,21,34,24,11,41,
31,19,33,29,31,34,30,14,23,8,14,35,32,41,22,22,41,34,24,34,17,7,19,30,9,8,25,28,
19,33,11,20,27,37,20,38,12,25,9,38,41,16,18,32,23,13,22,14,24,24,18,30,25,32,10,
39,33,22,8,31,22,9,23,27,41,7,40,13,24,28,13,33,42,42,29,9,40,21,43,21,36,12,32,
33,15,7,17,23,25,7,40,40,12,28,42,23,11,22,8,30,21,30,19,33,16,9,21,35,30,21,13,
26,36,35,33,28,40,11,19,11,44,32,13,37,10,33,7,35,8,29,39,16,27,12,43,17,39,9,14,
25,32,16,26,36,12,40,16,7,38,43,18,8,28,38,33,18,20,36,13,13,19,9,35,15,20,29,9,
11,30,19,24,9,36,30,17,28,13,40,24,30,20,30,38,30,25,26,25,27,16,21,34,33,38,44,
7,44,17,28,25,42,21,19,7,29,27,10,20,12,23,27,36,32,15,26,27,17,15,25,42,17,11,7,
37,16,21,14,16,24,9,32,41,11,18,38,32,33,29,18,19,25,8,34,23,16,43,13,20,14,43,42,
7,11,38,9,33,37,16,21,24,32,21,31,27,7,17,12,16,15,38,34,18,15,25,41,26,33,35,8,
31,41,21,21,14,22,25,19,22,31,12,40,18,30,35,16,14,42,12,25,20,28,10,12,23,32,30,
27,25,10,43,26,33,12,10,27,30,29,21,38,12,19,8,37,16,26,44,29,43,33,19,32,43,43,
12,9,27,13,21,25,9,37,24,17,29,11,44,11,30,42,31,27,30,38,21,34,40,10,19,19,15,38,
30,40,25,10,31,32,9,38,37,33,37,25,8,25,17,18,39,29,14,9,14,20,14,13,7,37,28,22,39,
18,16,29,7,32,13,32,24,38,8,43,13,30,19,34,15,9,12,39,19,8,24,22,12,14,27,17,15,27,
16,23,11,12,34,14,15,16,43,27,24,24,15,29,9,36,33,40,41,21,13,28,11,36,34,9,20,44,
12,25,39,16,8,26,14,33,22,35,15,27,33,23,39,33,43,41,22,32,41,41,27,7,31,14,14,18,
32,9,26,30,44,27,35,22,35,9,9,10,29,16,33,18,42,40,7,42,35,20,21,11,24,24,44,14,9,
20,12,31,37,18,21,29,33,29,40,15,12,38,19,36,7,26,17,36,24,39,36,44,44,23,14,40,21,
23,7,39,16,13,26,20,31,33,30,16,31,29,42,24,24,7,29,26,42,24,37,13,35,16,35,18,10,
30,38,12,26,21,37,25,9,19,23,22,33,23,10,27,43,32,27,27,41,7,21,21,31,30,29,16,12,
29,31,25,15,25,19,11,36,39,20,8,26,38,37,43,18,31,9,26,36,8,13,17,29,17,36,25,13,12,
35,41,20,20,23,42,32,26,31,9,24,34,8,16,9,38,11,28,26,36,38,30,11,42,25,13,17,9,24,
12,36,19,37,35,29,39,11,30,41,10,41,11,17,15,43,31,23,43,38,31,35,10,8,11,20,37,20,
19,36,14,31,35,18,29,40,15,37,37,36,8,24,42,39,11,15,23,18,40,21,39,39,39,42,29,11,
24,17,28,11,41,23,39,30,39,16,26,8,34,33,33,44,9,12,21,32,41,8,25,25,21,23,34,43,25,
22,7,14,18,30,28,44,41,39,31,27,10,32,35,8,27,22,36,28,41,19,19,19,43,32,7,16,21,17,
15,38,16,28,10,20,8,28,16,29,28,15,40,7,26,28,43,18,35,8,23,41,13,41,38,34,35,17,30,
33,44,28,32,41,21,20,18,37,11,40,26,12,14,24,42,40,34,42,9,15,32,38,30,28,33,38,15,
36,7,12,29,14,36,19,27,22,14,38,30,42,11,9,7,43,9,31,31,40,33,27,25,24,23,31,23,19,
32,29,20,29,29,30,17,39,21,23,14,35,30,36,10,41,27,35,31,18,43,43,12,38,43,7,35,34,8,
17,43,28,43,44,39,36,22,21,43,32,18,32,22,25,21,28,27,12,33,18,32,39,26,42,24,39,42,
14,23,34,37,21,35,40,13,34,17,32,7,42,28,20,14,11,13,22,26,13,8,7,10,7,33,7,40,13,32,
19,16,30,25,27,11,27,34,21,40,8,7,32,32,22,38,23,30,27,7,37,31,32,11,24,30,10,23,15,
38,44,33,35,36,11,31,27,7,37,30,37,8,29,22,27,21,39,31,29,27,25,30,13,21,42,34,16,28,
13,30,28,22,37,33,12,9,21,42,38,30,22,17,21,7,12,21,21,15,14,25,30,18,19,15,44,37,28,
32,43,12,37,36,32,24,42,19,40,42,26,12,37,34,44,30,39,15,11,39,7,26,7,42,26,10,25,40,
32,26,14,35,32,29,14,26,23,23,23,21,34,21,23,9,10,13,34,23,25,19,15,10,8,42,34,8,22,
34,35,21,34,32,30,43,8,38,8,15,23,43,31,25,44,38,37,44,29,44,38,12,24,20,11,36,41,10,
36,38,28,7,23,26,21,27,42,29,29,23,24,11,19,28,23,42,21,22,26,31,40,29,23,31,26,11,13,
39,28,19,10,39,16,28,13,24,21,12,20,42,11,36,18,15,26,37,23,22,23,40,32,32,11,13,20,29,
29,8,35,31,32,27,41,12,25,28,28,20,35,32,16,42,33,22,13,19,29,25,23,31,17,40,21,28,16,
10,20,19,12,20,44,16,35,44,22,39,26,42,10,42,36,37,44,38,39,24,7,32,37,34,24,7,26,8,
17,37,39,33,7,14,8,28,18,27,23,20,16,36,29,35,9,33,14,18,21,39,25,13,26,22,28,37,39,42,
39,19,15,14,15,26,32,19,29,33,41,13,32,17,23,7,40,18,42,10,39,15,28,24,11,28,29,21,21,
31,40,31,20,43,9,36,38,25,19,30,32,37,39,30,24,41,27,11,40,19,15,18,31,35,13,27,32,22,
10,15,19,40,35,19,32,21,43,24,32,38,31,35,16,25,13,16,39,17,12,25,41,27,23,16,30,19,37,
12,41,28,28,34,23,9,21,14,28,39,40,10,20,36,33,37,15,34,42,27,19,8,12,40,34,22,39,44,21,
44,38,26,12,37,9,33,8,40,13,8,28,18,36,28,36,39,10,41,37,18,13,40,41,15,16,15,10,17,44,
38,36,43,29,9,21,31,37,33,28,38,7,43,24,16,39,9,15,12,34,41,31,37,23,23,29,38,43,13,12,
42,38,16,34,7,30,42,41,11,18,15,19,43,15,26,18,44,34,31,27,30,18,31,33,8,23,33,25,31,29,
18,22,40,10,32,21,29,21,34,19,8,14,35,18,33,27,36,33,44,38,15,41,8,24,42,18,13,30,25,22,
31,29,37,40,43,32,21,19,8,18,29,31,21,36,26,9,42,34,30,30,36,42,17,30,31,30,42,13,17,9,
29,23,12,33,7,43,44,35,41,20,11,15,38,42,23,36,21,32,38,10,25,12,14,15,33,44,20,13,32,14,
32,41,44,7,10,39,29,29,37,24,7,7,27,35,25,11,37,10,8,32,20,38,37,41,33,24,11,24,38,32,28,
22,10,9,24,43,25,14,10,44,12,17,34,12,30,36,37,43,29,44,28,15,26,17,31,41,31,22,7,43,25,
42,19,40,42,41,25,9,42,42,10,25,28,38,17,9,30,28,27,41,15,12,17,40,13,40,44,16,12,21,33,
44,35,39,14,17,9,33,7,39,43,9,11,31,24,42,20,11,30,23,28,8,23,14,22,33,7,31,40,7,18,44,15,
24,40,8,40,18,19,37,19,21,34,44,24,20,9,36,29,42,11,44,22,13,9,16,23,31,44,35,8,39,38,20,
15,20,29,42,32,8,17,27,12,26,31,38,22,19,27,38,24,31,16,22,12,9,20,24,15,20,12,15,25,43,
17,13,8,25,35,20,39,9,36,41,14,24,31,42,28,15,16,41,39,12,32,12,40,28,32,41,29,39,10,18,
12,44,13,25,23,9,43,25,28,24,17,32,42,21,41,9,15,36,40,24,31,16,11,27,28,8,24,40,36,10,42,
15,9,13,24,35,30,44,25,35,36,10,44,23,40,39,16,14,29,23,43,10,17,28,12,41,14,38,30,39,35,
12,18,26,21,14,9,24,17,13,35,44,30,39,14,25,9,23,20,33,9,33,34,42,29,43,8,22,7,36,32,22,44,
34,39,16,33,24,39,36,20,18,17,28,14,8,38,10,8,33,38,34,8,34,38,34,24,16,19,15,28,27,25,40,
26,9,14,43,35,9,40,35,41,8,7,12,36,11,33,33,18,35,28,12,37,40,17,33,24,10,9,18,15,7,10,44,
43,24,32,37,10,28,10,24,23,44,27,12,43,36,38,33,13,9,41,27,14,28,31,29,20,36,21,26,19,17,
22,13,34,36,42,41,28,13,22,35,19,14,29,21,32,9,17,44,26,10,32,37,42,28,19,32,34,22,42,9,34,
27,7,11,14,29,17,25,18,32,41,31,39,15,18,18,26,31,7,32,41,34,29,25,12,22,37,16,10,26,26,44,
43,34,15,35,24,26,23,22,25,34,11,13,16,11,44,7,37,15,35,12,25,25,27,13,16,11,36,13,19,44,
30,22,10,24,41,18,18,38,33,32,37,35,43,9,33,9,15,15,8,12,18,10,25,9,27,41,11,37,19,34,30,
7,28,27,31,32,44,40,19,28,32,24,38,21,44,39,38,26,14,13,28,23,29,32,17,22,24,17,21,20,12,
18,27,36,13,20,18,34,20,34,21,8,35,8,26,23,22,12,22,38,14,34,40,18,9,37,40,42,24,20,16,41,
43,33,13,24,33,32,33,12,21,42,41,23,25,30,26,19,20,27,28,23,30,36,29,28,21,32,13,39,35,21,
24,39,38,11,44,20,36,44,42,27,26,20,35,42,43,22,14,30,13,35,22,43,34,23,44,27,28,34,24,41,
40,17,14,40,41,29,25,8,14,25,12,42,11,35,27,7,36,38,18,34,39,14,16,44,31,43,14,44,8,31,14,
28,8,24,25,15,20,21,42,36,13,43,10,26,25,29,20,28,14,9,8,39,44,39,30,23,28,10,44,24,18,23,
20,41,23,38,20,21,10,33,37,29,13,35,9,25,38,24,12,26,37,30,43,35,13,17,24,26,37,19,13,38,
41,19,36,13,34,42,9,9,43,13,40,38,7,9,12,8,37,32,24,12,7,41,44,37,19,17,7,13,15,12,20,43,
13,22,34,33,9,7,37,25,37,35,7,44,16,27,11,37,28,19,9,21,21,8,42,11,40,36,30,13,14,21,12,36,
21,20,8,14,12,40,27,37,42,17,44,33,27,37,22,36,14,44,8,23,15,27,8,27,44,20,9,37,41,24,40,30,
22,18,14,21,34,37,11,23,19,24,13,28,43,20,8,28,40,28,25,28,13,14,44,41,17,34,18,39,31,37,13,
25,16,18,41,35,39,7,29,11,40,7,29,39,21,18,34,23,43,13,35,32,13,8,24,17,41,41,12,14,26,17,31,
35,23,30,34,44,13,7,37,17,16,35,36,11,21,12,13,40,12,40,40,16,13,28,9,11,35,10,17,37,10,31,
41,22,16,31,24,25,8,29,17,11,38,37,28,12,19,19,38,25,42,10,38,27,37,25,21,21,43,28,7,29,25,22,
16,18,13,17,15,18,41,31,30,41,24,18,42,29,39,44,26,12,12,43,17,36,41,13,20
};


void setup() {

        // SET OUTPUT PIN ON ALL OUTPUT PORTS      
        OUTPUTPORTCTRL = 0b00011111;  //set all 5 LOWER bit pins as output
        OUTPUTPORT     = 0b00000000;  //set all 5 LOWER bit pins to ZERO

}

void loop() {
          // initializing variables for loops
          uint16_t sequence;
          uint8_t pulsewidth, i, bulk;

          // generating WAVE on BIT0-BIT5 DAC outputs
          // random values are taken from PROGMEM lookup table
          // are used to achieve random PWM effect
          
          for(sequence=0; sequence<4096; sequence++)
           {
           // take pulse width from lookup table 
           pulsewidth = pgm_read_byte(randomized + sequence);

             // sending a bulk of pulses with defined pulsewidth 
             // you may play with number of pulses for better effect of jamming
             // (value 40 means every 1/1000 seconds there will be change
             // in the signal shape )
             for(bulk=0; bulk<40; bulk++)
             {
              // send HIGH VOLTAGE - this time square wave
              // PB0 may serve as GND for PAM8403 module audio input
              OUTPUTPORT = 0b00000110; 
                                 
              // now delay to achieve desired frequency
              for(i=0; i<pulsewidth; i++)
                  {
                        asm volatile (
                                        "    nop  \n" 
                                     );
                  };
              //
              // send LOW VOLTAGE - this time square wave
              OUTPUTPORT = 0;               
              // now delay to achieve desired frequency
              // this value 53 determines center frequency of transducers 
              // which is 40KHZ in this version of the code
              for(i=0; i<(53-pulsewidth); i++)
                  {
                        asm volatile (
                                        "  nop  \n" 
                                     );
                  };
            // End of 'bulk' loop
            };
            
           // end of 'sequence' loop;
          };
          
// end of LOOP
}