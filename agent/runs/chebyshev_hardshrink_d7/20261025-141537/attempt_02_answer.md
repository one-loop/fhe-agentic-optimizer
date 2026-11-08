The first attempt failed because `c7·T7` met an already-combined sum at the same level with a different multiplication history. This version adds `c5·T5` and `c7·T7` together first, since both come from the same pattern of multiplications. The lower-level terms `c1·z` and `c3·T3` are summed separately, and the two partial sums meet at different levels, where CHEHAB aligns them for free. The circuit is otherwise the same as before, with 5 ct-ct multiplications and a depth of 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext low = z * c1;
  low = low + t3 * c3;
  Ciphertext high = t5 * c5;
  high = high + t7 * c7;
  Ciphertext result = high + low;
  result.set_output("y");
}
```