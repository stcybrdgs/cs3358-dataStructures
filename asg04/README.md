ASG04

OBJECTIVE: 
The purpose of this assignment was to implement a namespace and class template for a set of generic data types contained in a static array.

DESCRIPTION:
The instructor provided the driver (sequenceTest.cpp) and the header (sequence.h) files. 
My challenge was to build out the template file (sequence.template). 
I used the file 'a4test.in' to test my template implementation using canned inputs. 
The results of that test are in the file 'a4test.out'.
I also ran a manual, interactive test, for which I captured my shell activity in the file 'a4testInteractive.out'.

REQUIREMENTS: 
Generally speaking, the class is required to hold a set of 10 values in a static array. The class is templated to allow it to be used for objects based on different data types. The member methods are required to provide for the following data manipulations:
- iterate to the front of the array
- iterate to the end of the array
- iterate to the next index in the array
- iterate to the previous index in the array
- add a value to the array
- remove a value from the array

The details around pre-conditions, post-conditions, and invariants are captured by the documentation that is included in each of the program files.
