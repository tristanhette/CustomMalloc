# Custom Memory Allocator

## Overview

This project is a complete and functional custom memory allocator implemented in C. It provides custom versions of the standard memory management functions: `malloc`, `free`, `calloc`, and `realloc`. The behavior of each function follows the specifications described in their respective man pages.

The allocator was developed in a structured way, with increasing levels of complexity to ensure clarity and maintainability. A focus was placed on correctness first, followed by performance and efficiency improvements.

## Features

- ✅ **malloc**: Allocates memory blocks of arbitrary size.
- ✅ **free**: Frees previously allocated memory.
- ✅ **calloc**: Allocates and zero-initializes memory.
- ✅ **realloc**: Resizes previously allocated memory blocks.
- ✅ **Thread safety**: Implemented using simple and effective synchronization techniques.
- ✅ **Metadata efficiency**: Custom structures designed to reduce overhead and improve speed.
- ✅ **Optimized algorithms**: Smart allocation strategies for performance and memory reuse.

## Architecture

The allocator is based on custom memory zones and metadata blocks, with a focus on minimizing fragmentation and system calls. It manages memory internally through strategies such as block splitting and coalescing to optimize reuse.

### Key design principles

- **Efficiency**: Minimal overhead per allocation.
- **Scalability**: Works correctly and efficiently across multiple threads.
- **Simplicity first**: Prioritized a working version before applying optimizations.

## Usage

### Build

A Makefile is provided to compile the allocator as a shared library:

```bash
make
