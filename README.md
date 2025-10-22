# IT 2004 - Parallel Programming Lab: Data Design and Performance Models

## Introduction and Objectives

The objective of this laboratory session was to **understand how memory layout affects program performance** through practical comparison of different data allocation strategies in C/C++. We explored how various implementations of 2D arrays and structures interact critically with the processor's **cache hierarchy**.

Key concepts explored include:

*   **Fragmented vs. Contiguous Memory Allocation:** Understanding the measurable performance differences derived from memory structure.
*   **Cache Behavior Analysis:** Learning how the processor loads data using **64-byte cache lines** and observing how fragmentation wastes cache capacity.
*   **Performance Measurement:** Using timing functions to connect theoretical cache concepts to measurable execution times.

## Laboratory Activities and Examples

The session focused on analyzing four specific code examples, demonstrating how different data layouts impact performance by stressing the cache and memory bandwidth.

### Example 1: Fragmented Matrix Allocation

This example demonstrated a common, but often performance-limiting, method of allocating 2D arrays in C.

| Technical Element | Description |
| :--- | :--- |
| **Allocation Method** | Each row of the matrix is allocated separately using `malloc()`, resulting in a **fragmented 2D structure** in memory. This mimics an Array of Pointers structure. |
| **Memory Access** | The structure of the code uses a loop that repeatedly selects **random indices ($j, i$)** to accumulate a sum. This simulates irregular access that deliberately stresses cache locality and reveals performance degradation compared to contiguous storage. |
| **Performance Stress** | The program uses `flush_cache()` by touching a large dummy memory block (100 MB) to ensure the CPU caches are in a cold state, thus guaranteeing that timing reflects real memory-access costs rather than cached data. |

### Example 2: Contiguous Matrix Allocation

This example showed the highly optimized method for allocating 2D arrays, ensuring maximum spatial locality.

| Technical Element | Description |
| :--- | :--- |
| **Allocation Method** | A single `malloc()` call reserves **one large memory block** for all matrix elements. Row pointers are then adjusted sequentially (`x[j] = x[j-1] + imax;`) to map into this single contiguous block. |
| **Cache Benefit** | This approach ensures **spatial locality**—elements accessed sequentially in the array are also adjacent in physical memory. This maximizes the utilization of the 64-byte cache line when data is loaded, leading to efficient numerical workloads. |

### Example 3: Array of Structures (AoS)

This example focused on the performance implications of grouping related data elements into structures, specifically using an array of `Particle` structures.

| Technical Element | Description |
| :--- | :--- |
| **Data Structure** | Defines and dynamically allocates an array of `Particle` structures. The structure size often occupies significantly more memory than primitive types. |
| **Cache Behavior** | The core finding relates to the **64-byte caching block**. Even if only one element of a structure is accessed, the cache mechanism loads the entire cache block, which includes **adjacent elements/structures** from the array until the 64-byte line is full. This can lead to inefficient bandwidth usage if only a subset of the data fields within the structure are needed for a calculation. |

### Example 4: Array of Structures of Arrays (AoSoA)

This example demonstrated a **hybrid data layout** designed explicitly for high-performance computing (HPC) and efficient hardware alignment.

| Technical Element | Description |
| :--- | :--- |
| **Hybrid Layout** | Implements the **Array of Structures of Arrays (AoSoA)** layout, which tiles the data into defined vector lengths ($V$). The layout is described by the notation $A[\text{len}/V]SA[V]$. |
| **Vectorization Optimization** | The code explicitly defines the vector length $V=4$. This value is chosen to match hardware capabilities (like the vector length or GPU work group size), making the structure **portable**. The inner loop, which iterates over $V$, operates on a contiguous block of homogeneous data (e.g., $R_1 R_2 R_3 R_4$) and is designed to **vectorize**. |
| **Performance Benefit** | The AoSoA layout groups contiguous arrays inside structures, allowing it to leverage the advantages of both Array of Structures (AoS) and Structure of Arrays (SoA). Its performance generally tracks the better-performing method based on the specific kernel access pattern. |