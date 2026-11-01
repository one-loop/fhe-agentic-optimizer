This is the last attempt, and it is a long shot that I expect to time about the same as the best. The arithmetic is unchanged: 3 ciphertext multiplications, 3 plaintext multiplications, depth 5.

The one structural change is how the doubled operand of the middle multiplication is built. Before, it was 2u, made from u at level 2. Now it is v = 4w − 2, built straight from w = z² with additions and two subtractions of `one`. That frees the middle product v·h from depending on u, which changes where CHEHAB has to align operands.

The odd term c1·z and the −3c6·u correction are still combined at a shallow level before being added to the deep product.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext w2 = w + w;
  Ciphertext u = w2 - one;             // T2
  Ciphertext v = (w2 + w2) - one - one; // 2u = 4w - 2
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext mid = v * h + c2;         // 4 c6 u^2 + 2 c4 u + c2
  Ciphertext e = u * mid;              // 4 c6 u^3 + 2 c4 u^2 + c2 u
  Ciphertext lin = z * c1;
  Ciphertext r = lin - (s + s + s);    // c1 z - 3 c6 u
  Ciphertext result = (e + r) + c0 - c4;
  result.set_output("y");
}
```