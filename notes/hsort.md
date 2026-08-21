# ⚡ HeapSort: Makalah & Kurikulum Lengkap

**Versi:** 1.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** [Nama Anda]  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah HeapSort](#1-makalah-heapsort)
2. [Kurikulum Belajar](#2-kurikulum-belajar)
3. [Header dan Fungsinya](#3-header-dan-fungsinya)
4. [Implementasi: main.c](#4-implementasi-mainc)
5. [Implementasi: heapsort.h](#5-implementasi-heapsorth)
6. [Implementasi: heapsort.c](#6-implementasi-heapsortc)
7. [Implementasi: test_sort.c](#7-implementasi-test_sortc)
8. [Variasi HeapSort](#8-variasi-heapsort)
9. [Heap Data Structure](#9-heap-data-structure)
10. [Referensi](#10-referensi)
11. [Lampiran](#11-lampiran)

---

## 1. Makalah HeapSort

### 1.1. Abstrak — Ringkasan tentang HeapSort

**HeapSort** adalah algoritma sorting berbasis **Comparison Sort** yang menggunakan struktur data **Binary Heap**. HeapSort bekerja dengan membangun **Max-Heap** dari array, kemudian secara berulang mengambil elemen terbesar (root) dan menempatkannya di akhir array, lalu memperbaiki heap. HeapSort memiliki kompleksitas **O(n log n)** untuk semua kasus dan bersifat **in-place** (tidak membutuhkan memory tambahan).

**Kata Kunci:** HeapSort, Binary Heap, Max-Heap, Min-Heap, Priority Queue, Sorting, In-place

---

### 1.2. Pendahuluan — Latar belakang dan konsep

#### 1.2.1. Apa Itu HeapSort?

HeapSort adalah algoritma sorting yang dikembangkan oleh **J. W. J. Williams** pada tahun 1964. Algoritma ini bekerja dengan:

1. **Build Heap** — Mengubah array menjadi Max-Heap (parent > children)
2. **Extract Max** — Ambil root (elemen terbesar), tukar dengan elemen terakhir
3. **Heapify** — Perbaiki heap yang rusak
4. **Ulangi** — Sampai semua elemen terurut

```
┌─────────────────────────────────────────────────────────────────────┐
│                    HEAPSORT VISUALIZATION                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Array: [8, 3, 9, 1, 5, 7, 2, 6, 4]                               │
│                                                                     │
│  Step 1: Build Max-Heap                                            │
│            9                                                       │
│           / \                                                      │
│          8   7                                                     │
│         / \ / \                                                    │
│        6  5 3  2                                                   │
│       / \                                                          │
│      1   4                                                         │
│  Heap: [9, 8, 7, 6, 5, 3, 2, 1, 4]                               │
│                                                                     │
│  Step 2: Extract Max (9) → swap dengan akhir                      │
│  [4, 8, 7, 6, 5, 3, 2, 1] |9|                                     │
│                                                                     │
│  Step 3: Heapify → [8, 6, 7, 4, 5, 3, 2, 1] |9|                   │
│                                                                     │
│  Step 4: Extract Max (8) → swap                                   │
│  [1, 6, 7, 4, 5, 3, 2] |8, 9|                                     │
│                                                                     │
│  Continue...                                                        │
│                                                                     │
│  Result: [1, 2, 3, 4, 5, 6, 7, 8, 9] ✅                           │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.2.2. Kompleksitas Waktu

| Operasi | Kompleksitas | Keterangan |
|---------|--------------|------------|
| **Build Heap** | O(n) | Heapify dari bawah ke atas |
| **Extract Max** | O(log n) | Heapify setelah swap |
| **Total Sorting** | O(n log n) | n * O(log n) |
| **Best Case** | O(n log n) | Semua kasus sama |
| **Worst Case** | O(n log n) | Semua kasus sama |
| **Average Case** | O(n log n) | Semua kasus sama |

#### 1.2.3. Perbandingan dengan Algoritma Lain

| Algoritma | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| **HeapSort** | O(n log n) | O(n log n) | O(n log n) | O(1) | ❌ |
| QuickSort | O(n log n) | O(n log n) | O(n²) | O(log n) | ❌ |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | ✅ |
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | ✅ |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | ❌ |

---

### 1.3. Heap Data Structure

#### 1.3.1. Binary Heap — Complete Binary Tree

**Binary Heap** adalah Complete Binary Tree dengan sifat:

- **Max-Heap:** Parent ≥ Children (root adalah elemen terbesar)
- **Min-Heap:** Parent ≤ Children (root adalah elemen terkecil)

```
┌─────────────────────────────────────────────────────────────────────┐
│                    MAX-HEAP VISUALIZATION                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│          [9]  ← root (terbesar)                                    │
│         /   \                                                      │
│      [8]     [7]                                                   │
│      / \     / \                                                   │
│   [6]  [5] [3] [2]                                                 │
│   / \                                                              │
│ [1] [4]                                                            │
│                                                                     │
│  Array: [9, 8, 7, 6, 5, 3, 2, 1, 4]                               │
│                                                                     │
│  Parent index: i                                                   │
│  Left child: 2*i + 1                                               │
│  Right child: 2*i + 2                                              │
│  Parent of i: (i-1) / 2                                            │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.3.2. Heap Operations

| Operasi | Kompleksitas | Deskripsi |
|---------|--------------|-----------|
| **heapify** | O(log n) | Memperbaiki heap dari node |
| **build_heap** | O(n) | Membangun heap dari array |
| **extract_max** | O(log n) | Menghapus dan mengembalikan root |
| **insert** | O(log n) | Menambahkan elemen ke heap |
| **peek** | O(1) | Melihat root tanpa menghapus |

---

## 2. Kurikulum Belajar

### 2.1. Timeline 2 Minggu — Roadmap belajar HeapSort

```
Minggu 1: Dasar Heap & HeapSort
    ↓
Minggu 2: Variasi & Optimasi
```

### 2.2. Target Mingguan

| Minggu | Target | Output |
|--------|--------|--------|
| 1 | Memahami Binary Heap & HeapSort | Implementasi Max-Heap & Min-Heap |
| 2 | Memahami variasi & optimasi | HeapSort with custom comparator |

### 2.3. Materi Minggu 1 — Dasar Heap & HeapSort

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Complete Binary Tree | Baca teori, buat diagram |
| 2 | Heap Properties (Max-Heap, Min-Heap) | Implementasi |
| 3 | Heapify Operation | Implementasi |
| 4 | Build Heap | Implementasi |
| 5 | HeapSort | Implementasi |

### 2.4. Materi Minggu 2 — Variasi & Optimasi

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Min-Heap Sorting | Implementasi |
| 2 | HeapSort with Custom Comparator | Implementasi |
| 3 | Priority Queue | Implementasi |
| 4 | Benchmark & Testing | Testing |
| 5 | Final Documentation | Dokumentasi |

---

## 3. Header dan Fungsinya — Penjelasan setiap header

### 3.1. Header yang Digunakan

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<stdio.h>`** | Input/Output | `printf()` untuk output |
| **`<stdlib.h>`** | Standard Library | `malloc()` untuk dynamic array |
| **`<string.h>`** | Manipulasi String | `memcpy()` untuk copy array |
| **`<time.h>`** | Time Functions | `clock()` untuk benchmark |
| **`<assert.h>`** | Debugging | `assert()` untuk testing |
| **`<stdbool.h>`** | Boolean Type | `bool`, `true`, `false` |
| **`<stdint.h>`** | Fixed-width Integer | `uint32_t` |

---

## 4. Implementasi: `main.c`

### 4.1. Deskripsi — Entry point dan testing

**File:** `main.c`  
**Fungsi:** Entry point untuk testing HeapSort.

### 4.2. Source Code — `main.c`

```c
// main.c - Entry point untuk HeapSort
// Fungsi: Testing HeapSort dengan berbagai input
// Compile: gcc -o heapsort main.c heapsort.c
// Jalankan: ./heapsort

// ================================================================
// 1. HEADER
// ================================================================

#include <stdio.h>          // printf
#include <stdlib.h>         // malloc, free, rand
#include <time.h>           // clock, time
#include "heapsort.h"       // Deklarasi fungsi

// ================================================================
// 2. PRINT ARRAY
// ================================================================

void print_array(const int *arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// ================================================================
// 3. TEST HEAPSORT
// ================================================================

void test_heapsort() {
    printf("========================================\n");
    printf("  HEAPSORT TEST\n");
    printf("========================================\n\n");
    
    // Test Case 1: Random array
    int arr1[] = {8, 3, 9, 1, 5, 7, 2, 6, 4};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Original: ");
    print_array(arr1, size1);
    heapsort(arr1, size1);
    printf("Sorted:   ");
    print_array(arr1, size1);
    printf("\n");
    
    // Test Case 2: Already sorted
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Original (sorted): ");
    print_array(arr2, size2);
    heapsort(arr2, size2);
    printf("Sorted:            ");
    print_array(arr2, size2);
    printf("\n");
    
    // Test Case 3: Reverse sorted
    int arr3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("Original (reverse): ");
    print_array(arr3, size3);
    heapsort(arr3, size3);
    printf("Sorted:             ");
    print_array(arr3, size3);
    printf("\n");
    
    // Test Case 4: Array with duplicates
    int arr4[] = {5, 3, 5, 1, 3, 5, 2, 3, 1};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    printf("Original (duplicates): ");
    print_array(arr4, size4);
    heapsort(arr4, size4);
    printf("Sorted:                 ");
    print_array(arr4, size4);
    printf("\n");
}

// ================================================================
// 4. BENCHMARK
// ================================================================

void benchmark() {
    printf("========================================\n");
    printf("  BENCHMARK\n");
    printf("========================================\n\n");
    
    const int sizes[] = {1000, 10000, 50000, 100000};
    
    for (int s = 0; s < 4; s++) {
        int size = sizes[s];
        int *arr = malloc(size * sizeof(int));
        
        // Generate random array
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % (size * 10);
        }
        
        // Benchmark HeapSort
        int *copy = malloc(size * sizeof(int));
        memcpy(copy, arr, size * sizeof(int));
        
        clock_t start = clock();
        heapsort(copy, size);
        clock_t end = clock();
        double hs_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // Benchmark QuickSort (qsort) for comparison
        memcpy(copy, arr, size * sizeof(int));
        start = clock();
        qsort(copy, size, sizeof(int), compare_int);
        end = clock();
        double qs_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("Size: %d\n", size);
        printf("  HeapSort:  %.3f ms\n", hs_time);
        printf("  qsort:     %.3f ms\n", qs_time);
        printf("  Ratio:     %.2fx\n\n", qs_time / hs_time);
        
        free(copy);
        free(arr);
    }
}

// ================================================================
// 5. COMPARE FUNCTION
// ================================================================

int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// ================================================================
// 6. MAIN
// ================================================================

int main() {
    srand(time(NULL));
    
    test_heapsort();
    benchmark();
    
    return 0;
}
```

---

## 5. Implementasi: `heapsort.h`

### 5.1. Deskripsi — Deklarasi fungsi HeapSort

**File:** `heapsort.h`  
**Fungsi:** Mendeklarasikan semua fungsi HeapSort.

### 5.2. Source Code — `heapsort.h`

```c
// heapsort.h - Deklarasi HeapSort
// Fungsi: Header untuk heapsort.c

#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdint.h>     // uint32_t
#include <stdbool.h>    // bool

// ================================================================
// 1. HEAPSORT FUNCTIONS
// ================================================================

// HeapSort — Sorting ascending (Max-Heap)
void heapsort(int arr[], int size);

// HeapSort — Sorting descending (Min-Heap)
void heapsort_descending(int arr[], int size);

// HeapSort with custom comparator
void heapsort_comparator(int arr[], int size, int (*cmp)(int, int));

// ================================================================
// 2. HEAP OPERATIONS
// ================================================================

// Heapify — Memperbaiki heap dari node i
void heapify(int arr[], int size, int i);

// Heapify for Min-Heap
void heapify_min(int arr[], int size, int i);

// Build Heap — Membangun Max-Heap dari array
void build_heap(int arr[], int size);

// Build Min-Heap
void build_heap_min(int arr[], int size);

// ================================================================
// 3. PRIORITY QUEUE
// ================================================================

// Priority Queue structure
typedef struct {
    int *data;          // Array
    int size;           // Current size
    int capacity;       // Max capacity
    int (*cmp)(int, int); // Comparator (Max-Heap or Min-Heap)
} priority_queue_t;

// Create priority queue
priority_queue_t* pq_create(int capacity, int (*cmp)(int, int));

// Destroy priority queue
void pq_destroy(priority_queue_t *pq);

// Insert element
void pq_push(priority_queue_t *pq, int value);

// Extract root (max or min)
int pq_pop(priority_queue_t *pq);

// Peek root
int pq_peek(priority_queue_t *pq);

// Check if empty
bool pq_empty(priority_queue_t *pq);

// ================================================================
// 4. COMPARATORS
// ================================================================

int max_cmp(int a, int b);  // Untuk Max-Heap
int min_cmp(int a, int b);  // Untuk Min-Heap

#endif // HEAPSORT_H
```

---

## 6. Implementasi: `heapsort.c`

### 6.1. Deskripsi — Inti HeapSort

**File:** `heapsort.c`  
**Fungsi:** Implementasi HeapSort dan semua heap operations.

### 6.2. Source Code — `heapsort.c`

```c
// heapsort.c - Implementasi HeapSort
// Fungsi: HeapSort dengan Max-Heap dan Min-Heap

// ================================================================
// 1. HEADER
// ================================================================

#include "heapsort.h"
#include <stdlib.h>     // malloc, free
#include <string.h>     // memcpy

// ================================================================
// 2. COMPARATORS
// ================================================================

int max_cmp(int a, int b) {
    return a > b;  // Untuk Max-Heap (parent > child)
}

int min_cmp(int a, int b) {
    return a < b;  // Untuk Min-Heap (parent < child)
}

// ================================================================
// 3. HEAPIFY
// ================================================================

// ===== HEAPIFY (MAX-HEAP) =====
// Fungsi: Memperbaiki heap dengan asumsi subtree sudah heap
// Kompleksitas: O(log n)
void heapify(int arr[], int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // Cek apakah left child lebih besar
    if (left < size && arr[left] > arr[largest]) {
        largest = left;
    }
    
    // Cek apakah right child lebih besar
    if (right < size && arr[right] > arr[largest]) {
        largest = right;
    }
    
    // Jika largest bukan i, swap dan heapify lagi
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, size, largest);
    }
}

// ===== HEAPIFY (MIN-HEAP) =====
void heapify_min(int arr[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < size && arr[left] < arr[smallest]) {
        smallest = left;
    }
    if (right < size && arr[right] < arr[smallest]) {
        smallest = right;
    }
    
    if (smallest != i) {
        int temp = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = temp;
        heapify_min(arr, size, smallest);
    }
}

// ===== HEAPIFY WITH COMPARATOR =====
void heapify_cmp(int arr[], int size, int i, int (*cmp)(int, int)) {
    int root = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < size && cmp(arr[left], arr[root])) {
        root = left;
    }
    if (right < size && cmp(arr[right], arr[root])) {
        root = right;
    }
    
    if (root != i) {
        int temp = arr[i];
        arr[i] = arr[root];
        arr[root] = temp;
        heapify_cmp(arr, size, root, cmp);
    }
}

// ================================================================
// 4. BUILD HEAP
// ================================================================

// ===== BUILD MAX-HEAP =====
// Fungsi: Membangun Max-Heap dari array
// Kompleksitas: O(n)
void build_heap(int arr[], int size) {
    // Mulai dari parent terakhir (size/2 - 1) sampai root
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
}

// ===== BUILD MIN-HEAP =====
void build_heap_min(int arr[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify_min(arr, size, i);
    }
}

// ===== BUILD HEAP WITH COMPARATOR =====
void build_heap_cmp(int arr[], int size, int (*cmp)(int, int)) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify_cmp(arr, size, i, cmp);
    }
}

// ================================================================
// 5. HEAPSORT
// ================================================================

// ===== HEAPSORT (ASCENDING) =====
// Fungsi: Sorting ascending menggunakan Max-Heap
// Kompleksitas: O(n log n)
void heapsort(int arr[], int size) {
    // Step 1: Build Max-Heap
    build_heap(arr, size);
    
    // Step 2: Extract max satu per satu
    for (int i = size - 1; i > 0; i--) {
        // Swap root (max) dengan elemen terakhir
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        // Heapify root (ukuran heap berkurang)
        heapify(arr, i, 0);
    }
}

// ===== HEAPSORT (DESCENDING) =====
void heapsort_descending(int arr[], int size) {
    // Step 1: Build Min-Heap
    build_heap_min(arr, size);
    
    // Step 2: Extract min satu per satu
    for (int i = size - 1; i > 0; i--) {
        // Swap root (min) dengan elemen terakhir
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        // Heapify root (ukuran heap berkurang)
        heapify_min(arr, i, 0);
    }
}

// ===== HEAPSORT WITH COMPARATOR =====
void heapsort_comparator(int arr[], int size, int (*cmp)(int, int)) {
    build_heap_cmp(arr, size, cmp);
    
    for (int i = size - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify_cmp(arr, i, 0, cmp);
    }
}

// ================================================================
// 6. PRIORITY QUEUE
// ================================================================

// ===== CREATE PRIORITY QUEUE =====
priority_queue_t* pq_create(int capacity, int (*cmp)(int, int)) {
    priority_queue_t *pq = malloc(sizeof(priority_queue_t));
    pq->data = malloc(capacity * sizeof(int));
    pq->size = 0;
    pq->capacity = capacity;
    pq->cmp = cmp;
    return pq;
}

// ===== DESTROY PRIORITY QUEUE =====
void pq_destroy(priority_queue_t *pq) {
    free(pq->data);
    free(pq);
}

// ===== PUSH ELEMENT =====
void pq_push(priority_queue_t *pq, int value) {
    if (pq->size >= pq->capacity) return;
    
    // Insert di akhir
    pq->data[pq->size] = value;
    pq->size++;
    
    // Percolate up
    int i = pq->size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->cmp(pq->data[i], pq->data[parent])) {
            // Swap
            int temp = pq->data[i];
            pq->data[i] = pq->data[parent];
            pq->data[parent] = temp;
            i = parent;
        } else {
            break;
        }
    }
}

// ===== POP ROOT =====
int pq_pop(priority_queue_t *pq) {
    if (pq->size == 0) return -1;
    
    int root = pq->data[0];
    
    // Replace root dengan last element
    pq->size--;
    pq->data[0] = pq->data[pq->size];
    
    // Heapify root
    heapify_cmp(pq->data, pq->size, 0, pq->cmp);
    
    return root;
}

// ===== PEEK ROOT =====
int pq_peek(priority_queue_t *pq) {
    if (pq->size == 0) return -1;
    return pq->data[0];
}

// ===== CHECK EMPTY =====
bool pq_empty(priority_queue_t *pq) {
    return pq->size == 0;
}

// ================================================================
// 7. VISUALIZATION HELPER
// ================================================================

// ===== PRINT HEAP TREE =====
// Fungsi: Bantuan untuk debugging, print heap sebagai tree
void print_heap_tree(int arr[], int size, int index, int level) {
    if (index >= size) return;
    
    print_heap_tree(arr, size, 2 * index + 2, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%d\n", arr[index]);
    
    print_heap_tree(arr, size, 2 * index + 1, level + 1);
}
```

---

## 7. Implementasi: `test_sort.c`

### 7.1. Deskripsi — Unit test untuk HeapSort

**File:** `test_sort.c`  
**Fungsi:** Unit test untuk semua fungsi HeapSort.

### 7.2. Source Code — `test_sort.c`

```c
// test_sort.c - Unit Test untuk HeapSort
// Fungsi: Test semua variasi HeapSort
// Compile: gcc -o test_sort test_sort.c heapsort.c
// Jalankan: ./test_sort

// ================================================================
// 1. HEADER
// ================================================================

#include <stdio.h>          // printf
#include <assert.h>         // assert
#include "heapsort.h"       // Fungsi HeapSort

// ================================================================
// 2. ARRAY COMPARISON
// ================================================================

int is_sorted_ascending(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

int is_sorted_descending(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[i - 1]) return 0;
    }
    return 1;
}

int arrays_equal(int a[], int b[], int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

// ================================================================
// 3. TEST FUNCTIONS
// ================================================================

void test_heapsort_basic() {
    int arr[] = {8, 3, 9, 1, 5, 7, 2, 6, 4};
    int size = sizeof(arr) / sizeof(arr[0]);
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    heapsort(arr, size);
    assert(is_sorted_ascending(arr, size));
    assert(arrays_equal(arr, expected, size));
    
    printf("✅ heapsort_basic passed\n");
}

void test_heapsort_descending() {
    int arr[] = {8, 3, 9, 1, 5, 7, 2, 6, 4};
    int size = sizeof(arr) / sizeof(arr[0]);
    int expected[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    
    heapsort_descending(arr, size);
    assert(is_sorted_descending(arr, size));
    assert(arrays_equal(arr, expected, size));
    
    printf("✅ heapsort_descending passed\n");
}

void test_heapsort_sorted() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    heapsort(arr, size);
    assert(is_sorted_ascending(arr, size));
    
    printf("✅ heapsort_sorted passed\n");
}

void test_heapsort_reverse() {
    int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    heapsort(arr, size);
    assert(is_sorted_ascending(arr, size));
    
    printf("✅ heapsort_reverse passed\n");
}

void test_heapsort_duplicates() {
    int arr[] = {5, 3, 5, 1, 3, 5, 2, 3, 1};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    heapsort(arr, size);
    assert(is_sorted_ascending(arr, size));
    
    printf("✅ heapsort_duplicates passed\n");
}

void test_heapsort_single() {
    int arr[] = {5};
    int size = 1;
    
    heapsort(arr, size);
    assert(is_sorted_ascending(arr, size));
    
    printf("✅ heapsort_single passed\n");
}

void test_priority_queue() {
    int cmp = 1;
    priority_queue_t *pq = pq_create(10, max_cmp);
    
    pq_push(pq, 5);
    pq_push(pq, 3);
    pq_push(pq, 9);
    pq_push(pq, 1);
    pq_push(pq, 7);
    
    assert(pq_pop(pq) == 9);
    assert(pq_pop(pq) == 7);
    assert(pq_pop(pq) == 5);
    assert(pq_pop(pq) == 3);
    assert(pq_pop(pq) == 1);
    assert(pq_empty(pq));
    
    pq_destroy(pq);
    
    printf("✅ priority_queue passed\n");
}

// ================================================================
// 4. MAIN
// ================================================================

int main() {
    printf("========================================\n");
    printf("  HEAPSORT UNIT TESTS\n");
    printf("========================================\n\n");
    
    test_heapsort_basic();
    test_heapsort_descending();
    test_heapsort_sorted();
    test_heapsort_reverse();
    test_heapsort_duplicates();
    test_heapsort_single();
    test_priority_queue();
    
    printf("\n✅ All tests passed!\n");
    return 0;
}
```

---

## 8. Variasi HeapSort

### 8.1. Max-Heap Sort — Sorting ascending

```c
void heapsort(int arr[], int size) {
    // Build Max-Heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
    
    // Extract max
    for (int i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}
```

### 8.2. Min-Heap Sort — Sorting descending

```c
void heapsort_descending(int arr[], int size) {
    // Build Min-Heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify_min(arr, size, i);
    }
    
    // Extract min
    for (int i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify_min(arr, i, 0);
    }
}
```

### 8.3. HeapSort with Custom Comparator

```c
void heapsort_comparator(int arr[], int size, int (*cmp)(int, int)) {
    build_heap_cmp(arr, size, cmp);
    
    for (int i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify_cmp(arr, i, 0, cmp);
    }
}
```

---

## 9. Heap Data Structure

### 9.1. Priority Queue Implementation

```c
// Insert element (percolate up)
void pq_push(priority_queue_t *pq, int value) {
    if (pq->size >= pq->capacity) return;
    
    pq->data[pq->size] = value;
    pq->size++;
    
    int i = pq->size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->cmp(pq->data[i], pq->data[parent])) {
            swap(&pq->data[i], &pq->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}
```

### 9.2. Heap Applications

| Aplikasi | Deskripsi |
|----------|-----------|
| **Priority Queue** | Task scheduling, Dijkstra's algorithm |
| **HeapSort** | Sorting algorithm |
| **Selection Algorithms** | Find kth largest/smallest element |
| **Graph Algorithms** | Prim's, Dijkstra's, A* |
| **Event Simulation** | Event-driven simulation |

---

## 10. Referensi

### 10.1. Buku — Referensi yang direkomendasikan

- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "The Art of Computer Programming" - Donald Knuth
- "The C Programming Language" - Kernighan & Ritchie

### 10.2. Website — Sumber online

- GeeksforGeeks: https://www.geeksforgeeks.org/heap-sort/
- Wikipedia: https://en.wikipedia.org/wiki/Heapsort

---

## 11. Lampiran

### A. Kompleksitas Heap Operations

| Operasi | Kompleksitas |
|---------|--------------|
| Build Heap | O(n) |
| Heapify | O(log n) |
| Insert | O(log n) |
| Extract Max/Min | O(log n) |
| Peek | O(1) |
| HeapSort | O(n log n) |

### B. Perbandingan HeapSort dengan Algoritma Lain

| Algoritma | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| **HeapSort** | O(n log n) | O(n log n) | O(n log n) | O(1) | ❌ |
| QuickSort | O(n log n) | O(n log n) | O(n²) | O(log n) | ❌ |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | ✅ |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | ❌ |

### C. Common Pitfalls

| Kesalahan | Solusi |
|-----------|--------|
| Index out of bounds | Pastikan 2*i+1 < size |
| Build heap salah | Mulai dari size/2-1 |
| Heapify tidak rekursif | Tambahkan rekursi setelah swap |
| Priority queue overflow | Cek kapasitas sebelum push |

---

**Akhir Dokumen**

> *"HeapSort: Heap your way to sorted."*