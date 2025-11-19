# Lab 8: MPI - The Parallel Backbone

The Message Passing Interface (MPI) is the dominant library-based language in high-performance computing, crucial for allowing parallel programs to access additional compute nodes and run larger problems. This repository contains fundamental examples and explorations of core MPI communication principles, particularly focusing on mitigating communication bottlenecks and utilizing efficient collective routines.

## 1. MPI Program Basics

All MPI programs adhere to a basic structure: they begin with `MPI_Init` (or `MPI_Init_thread` for hybrid approaches) and end with `MPI_Finalize`. The program typically uses `MPI_Comm_rank` and `MPI_Comm_size` to determine the unique process ID (rank) and the total number of processes (`nprocs`) within the communication group, typically `MPI_COMM_WORLD`.

## 2. Point-to-Point Communication Bottlenecks (Send/Receive)

The core message-passing approach relies on sending messages from process-to-process using basic blocking routines: `MPI_Send` and `MPI_Recv`. These routines are **blocking calls**, meaning they do not return until a specific condition is fulfilled (the send buffer is safe to reuse or the receive buffer is filled).

### The Hang (Deadlock) Problem

The sequencing of these blocking calls is tricky, and if done incorrectly, they can lead to a **hang** or **deadlock**.

*   **Blocking Receive First (Guaranteed Hang):** If all participating processes execute a blocking `MPI_Recv` before calling `MPI_Send`, all processes will wait indefinitely for a message that will never arrive, resulting in a hang.
*   **Blocking Send First (Conditional Hang):** If all processes execute a blocking `MPI_Send` first, the program may still hang if the message size is large. For large messages, the send waits for the corresponding receive to post a buffer before the send returns; if the receive is scheduled later, the program stalls.

### Solutions to Avoid Deadlocks

To avoid these bottlenecks and bugs, safer communication methods are recommended:

1.  **Alternating Ranks:** A conditional check (`if (rank % 2 == 0)`) can be used to alternate the order of sends and receives (even ranks send first, odd ranks receive first). However, this method is complicated, requires careful use of conditionals, and is generally avoided in favor of library-managed calls.
2.  **Combined Calls (`MPI_Sendrecv`):** Using the combined `MPI_Sendrecv` routine is a better practice as it replaces individual `MPI_Send` and `MPI_Recv` calls, **delegating the responsibility for avoiding hangs** to the MPI library.
3.  **Non-Blocking Calls (`MPI_Isend`/`MPI_Irecv`):** Also known as immediate or asynchronous calls, these initiate communication immediately but return before completion. The actual completion of the communication is checked later using a synchronization routine like **`MPI_Waitall`**. This strategy reduces execution stalls and is often both safer and faster.

## 3. Collective Communication

Collective communications are specialized MPI routines that operate on a **group of processes** defined by a communicator. They are essential because they encapsulate common, complex communication patterns into single calls, simplifying code and providing optimization advantages managed internally by the MPI library.

**Note:** All members of the communication group **must call the collective routine** or the program will hang.

### Key Collective Routines

| Routine | Description | Function |
| :--- | :--- | :--- |
| **`MPI_Bcast`** | **Broadcast** | Sends data from one root process (e.g., rank 0) to all other processes in the group. Used efficiently for sending small file input data. |
| **`MPI_Scatter`** | **Scatter** | Distributes contiguous chunks of a large array from the root process out to all other processes for distributed work. |
| **`MPI_Gather`** | **Gather** | Collects data from all processes and stacks it into a single array on the root process. Used for aggregating distributed results or ordering debug printouts. |
| **`MPI_Reduce`** | **Reduction** | Combines values across all processes into a single scalar result (e.g., calculating sum, maximum, or minimum) using predefined operators (`MPI_SUM`, `MPI_MAX`, etc.). The result is stored on the root process. |
| **`MPI_Allreduce`** | **All Reduction** | Performs the same reduction operation as `MPI_Reduce`, but makes the final result available on **all processes**. |
| **`MPI_Barrier`** | **Barrier** | The simplest collective call. It forces all threads to synchronize and wait until every process has reached that point before continuing execution. Often used for timing synchronization. |

## 4. Advanced Concepts and Optimization

MPI offers advanced features to simplify complex code patterns, particularly for large-scale scientific simulations:

*   **Custom Data Types:** Functions like `MPI_Type_vector` or `MPI_Type_create_subarray` allow programmers to define complex data structures (like columns of an array or structured data) into a single custom type. Using custom data types can lead to cleaner code and may avoid unnecessary data copies, potentially improving performance.
*   **Ghost Cell Exchanges:** This critical technique links computational subdomains on separate processes. Ghost cells cache values from adjacent processors, grouping needed communications into fewer, optimized calls.
*   **Hybrid MPI + OpenMP:** This strategy combines distributed memory parallelism (MPI ranks) with shared memory parallelism (OpenMP threads) within a node. This can reduce memory requirements for MPI buffers, lower communication overhead (fewer ghost cells to exchange between nodes), and access full hardware capabilities. Hybrid execution requires initializing MPI with `MPI_Init_thread`.