The scale exponents tell the story. A term's exponent is the sum of its operands' exponents, plus one for the coefficient multiply. Terms that land at the same level therefore end up with different exponents.

The terms below are written as (level, exponent):

| Term | (level, exponent) |
|---|---|
| z·c1 | (2, 3) |
| t2·c2 | (3, 5) |
| t3·c3 | (4, 7) |
| t4·c4 | (4, 9) |
| t5·c5 | (5, 11) |
| t6·c6 | (5, 13) |
| t7·c7 | (5, 15) |

Multiplying a term by the plaintext `one` raises its level by one, which puts it at a level of its own. CHEHAB then aligns different levels for free. I push the later terms by 1, 1, 2 and 3 levels so every term meets the others at a distinct level. This costs extra plaintext multiplies at deep levels, which are cheap. It uses only 6 ciphertext multiplications, one fewer than attempt 2.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext w3 = z * t2;
  Ciphertext t3 = w3 + w3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext w5 = t2 * t3;
  Ciphertext t5 = w5 + w5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext w7 = t3 * t4;
  Ciphertext t7 = w7 + w7 - z;

  Ciphertext r1 = z * c1;
  Ciphertext r2 = t2 * c2;
  Ciphertext r3 = t3 * c3;
  Ciphertext r4 = (t4 * c4) * one;
  Ciphertext r5 = (t5 * c5) * one;
  Ciphertext r6 = ((t6 * c6) * one) * one;
  Ciphertext r7 = (((t7 * c7) * one) * one) * one;

  Ciphertext result = r1 + r2;
  result = result + r3;
  result = result + r4;
  result = result + r5;
  result = result + r6;
  result = result + r7;
  result = result + c0;
  result.set_output("y");
}
```