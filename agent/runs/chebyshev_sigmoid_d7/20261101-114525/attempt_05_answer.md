Attempt 1 looks close to the floor for this function. The odd degree-7 polynomial is z·B(T2) with B cubic. Getting T2 costs one ciphertext multiplication, the final multiplication by z costs another, and the coefficients can't be combined in plaintext, so B needs two more: four in all. The 5-level form (attempt 2) needed two extra plaintext multiplications and measured slower.

So this attempt only changes how the constants are combined. It takes the attempt-4 recurrence and expands −g into −c5 − 2u. That replaces the ciphertext subtraction of g (level 3) from the level-5 product by plaintext additions plus a subtraction of u, which may change CHEHAB's alignment pattern. Multiplications stay at 4 ciphertext-ciphertext and 2 ciphertext-plaintext over 6 levels. I expect at most a small gain.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = z * z;
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7 T2
  Ciphertext g = u + u + c5;              // c5 + 2 c7 T2
  Ciphertext h = t2 * g;
  Ciphertext n = h + h + c3 - c7;         // c3 - c7 + 2 T2 g
  Ciphertext q = t2 + t2 - one;           // 2T2 - 1
  Ciphertext qn = q * n;
  Ciphertext uu = u + u;
  Ciphertext s = qn - uu + c1 - c5;       // c1 - g + q n
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```