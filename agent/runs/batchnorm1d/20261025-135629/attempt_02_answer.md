Idea: avoid the plaintext-vector add (encoding, ~2.5 ms) and the separate multiply. Without changing the function, I can't remove B, since only x is a ciphertext. But I can try to make the add happen before the multiply, so that it is applied at the first level, where the encoding is the same cost, and see whether CHEHAB produces a different circuit: y = (x + B/A) * A. This needs B/A, which is not available as an input (no plaintext arithmetic).

An alternative is to use a ciphertext-ciphertext trick, but it costs more (30 ms).

The only legal structural change is to put the add into the multiplication stage. Here I compute x*a and add b through a different route: (x*a - (-b)). The negation of a plaintext is plaintext arithmetic, which is not allowed. So I'll keep the subtraction form on the ciphertext side: y = -((-(x*a)) - b). This uses negations, which are cheap and may change the circuit's operation counts. It is unlikely to be faster, but it is a valid variation.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext u = -t;
  Ciphertext v = u - b;
  Ciphertext y = -v;
  y.set_output("y");
}
```