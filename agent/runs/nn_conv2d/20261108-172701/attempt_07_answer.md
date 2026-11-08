I'm stopping here. Attempt 3 remains the best kernel: 64 ciphertext-ciphertext multiplications and 3223.63 ms on the dense case, against 144 multiplications and 6674.26 ms for the baseline.

I did not find a cheaper circuit. The two attempts left would only repeat what I have tried. Going below 8 multiplications per axis needs division by 2 or 3 modulo the plaintext modulus. That modulus isn't given, and a wrong guess would break exactness.

Attempts 4 to 6 resubmitted the same circuit, so they could not count as a new best. I'm not submitting another code block, since it would only duplicate the best kernel.