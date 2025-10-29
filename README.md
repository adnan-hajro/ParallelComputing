# Lab 5: Parallel Algorithms and Patterns

## Enhancing Scalability and Reproducibility in Computational Science

Parallel algorithms are well-defined, step-by-step computational procedures that emphasize concurrency to solve a problem. Parallel patterns are concurrent, separable fragments of code, such as reductions, that occur frequently in diverse scenarios. Recognizing and utilizing these patterns is crucial for successful parallelization efforts.

The main objective of studying these topics is to understand how **non-comparison-based parallel algorithms**, specifically **hashing techniques**, enhance performance and scalability for computational science tasks. This chapter focuses on:

1.  Utilizing hashing to break the algorithmic complexity limit of comparison-based algorithms, which is $O(N \log N)$.
2.  Exploring how **spatial hashing** achieves $\Theta(N)$ complexity for spatial operations, crucial for efficiency as problem size $N$ grows.
3.  Addressing the **finite-precision reproducibility issues** inherent in parallel execution of **global sum reduction**.

---

## I. Spatial Hashing for Highly Parallel Operations

Spatial hashing is a technique where the key is based on spatial information. The basic principle is mapping objects onto a regular grid of buckets to enforce locality. This approach is essential for managing complex computational meshes like **Cell-based Adaptive Mesh Refinement (AMR)**.

### The Scalability Advantage

By enforcing locality, spatial hashing prevents computational costs from increasing as the problem size $N$ grows. This allows spatial operations, such as **neighbor finding**, to be accomplished with an average algorithmic complexity of **$\Theta(N)$** for $N$ cells. This is a significant break from the limits of the naive $O(N^2)$ method or tree-based comparison algorithms, which are $O(N \log N)$.

The **simplicity** of the hash algorithm stands in sharp contrast to the complexity (often thousands of lines of code) required for alternative methods like the k-D tree. This simplicity facilitates easy porting to highly parallel hardware like GPUs, achieving speedups that can far exceed 3,000x compared to the original tree algorithms.

### Key Process: Neighbor Finding using Perfect Spatial Hash

The use of a **spatial perfect hash** guarantees that there are **no collisions** (at most one entry per bucket) because the hash table is sized based on the **finest cell level** of the AMR mesh. The neighbor finding operation relies on two main phases:

1.  **The Write Phase (Hash Table Setup):** This phase implements the spatial index structure. Each cell in the AMR mesh, regardless of its size, writes its cell number (`ic`) to all the underlying fine-level hash buckets that it covers.
2.  **The Read Phase (Neighbor Retrieval):** After the hash table is populated, this phase performs the spatial queries. The algorithm computes the index for a finer cell location one cell outside the current cell's boundary on each side and reads the cell index stored in that hash bucket.

---

## II. Parallel Global Sum and Reproducibility

The **global sum** is a type of **reduction** operation where an array of values (like total mass or energy) is reduced to a single scalar value. This operation presents a fundamental challenge in parallel computing related to **reproducibility**.

### The Problem of Non-Reproducibility

The core issue is that **finite-precision arithmetic is not associative**. Since parallel execution changes the order in which numbers are added (unlike serial execution, which always produces the same inexact result), the global sum will differ, leading to the **non-reproducibility of sums** across processors.

This problem is compounded by **catastrophic cancellation**, which happens when two nearly similar values of different signs are subtracted, resulting in a sum with only a few significant digits and noise filling the rest. Standard double summation is numerically unstable and generally inadequate for robust production applications.

