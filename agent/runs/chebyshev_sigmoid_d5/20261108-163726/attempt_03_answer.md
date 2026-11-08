This version builds W = 4w by doubling w with two additions instead of computing x·z, because the x·z product needed a level alignment and was slower. The terms are then regrouped to cut additions. With t = v − 5u:

- r = t + u·W
- low = s − 2v − t

I haven't run it, so the timing and accuracy are untested.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext w2 = w + w;
  Ciphertext W = w2 + w2;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext uW = u * W;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext t = v - u5;
  Ciphertext r = t + uW;

  Ciphertext m = W * r;

  Ciphertext v2 = v + v;
  Ciphertext low = (s - v2) - t;

  Ciphertext result = low + m;
  result = result + c0;
  result.set_output("y");
}
```