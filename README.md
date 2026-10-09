# [AGH] Operating Systems 2025/2026

This repository contains solutions to laboratory tasks for the **Operating Systems** course. All programs are implemented in **C** for Linux/POSIX systems.

---

## Folder Structure and Topics

| Laboratory | Topic | Description |
| :--- | :--- | :--- |
| **[`lab1`](https://github.com/kkordecky/operating-systems/tree/master/lab1)** | **Processes** | Creating child processes (`fork`, `vfork`), handling PID, memory sharing, and project organization using `Makefile`. |
| **[`lab2`](https://github.com/kkordecky/operating-systems/tree/master/lab2)** | **Signals** | Signal handling (`SIGUSR1`, `SIGUSR2`), masking, ignoring, custom handlers, sending signals (`raise`, `sigqueue`), and working with static and dynamic libraries. |
| **[`lab3`](https://github.com/kkordecky/operating-systems/tree/master/lab3)** | **Pipes** | Inter-process communication using anonymous (`pipe`) and named (`FIFO`) pipes — calculating numerical integration using the rectangle rule. |
| **[`lab4`](https://github.com/kkordecky/operating-systems/tree/master/lab4)** | **Message Queues** | IPC mechanisms (System V / POSIX `mqueue`) implemented in a simple multi-client chat application. |
| **[`lab5`](https://github.com/kkordecky/operating-systems/tree/master/lab5)** | **Semaphores and Shared Memory** | Process synchronization using shared memory and semaphores (`empty`, `full`, `mutex`) based on the Producer-Consumer problem. |
| **[`lab7`](https://github.com/kkordecky/operating-systems/tree/master/lab7)** | **Sockets** | TCP IPv4 network communication — simple HTTP server implementation and a dedicated TCP client. |

---

## Requirements and Compilation

- **Compiler:** `gcc`
- **Build System:** `make`
- **Environment:** Linux / POSIX

Most directories contain their own `Makefile`. To compile the code for a specific lab, navigate to its directory and run:

```bash
make
