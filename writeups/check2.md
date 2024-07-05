Checkpoint 2 Writeup
====================

My name: [your name here]

My SUNet ID: [your sunetid here]

I collaborated with: [list sunetids here]

I would like to thank/reward these classmates for their help: [list sunetids here]

This lab took me about [n] hours to do. I [did/did not] attend the lab session.

Describe Wrap32 and TCPReceiver structure and design. [Describe data
structures and approach taken. Describe alternative designs considered
or tested.  Describe benefits and weaknesses of your design compared
with alternatives -- perhaps in terms of simplicity/complexity, risk
of bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

Implementation Challenges:
[

0 ~ 499 ~ 999 = 1000
1000/2= 500

			1000,		1001,	1050,	1501,		2078,	11500,	10500
		c1	1,			1,		1,		1,			2,		11,		10	
w  z	c2	0,			1,		50,		501,		78,		500,	500
50,0---->	1050,		1050,	1050,	1050,		2050,	11050,	10050
499,0---->	1499,		1499,	1499,	1499,		2499,	11499,	10499
800,499-->	1301,		1301,	1301,	1301,		2301,	11301,	10301
800,500-->	1300,		1300,	1300,	1300,		2300,	11300,	10300
800,510-->	1290,		1290,	1290,	1290,		2290,	11290,	10290

500,0---->	500,		1500,	1500,	1500,		2500,	11500,	10500
501,0---->	501,		501,	1501,	1501,		2501,	11501,	10501
789,0---->	789,		789,	789,	1789,		1789,	11789,	10789

9,10-->		999,		999,	999,	1999,	    1999,	11999,	10999
498,499-->	999,		999,	999,	1999,	    1999,	11999,	10999
499,500-->	999,		999,	999,	1999,	    1999,	11999,	10999
20,500-->	520,		520,	1520,	1520,	    2520,	11520,	10520

]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

- Optional: I made an extra test I think will be helpful in catching bugs: [describe where to find]
