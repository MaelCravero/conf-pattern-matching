Conclusion
==========

### What about TC?

- The chosen solution for TC's instruction selection is nested visits
- This is bad, but is it worse than MonoBURG? We do have one less dependance on
  an obscure program and learn useful C++ concepts
- Another section of the project uses `std::visit` too (without nested visits)
  and it is very clean

\pause
And since TC's goal is to learn C++... Is this bad solution really that bad?

Learning about the limits of a language is in my opinion as important as
learning what it's good at, and we're certainly reaching the limits there.

### Libraries are, in fact, a thing

- There are smarter and more skilled C++ developers than me
- We want to avoid external libraries in TC to learn standard "vanilla" C++, but
in real life libraries are what we would use
\pause
- One of the most well known is Michael Park's **Patterns** library which looks
like this:

```cpp
int fact(int n) {
  using namespace mpark::patterns;
  return match(n)(pattern(0) = [] { return 1; },
                  pattern(arg) = [](auto n) { return n * fact(n - 1); });
}
```

\pause
#### Check it out
- [https://github.com/mpark/patterns](https://github.com/mpark/patterns)
- On YouTube, [CppCon 2019: Michael Park “Pattern Matching: A Sneak
  Peek”](https://www.youtube.com/watch?v=PBZBG4nZXhk)

### One day maybe?

- This library has evolved to a feature proposition for C++23
- The last proposal is [P1371](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1371r3.pdf) which introduces nice concepts
- There has been a few attempts to add pattern-matching in C++17 and C++20, it
will most probably be added to the language at some point

\pause
So who knows, maybe one day we'll have what we need in C++...

\pause
In the meantime, I'll be coding in OCaml!
