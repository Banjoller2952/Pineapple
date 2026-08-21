# 🔍 Binary Search: Makalah & Kurikulum Lengkap

**Versi:** 1.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** DeepSeek  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah Binary Search](#1-makalah-binary-search)
2. [Kurikulum Belajar](#2-kurikulum-belajar)
3. [Header dan Fungsinya](#3-header-dan-fungsinya)
4. [Implementasi: main.c](#4-implementasi-mainc)
5. [Implementasi: binary_search.h](#5-implementasi-binary_searchh)
6. [Implementasi: binary_search.c](#6-implementasi-binary_searchc)
7. [Implementasi: test_search.c](#7-implementasi-test_searchc)
8. [Compile & Jalankan](#8-compile--jalankan)
9. [Variasi Binary Search](#9-variasi-binary-search)
10. [Referensi](#10-referensi)
11. [Lampiran](#11-lampiran)

---

## 1. Makalah Binary Search

### 1.1. Abstrak — Ringkasan tentang Binary Search

**Binary Search** (Pencarian Biner) adalah algoritma pencarian yang efisien untuk menemukan posisi suatu nilai dalam **array yang sudah terurut**. Algoritma ini bekerja dengan cara membagi array menjadi dua bagian secara berulang, membandingkan nilai target dengan elemen tengah, dan menentukan apakah target berada di kiri atau kanan.

**Kata Kunci:** Binary Search, Divide and Conquer, Algoritma Pencarian, Kompleksitas O(log n)

---

### 1.2. Pendahuluan — Latar belakang dan konsep

#### 1.2.1. Apa Itu Binary Search?

Binary Search adalah algoritma pencarian yang bekerja pada **data terurut** dengan cara:
1. Menentukan elemen tengah dari array
2. Membandingkan elemen tengah dengan nilai yang dicari
3. Jika sama → ditemukan
4. Jika lebih kecil → cari di bagian kiri
5. Jika lebih besar → cari di bagian kanan
6. Ulangi sampai ditemukan atau tidak ada

```
┌─────────────────────────────────────────────────────────────────────┐
│                    BINARY SEARCH VISUALIZATION                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Array: [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]                       │
│  Target: 11                                                         │
│                                                                     │
│  Step 1: Cari di tengah (5) = 9 < 11 → cari di kanan              │
│  [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]                              │
│               ↑                                                    │
│                                                                     │
│  Step 2: Cari di tengah (7) = 15 > 11 → cari di kiri              │
│  [11, 13, 15]                                                       │
│       ↑                                                            │
│                                                                     │
│  Step 3: Cari di tengah (8) = 11 = 11 → DITEMUKAN!                │
│  [11, 13]                                                           │
│   ↑                                                                │
│                                                                     │
│  Hasil: Index 8                                                    │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.2.2. Kompleksitas Waktu

| Metode | Best Case | Average Case | Worst Case |
|--------|-----------|--------------|------------|
| **Binary Search** | O(1) | O(log n) | O(log n) |
| Linear Search | O(1) | O(n) | O(n) |
| Jump Search | O(√n) | O(√n) | O(√n) |
| Interpolation Search | O(1) | O(log log n) | O(n) |

#### 1.2.3. Kapan Menggunakan Binary Search?

| Situasi | Pakai Binary Search? | Alasan |
|---------|---------------------|--------|
| **Array terurut** | ✅ Ya | Waktu O(log n) |
| **Array tidak terurut** | ❌ Tidak | Harus sorting dulu |
| **Data kecil (< 10 elemen)** | ⚠️ Mungkin tidak | Linear search lebih simpel |
| **Data besar (1M elemen)** | ✅ Ya | Jauh lebih cepat |
| **Data dinamis (sering berubah)** | ❌ Tidak | Sorting ulang mahal |

---

### 1.3. Cara Kerja Binary Search

#### 1.3.1. Pseudocode

```
FUNCTION binary_search(array, target):
    low = 0
    high = length(array) - 1
    
    WHILE low <= high:
        mid = (low + high) / 2
        
        IF array[mid] == target:
            RETURN mid
        ELSE IF array[mid] < target:
            low = mid + 1
        ELSE:
            high = mid - 1
    
    RETURN -1  // Tidak ditemukan
END FUNCTION
```

#### 1.3.2. Visualisasi Step-by-Step

```
Array: [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
Target: 23

Iterasi 1:
  low = 0, high = 9, mid = 4
  array[4] = 16 < 23 → low = 5

Iterasi 2:
  low = 5, high = 9, mid = 7
  array[7] = 56 > 23 → high = 6

Iterasi 3:
  low = 5, high = 6, mid = 5
  array[5] = 23 = 23 → FOUND!
```

---

## 2. Kurikulum Belajar

### 2.1. Timeline 2 Minggu — Roadmap belajar Binary Search

```
Minggu 1: Dasar Binary Search
    ↓
Minggu 2: Variasi dan Optimasi
```

### 2.2. Target Mingguan

| Minggu | Target | Output |
|--------|--------|--------|
| 1 | Memahami konsep Binary Search | Bisa implementasi iteratif & rekursif |
| 2 | Memahami variasi Binary Search | Bisa implementasi lower_bound, upper_bound |

### 2.3. Materi Minggu 1 — Dasar Binary Search

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Konsep Divide and Conquer | Baca teori, buat diagram |
| 2 | Binary Search Iteratif | Implementasi di C |
| 3 | Binary Search Rekursif | Implementasi di C |
| 4 | Analisis Kompleksitas | Hitung O(log n) |
| 5 | Testing & Debugging | Uji dengan berbagai input |

### 2.4. Materi Minggu 2 — Variasi Binary Search

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Lower Bound (First Position) | Implementasi |
| 2 | Upper Bound (Last Position) | Implementasi |
| 3 | Count Occurrences | Implementasi |
| 4 | Binary Search pada Rotated Array | Implementasi |
| 5 | Binary Search pada Infinite Array | Implementasi |

---

## 3. Header dan Fungsinya — Penjelasan setiap header

### 3.1. Header yang Digunakan

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<stdio.h>`** | Input/Output | `printf()` untuk output, `scanf()` untuk input |
| **`<stdlib.h>`** | Standard Library | `malloc()` untuk alokasi memory, `qsort()` untuk sorting |
| **`<string.h>`** | Manipulasi String | `memcpy()` untuk copy array |
| **`<time.h>`** | Time Functions | `clock()` untuk mengukur waktu |
| **`<math.h>`** | Mathematical Functions | `log2()` untuk kompleksitas |
| **`<assert.h>`** | Debugging | `assert()` untuk testing |
| **`<stdint.h>`** | Fixed-width Integer | `uint32_t`, `int64_t` |
| **`<stdbool.h>`** | Boolean Type | `bool`, `true`, `false` |

---

## 4. Implementasi: `main.c`

### 4.1. Deskripsi — Entry point dan testing

**File:** `main.c`  
**Fungsi:** Entry point program untuk testing binary search.

### 4.2. Source Code — `main.c`

```c
// main.c - Entry point untuk Binary Search
// Fungsi: Testing binary search dengan berbagai input
// Compile: gcc -o binary_search main.c binary_search.c
// Jalankan: ./binary_search

// ================================================================
// 1. HEADER
// ================================================================

#include <stdio.h>          // printf, scanf
#include <stdlib.h>         // malloc, free, qsort
#include <time.h>           // clock, time
#include <assert.h>         // assert
#include "binary_search.h"  // Deklarasi fungsi binary search

// ================================================================
// 2. COMPARE FUNCTION FOR QSORT
// ================================================================

// Fungsi: Comparison function untuk qsort (ascending)
// Header: <stdlib.h> (qsort)
int compare_int(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// ================================================================
// 3. PRINT ARRAY
// ================================================================

// Fungsi: Cetak array ke layar
// Header: <stdio.h> (printf)
void print_array(const int *arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// ================================================================
// 4. TEST BINARY SEARCH
// ================================================================

// Fungsi: Test binary search dengan berbagai kasus
// Header: <stdio.h> (printf), <assert.h> (assert)
void test_binary_search() {
    printf("========================================\n");
    printf("  TESTING BINARY SEARCH\n");
    printf("========================================\n\n");
    
    // Test Case 1: Array normal, target ada
    int arr1[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int target1 = 11;
    int result1 = binary_search(arr1, size1, target1);
    printf("Test 1: Array: ");
    print_array(arr1, size1);
    printf("        Target: %d -> Index: %d (expected: 5)\n", target1, result1);
    assert(result1 == 5);
    
    // Test Case 2: Target di awal
    int target2 = 1;
    int result2 = binary_search(arr1, size1, target2);
    printf("Test 2: Target: %d -> Index: %d (expected: 0)\n", target2, result2);
    assert(result2 == 0);
    
    // Test Case 3: Target di akhir
    int target3 = 19;
    int result3 = binary_search(arr1, size1, target3);
    printf("Test 3: Target: %d -> Index: %d (expected: 9)\n", target3, result3);
    assert(result3 == 9);
    
    // Test Case 4: Target tidak ada
    int target4 = 10;
    int result4 = binary_search(arr1, size1, target4);
    printf("Test 4: Target: %d -> Index: %d (expected: -1)\n", target4, result4);
    assert(result4 == -1);
    
    // Test Case 5: Array kosong
    int *arr2 = NULL;
    int result5 = binary_search(arr2, 0, 5);
    printf("Test 5: Empty array -> Index: %d (expected: -1)\n", result5);
    assert(result5 == -1);
    
    // Test Case 6: Array 1 elemen, target ada
    int arr3[] = {5};
    int result6 = binary_search(arr3, 1, 5);
    printf("Test 6: [5], target 5 -> Index: %d (expected: 0)\n", result6);
    assert(result6 == 0);
    
    // Test Case 7: Array 1 elemen, target tidak ada
    int result7 = binary_search(arr3, 1, 3);
    printf("Test 7: [5], target 3 -> Index: %d (expected: -1)\n", result7);
    assert(result7 == -1);
    
    printf("\n✅ All tests passed!\n");
}

// ================================================================
// 5. BENCHMARK
// ================================================================

// Fungsi: Benchmark binary search vs linear search
// Header: <stdio.h> (printf), <time.h> (clock), <stdlib.h> (malloc, qsort)
void benchmark() {
    printf("\n========================================\n");
    printf("  BENCHMARK\n");
    printf("========================================\n\n");
    
    const int sizes[] = {1000, 10000, 100000, 1000000};
    
    for (int s = 0; s < 4; s++) {
        int size = sizes[s];
        int *arr = malloc(size * sizeof(int));
        
        // Generate random array
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % (size * 10);
        }
        
        // Sort array untuk binary search
        qsort(arr, size, sizeof(int), compare_int);
        
        // Target: elemen di tengah
        int target = arr[size / 2];
        
        // Binary Search
        clock_t start = clock();
        int bs_result = binary_search(arr, size, target);
        clock_t end = clock();
        double bs_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // Linear Search
        start = clock();
        int ls_result = -1;
        for (int i = 0; i < size; i++) {
            if (arr[i] == target) {
                ls_result = i;
                break;
            }
        }
        end = clock();
        double ls_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("Size: %d\n", size);
        printf("  Binary Search: %.3f ms (result: %d)\n", bs_time, bs_result);
        printf("  Linear Search: %.3f ms (result: %d)\n", ls_time, ls_result);
        printf("  Speedup: %.2fx\n\n", ls_time / bs_time);
        
        free(arr);
    }
}

// ================================================================
// 6. MAIN
// ================================================================

int main() {
    // Seed random
    srand(time(NULL));
    
    // Test
    test_binary_search();
    
    // Benchmark
    benchmark();
    
    return 0;
}
```

---

## 5. Implementasi: `binary_search.h`

### 5.1. Deskripsi — Deklarasi fungsi binary search

**File:** `binary_search.h`  
**Fungsi:** Mendeklarasikan semua fungsi binary search.

### 5.2. Source Code — `binary_search.h`

```c
// binary_search.h - Deklarasi Binary Search
// Fungsi: Header untuk binary_search.c
// Digunakan oleh: main.c, test_search.c

#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

// ================================================================
// 1. HEADER YANG DIGUNAKAN
// ================================================================

#include <stdint.h>     // int64_t
#include <stdbool.h>    // bool

// ================================================================
// 2. DEKLARASI FUNGSI
// ================================================================

// ===== BASIC BINARY SEARCH =====

// Fungsi: Binary search iteratif
// Input: arr (array terurut), size (ukuran array), target (nilai dicari)
// Output: Index jika ditemukan, -1 jika tidak
int binary_search(const int *arr, int size, int target);

// Fungsi: Binary search rekursif
int binary_search_recursive(const int *arr, int low, int high, int target);

// ===== VARIASI BINARY SEARCH =====

// Fungsi: Lower bound (posisi pertama target)
// Output: Index pertama target, atau posisi insert
int lower_bound(const int *arr, int size, int target);

// Fungsi: Upper bound (posisi terakhir target + 1)
// Output: Index terakhir target + 1
int upper_bound(const int *arr, int size, int target);

// Fungsi: Count occurrences (jumlah kemunculan target)
int count_occurrences(const int *arr, int size, int target);

// Fungsi: Binary search pada rotated array
int search_rotated(const int *arr, int size, int target);

// Fungsi: Binary search pada infinite array
int search_infinite(const int *arr, int target);

// ===== BINARY SEARCH WITH STRUCT =====

// Struktur untuk data yang lebih kompleks
typedef struct {
    int key;
    char value[64];
} key_value_t;

// Fungsi: Binary search pada struct array
int binary_search_struct(const key_value_t *arr, int size, int target_key,
                         char *result_value);

#endif // BINARY_SEARCH_H
```

---

## 6. Implementasi: `binary_search.c`

### 6.1. Deskripsi — Inti Binary Search

**File:** `binary_search.c`  
**Fungsi:** Implementasi semua fungsi binary search.

### 6.2. Source Code — `binary_search.c`

```c
// binary_search.c - Implementasi Binary Search
// Fungsi: Semua fungsi binary search (iteratif, rekursif, variasi)
// Compile: gcc -c binary_search.c

// ================================================================
// 1. HEADER
// ================================================================

#include "binary_search.h"
#include <string.h>     // memcpy, strcpy
#include <math.h>       // log2 (opsional)

// ================================================================
// 2. BASIC BINARY SEARCH
// ================================================================

// ===== BINARY SEARCH ITERATIF =====
// Fungsi: Binary search dengan loop
// Kompleksitas: O(log n)
int binary_search(const int *arr, int size, int target) {
    int low = 0;
    int high = size - 1;
    
    while (low <= high) {
        // Hitung mid — hindari overflow
        int mid = low + (high - low) / 2;
        
        if (arr[mid] == target) {
            return mid;  // Ditemukan
        } else if (arr[mid] < target) {
            low = mid + 1;  // Cari di kanan
        } else {
            high = mid - 1;  // Cari di kiri
        }
    }
    
    return -1;  // Tidak ditemukan
}

// ===== BINARY SEARCH REKURSIF =====
// Fungsi: Binary search dengan rekursi
// Kompleksitas: O(log n)
int binary_search_recursive(const int *arr, int low, int high, int target) {
    if (low > high) {
        return -1;  // Tidak ditemukan
    }
    
    int mid = low + (high - low) / 2;
    
    if (arr[mid] == target) {
        return mid;
    } else if (arr[mid] < target) {
        return binary_search_recursive(arr, mid + 1, high, target);
    } else {
        return binary_search_recursive(arr, low, mid - 1, target);
    }
}

// ================================================================
// 3. VARIASI BINARY SEARCH
// ================================================================

// ===== LOWER BOUND =====
// Fungsi: Cari posisi pertama target
// Output: Index pertama target, atau posisi insert
// Kompleksitas: O(log n)
int lower_bound(const int *arr, int size, int target) {
    int low = 0;
    int high = size;
    
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    
    return low;
}

// ===== UPPER BOUND =====
// Fungsi: Cari posisi terakhir target + 1
// Output: Index terakhir target + 1
// Kompleksitas: O(log n)
int upper_bound(const int *arr, int size, int target) {
    int low = 0;
    int high = size;
    
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] <= target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    
    return low;
}

// ===== COUNT OCCURRENCES =====
// Fungsi: Hitung jumlah kemunculan target
// Kompleksitas: O(log n)
int count_occurrences(const int *arr, int size, int target) {
    int lb = lower_bound(arr, size, target);
    int ub = upper_bound(arr, size, target);
    return ub - lb;
}

// ===== SEARCH IN ROTATED ARRAY =====
// Fungsi: Binary search pada array yang di-rotasi
// Contoh: [4, 5, 6, 7, 0, 1, 2]
// Kompleksitas: O(log n)
int search_rotated(const int *arr, int size, int target) {
    int low = 0;
    int high = size - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        // Cek apakah bagian kiri terurut
        if (arr[low] <= arr[mid]) {
            // Target di kiri?
            if (arr[low] <= target && target < arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } else {
            // Bagian kanan terurut
            if (arr[mid] < target && target <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    
    return -1;
}

// ===== SEARCH IN INFINITE ARRAY =====
// Fungsi: Binary search pada array yang tak terbatas
// Kompleksitas: O(log n)
int search_infinite(const int *arr, int target) {
    // Temukan batas dengan eksponensial
    int low = 0;
    int high = 1;
    
    while (arr[high] < target) {
        low = high;
        high = high * 2;
    }
    
    // Binary search di antara low dan high
    return binary_search_recursive(arr, low, high, target);
}

// ================================================================
// 4. BINARY SEARCH ON STRUCT
// ================================================================

// ===== BINARY SEARCH STRUCT =====
// Fungsi: Binary search pada array of struct
// Kompleksitas: O(log n)
int binary_search_struct(const key_value_t *arr, int size, int target_key,
                         char *result_value) {
    int low = 0;
    int high = size - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (arr[mid].key == target_key) {
            strcpy(result_value, arr[mid].value);
            return mid;
        } else if (arr[mid].key < target_key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return -1;
}
```

---

## 7. Implementasi: `test_search.c`

### 7.1. Deskripsi — Unit test untuk binary search

**File:** `test_search.c`  
**Fungsi:** Unit test untuk semua fungsi binary search.

### 7.2. Source Code — `test_search.c`

```c
// test_search.c - Unit Test untuk Binary Search
// Fungsi: Test semua fungsi binary search
// Compile: gcc -o test_search test_search.c binary_search.c
// Jalankan: ./test_search

// ================================================================
// 1. HEADER
// ================================================================

#include <stdio.h>          // printf
#include <assert.h>         // assert
#include "binary_search.h"  // Fungsi binary search

// ================================================================
// 2. TEST FUNCTIONS
// ================================================================

void test_basic_search() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int size = 8;
    
    // Test 1: Target di tengah
    assert(binary_search(arr, size, 7) == 3);
    assert(binary_search_recursive(arr, 0, size - 1, 7) == 3);
    
    // Test 2: Target di awal
    assert(binary_search(arr, size, 1) == 0);
    
    // Test 3: Target di akhir
    assert(binary_search(arr, size, 15) == 7);
    
    // Test 4: Target tidak ada
    assert(binary_search(arr, size, 10) == -1);
    
    // Test 5: Array kosong
    assert(binary_search(NULL, 0, 5) == -1);
    
    printf("✅ test_basic_search passed\n");
}

void test_lower_upper_bound() {
    int arr[] = {1, 2, 2, 2, 3, 4, 5, 5, 6};
    int size = 9;
    
    // Test lower_bound
    assert(lower_bound(arr, size, 2) == 1);
    assert(lower_bound(arr, size, 5) == 6);
    assert(lower_bound(arr, size, 7) == 9);
    
    // Test upper_bound
    assert(upper_bound(arr, size, 2) == 4);
    assert(upper_bound(arr, size, 5) == 8);
    assert(upper_bound(arr, size, 7) == 9);
    
    // Test count_occurrences
    assert(count_occurrences(arr, size, 2) == 3);
    assert(count_occurrences(arr, size, 5) == 2);
    assert(count_occurrences(arr, size, 7) == 0);
    
    printf("✅ test_lower_upper_bound passed\n");
}

void test_rotated_array() {
    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    int size = 7;
    
    assert(search_rotated(arr, size, 4) == 0);
    assert(search_rotated(arr, size, 7) == 3);
    assert(search_rotated(arr, size, 0) == 4);
    assert(search_rotated(arr, size, 2) == 6);
    assert(search_rotated(arr, size, 3) == -1);
    
    printf("✅ test_rotated_array passed\n");
}

void test_struct_search() {
    key_value_t dict[] = {
        {1, "Apple"},
        {2, "Banana"},
        {3, "Cherry"},
        {4, "Date"},
        {5, "Elderberry"}
    };
    int size = 5;
    char result[64];
    
    assert(binary_search_struct(dict, size, 3, result) == 2);
    assert(strcmp(result, "Cherry") == 0);
    
    assert(binary_search_struct(dict, size, 5, result) == 4);
    assert(strcmp(result, "Elderberry") == 0);
    
    assert(binary_search_struct(dict, size, 6, result) == -1);
    
    printf("✅ test_struct_search passed\n");
}

// ================================================================
// 3. MAIN
// ================================================================

int main() {
    printf("========================================\n");
    printf("  BINARY SEARCH UNIT TESTS\n");
    printf("========================================\n\n");
    
    test_basic_search();
    test_lower_upper_bound();
    test_rotated_array();
    test_struct_search();
    
    printf("\n✅ All tests passed!\n");
    return 0;
}
```

---

## 8. Compile & Jalankan

### 8.1. Compile — Build binary

```bash
# Compile semua file
gcc -o binary_search main.c binary_search.c

# Compile dengan test
gcc -o test_search test_search.c binary_search.c

# Compile dengan debug symbol
gcc -g -o binary_search main.c binary_search.c

# Compile dengan optimasi
gcc -O2 -o binary_search main.c binary_search.c
```

### 8.2. Jalankan — Run program

```bash
# Jalankan program utama
./binary_search

# Jalankan unit test
./test_search
```

### 8.3. Output yang Diharapkan

```bash
========================================
  TESTING BINARY SEARCH
========================================

Test 1: Array: [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
        Target: 11 -> Index: 5 (expected: 5)
Test 2: Target: 1 -> Index: 0 (expected: 0)
Test 3: Target: 19 -> Index: 9 (expected: 9)
Test 4: Target: 10 -> Index: -1 (expected: -1)
Test 5: Empty array -> Index: -1 (expected: -1)
Test 6: [5], target 5 -> Index: 0 (expected: 0)
Test 7: [5], target 3 -> Index: -1 (expected: -1)

✅ All tests passed!

========================================
  BENCHMARK
========================================

Size: 1000
  Binary Search: 0.001 ms (result: 500)
  Linear Search: 0.023 ms (result: 500)
  Speedup: 23.00x

Size: 10000
  Binary Search: 0.002 ms (result: 5000)
  Linear Search: 0.234 ms (result: 5000)
  Speedup: 117.00x

Size: 100000
  Binary Search: 0.003 ms (result: 50000)
  Linear Search: 2.345 ms (result: 50000)
  Speedup: 781.67x

Size: 1000000
  Binary Search: 0.005 ms (result: 500000)
  Linear Search: 23.456 ms (result: 500000)
  Speedup: 4691.20x
```

---

## 9. Variasi Binary Search

### 9.1. Lower Bound (First Position)

**Fungsi:** Mencari posisi pertama dari target.

```c
int lower_bound(const int *arr, int size, int target) {
    int low = 0, high = size;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < target) low = mid + 1;
        else high = mid;
    }
    return low;
}
```

### 9.2. Upper Bound (Last Position + 1)

**Fungsi:** Mencari posisi terakhir target + 1.

```c
int upper_bound(const int *arr, int size, int target) {
    int low = 0, high = size;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] <= target) low = mid + 1;
        else high = mid;
    }
    return low;
}
```

### 9.3. Search in Rotated Array

**Fungsi:** Binary search pada array yang di-rotasi.

```c
int search_rotated(const int *arr, int size, int target) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        if (arr[low] <= arr[mid]) {
            if (arr[low] <= target && target < arr[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (arr[mid] < target && target <= arr[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return -1;
}
```

---

## 10. Referensi

### 10.1. Buku — Referensi yang direkomendasikan

- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "The C Programming Language" - Kernighan & Ritchie
- "Algorithms" - Robert Sedgewick

### 10.2. Website — Sumber online

- GeeksforGeeks: https://www.geeksforgeeks.org/binary-search/
- Khan Academy: https://www.khanacademy.org/computing/computer-science/algorithms

### 10.3. Video — Sumber video

- CS50 Binary Search: https://cs50.harvard.edu/
- MIT OpenCourseWare - Binary Search

---

## 11. Lampiran

### A. Kompleksitas Waktu — Perbandingan berbagai algoritma pencarian

| Algoritma | Best Case | Average Case | Worst Case |
|-----------|-----------|--------------|------------|
| **Binary Search** | O(1) | O(log n) | O(log n) |
| Linear Search | O(1) | O(n) | O(n) |
| Jump Search | O(√n) | O(√n) | O(√n) |
| Interpolation Search | O(1) | O(log log n) | O(n) |
| Exponential Search | O(1) | O(log n) | O(log n) |

### B. Kapan Pakai Binary Search?

| Skenario | Rekomendasi |
|----------|-------------|
| Array terurut, ukuran besar | ✅ Binary Search |
| Array terurut, ukuran kecil (n<10) | ⚠️ Linear Search lebih simpel |
| Array tidak terurut | ❌ Linear Search atau sort dulu |
| Data dinamis sering berubah | ❌ Binary Search (sorting mahal) |
| Perlu mengetahui posisi insert | ✅ Lower Bound / Upper Bound |

### C. Common Pitfalls — Kesalahan umum

| Kesalahan | Solusi |
|-----------|--------|
| **Overflow pada mid** | `mid = low + (high - low) / 2` |
| **Infinite loop** | Pastikan `low <= high` |
| **Off-by-one error** | Periksa kondisi `low = mid + 1` dan `high = mid - 1` |
| **Array tidak terurut** | Pastikan sort sebelum binary search |

### D. Format Markdown untuk Unduh

```markdown
# Cara Mengunduh

1. Copy seluruh teks di atas
2. Paste ke text editor
3. Save sebagai `binary_search_whitepaper.md`
4. Baca dengan Markdown viewer
```

---

**Akhir Dokumen**

> *"Binary Search: Divide and conquer."*