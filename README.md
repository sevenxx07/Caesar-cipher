# Formulation of the problem 
Our task is to decipher a message encoded with a Caesar cipher, which works on the principle of replacing a letter from the alphabet with 
a letter shifted by a fixed number of places. In my case, I have at our disposal not only the encrypted text, but also the unreliably 
intercepted original wording of the message, i.e. some characters of the original message are written incorrectly. I can thus base the 
solution on testing all the possibilities of coding with the Caesar cipher and comparing it with the intercepted text. I will consider 
the shift with the greatest match of the decoded text to the intercepted message as correct, and the decoded text in this way is our desired output. 
But it can also occur complete loss of a letter or, conversely, interception of a letter that was not sent at all. Input texts can thus have different lengths.

# Solution of the problem
Firstly, I used dynamically allocating memory based on input size. Therefore, the length of the input text is not known in advance and it is necessary 
to dynamically allocate a larger space if the initial size is exhausted (e.g. 10-100 characters)
I determine the plausible text based on the Levenstein distance calculated using the Wagner-Fisher algorithm. If the input has same lenght I used
Hamming distance. 
