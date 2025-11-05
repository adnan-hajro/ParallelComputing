Lab 6: Vectorization: FLOPs for Free
---

## 1. LAB OBJECTIVE

The main objective of this laboratory is to understand how **vectorization** operates by utilizing specialized hardware—specifically the **Single Instruction, Multiple Data (SIMD) architecture**. This technique is crucial for operating on multiple data elements concurrently, thereby maximizing **Floating-Point Operations (FLOPs)** and achieving the peak performance capabilities of the hardware. Students will explore and practically implement various approaches for enabling vector parallelization in their code.

### Learning Outcomes

Upon completion of this lab, students will be able to experience:

*   **Vectorization Methods:** Exploring the different ways programmers can access vector parallelization with varying levels of effort and control.
*   **Optimized Libraries:** Recognizing how pre-optimized libraries (such as BLAS or LAPACK) provide vectorization performance with the least programming effort.
*   **Auto-vectorization:** Demonstrating how compilers automatically vectorize standard code, and how compiler flags and programming styles, such as using the **restrict** attribute, can improve the compiler's ability to safely vectorize code.
*   **Hints to the Compiler:** Utilizing pragmas and directives (like **OpenMP SIMD**) to explicitly guide the compiler to vectorize loops, particularly those that auto-vectorization misses due to complexities like conditional statements or dependencies.
*   **Vector Intrinsics:** Implementing low-level vector instructions to gain maximum control over the vectorization process for difficult or "troublesome" loops, while understanding the associated trade-off with code portability.

---

## 2. Vectorization Methods and Examples

This lab focuses on the implementation details for achieving vectorization beyond optimized libraries.

### 2.1 Auto-vectorization

Auto-vectorization is the vectorization of the source code by the compiler for standard C, C++, or Fortran languages.

The compiler uses a cost function to decide whether to vectorize. The compiler vectorizes if the cost function indicates the code would be faster, but this process involves guessing array lengths and assuming data is in the first level of the cache. Sometimes, the overhead of setting up vector instructions can be greater than the performance gain, potentially slowing down code if array lengths are small.

#### Example 1: Stream Triad Loop

In a simple vector loop (Stream Triad), the compiler often successfully vectorizes the operations. The compiler provides feedback indicating the vector size used (e.g., "loop vectorized using 16 byte vectors," which corresponds to a 128 bit vector).

**Controlling Vector Width:**
The vector length used by the compiler may not reflect the full capability of the hardware, especially with older compilers that struggle to recognize newer CPU architectures. Hardware vectorization capability can be checked using the `likwid` tool (e.g., running `likwid-perfctr`).

To force the GCC compiler to generate instructions for a wider vector length (e.g., 256 bits or 32 bytes), the flag `-mprefer-vector-width=256` can be used during compilation. This ensures the CPU's full vector processing capability is utilized.

#### Example 2: Auto-vectorization in a Function (Aliasing)

When loops are encapsulated within functions, the compiler may face an issue called **aliasing**.

**Aliasing Definition:** Aliasing occurs where pointers point to overlapping regions of memory. Since the compiler cannot confirm if the memory regions are distinct, it is unsafe to generate vectorized code or other optimizations.

To mitigate this, the compiler sometimes creates multiple versions of the loop and adds code that tests the arguments at runtime to determine which version to use, which introduces overhead.

**Resolution using `restrict`:** Programmers can fix this by adding the **`restrict` attribute** to the function definition's pointer arguments. This C99 keyword (or the `__restrict` attribute in GCC/Clang/Visual C++) informs the compiler that the pointers do not overlap, thereby removing aliasing vulnerabilities and enabling cleaner vectorization.

### 2.2 Hints to the Compiler: Pragmas and Directives

When compilers fail to auto-vectorize complex code (often due to loop-carried dependencies where one loop iteration needs access to a variable modified in the previous iteration), the programmer must provide hints.

**Pragma Definition:** A Pragma is an instruction to a C or C++ compiler to help it interpret the source code, usually implemented as a preprocessor statement starting with `#pragma`.

One powerful method is using **OpenMP SIMD** pragmas (`#pragma omp simd`) to tell the compiler that it is safe to vectorize the subsequent loop.

#### Example: Minimum Reduction Loop

For a loop containing a reduction operation (like finding the global minimum), the compiler needs explicit guidance via a **reduction clause**.

1.  The pragma instructs the compiler that the loop is safe to vectorize: `#pragma omp simd reduction(min:mymindt)`.
2.  Each vector lane (e.g., 4 lanes for a 256-bit vector of doubles) receives its own private copy of the reduction variable (`mymindt`).
3.  Each lane processes its simultaneous iterations and updates its private minimum value.
4.  At the end of the loop, the `min` operation specified in the reduction clause is used to combine the four private results into a single final value.

### 2.3 Vector Intrinsics

Vector intrinsics are the last resort for loops that remain "troublesome" even after applying compiler flags and pragmas. This technique offers maximum control over instruction generation but sacrifices code portability, as the instructions are specific to the target architecture (e.g., Intel x86).