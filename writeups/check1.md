Checkpoint 1 Writeup
====================

My name: [your name here]

My SUNet ID: [your sunetid here]

I collaborated with: [list sunetids here]

I would like to thank/reward these classmates for their help: [list sunetids here]

This lab took me about [100] hours to do. I [did/did not] attend the lab session.

I was surprised by or edified to learn that: [
    - I complete my code with all cases of test;
]

Describe Reassembler structure and design. [Describe data structures and
approach taken. Describe alternative designs considered or tested.
Describe benefits and weaknesses of your design compared with
alternatives -- perhaps in terms of simplicity/complexity, risk of
bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

Implementation Challenges:
[
    Structure:
    - An ordered set of indexes named "index_" which holds all first indexes of strings;
    - An unordered map of data named "dataMap_" which stores any strings not put into the stream. The key is first index of a string and the value is the string itself;
    - A count flag named "pendingCount" which holds the count of the map;
    - A flag named "nextIndex" which is the first index of the string that should be pushed into the stream;

    Design:
    - First, a string goes into the reassembler, then it should be written into the Bytestream if its first index is less then or equal to "nextIndex"; otherwise, it is stored in dataMap_;
    - Second, strings with same substring will be merged, or they will be combined if they are close to each other; 
    - Finally, checking merged index_ and writing into stream when its first index less or equal then nexIndex;
    - We can guarantee that index_ strictly matches dataMap_ and the size of dataMap_ is "pendingCount", also strictly increasing nextIndex;

    A Bug Handling Processing:
    - Problem Description:
        1. I encountered an error when running a test case named "reassembler_win" in the shell, and the command is "cmake --build build --target check1".
	    2. This case involves a loop that splits a string into substrings and delivers them unordered.
	    3. The error seems to be due to unequal strings that occur at some time when read all strings are pushed stream, and I am not sure how to resolve it.
	    4. I have only a little knowledge about the test case.
    - Handling Process:
	    1. I debugged and studied this test case and found the error spot.
	    2. The unequal strings may be due to inconsistent encoding.
	    3. Why not try logging some important processes?
	    4. I analyzed the logs but was unsure how to pinpoint the issue.
	    5. I added more detailed processing logs: sum of bytes, process stream, the spot which the program returns, and the merge process.
    - Result:
	    1. I discovered that the program returns at the end of the loop.
	    2. The string should be written into the stream when there are no other strings before it.
	    3. DataMap_ should be empty when breaking the loop.
        4. Finally, I analyzed the logs and found that merging substrings caused this bug. 
        5. The reason is that I incorrectly calculated the count for the "substr" string's function, which has two parameters: the first one is the beginning index, and the second one is count to the end of substring.I realized that I did not calculate the count correctly and just used the one-parameter version of the substr function, where the end is the end of string;
]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [
    - A test case which named "reassembler_win" do not run seccess. This bug take my many time to find reson.
]

- Optional: I think you could make this lab better by: [
    - Reducing unnessery loop;
]

- Optional: I'm not sure about: [describe]
