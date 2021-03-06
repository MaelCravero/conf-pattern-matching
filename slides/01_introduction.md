Introduction
============

### Hey Vsauce

- Mael Cravero
- ACDC, YAKA, ACU
- Tiger Maintainer
- Strong interest in **static analysis**, **compilation** and **programming
  languages** in general

\vfill
\pause

#### Warning

We will talk about OCaml.
\pause
I like OCaml.

### A bit of context

- About a year ago, I was working on Tiger Compiler a lot
- TC's instruction selector was **old** (last meaningful modification was 2005)
- **Modern C++** is way different from 2005 C++

How can this section of the project be modernized?

### Instruction selection in a nutshell

![Instruction selection as depicted by *Modern Compiler Implementation in
ML*](img/instruction_selection.png){height=100%}

### The main idea

- Instruction selection is basically **pattern-matching**...
\pause
- ...but there is no pattern-matching in C++...
\pause
- ...so we used an external program (MonoBURG) to generate nested switches

\vfill
\pause

#### Goal

**Replace** non-idiomatic pattern-matching generated by MonoBURG with
**idiomatic** modern C++ using `std::visit` and `std::variant`.


### Goals of this presentation

1. C++ and compilation basics
\pause
2. Pattern-matching concepts
\pause
3. Emulate basic pattern-matching in C++
\pause
4. Static vs dynamic polymorphism
\pause
5. Try to do smart things and fail miserably
