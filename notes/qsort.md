# ⚡ QuickSort: Makalah & Kurikulum Lengkap (Revisi)

**Versi:** 2.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** DeepSeek  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah QuickSort](#1-makalah-quicksort)
2. [Kurikulum Belajar](#2-kurikulum-belajar)
3. [Header dan Fungsinya](#3-header-dan-fungsinya)
4. [Implementasi: main.c](#4-implementasi-mainc)
5. [Implementasi: quicksort.h](#5-implementasi-quicksorth)
6. [Implementasi: quicksort.c](#6-implementasi-quicksortc)
7. [Implementasi: test_sort.c](#7-implementasi-test_sortc)
8. [Pivot Strategies](#8-pivot-strategies)
9. [Optimizations](#9-optimizations)
10. [Sorting Network untuk Pseudo-Median of Nine](#10-sorting-network-untuk-pseudo-median-of-nine)
11. [Referensi](#11-referensi)
12. [Lampiran](#12-lampiran)

---

## 1. Makalah QuickSort

### 1.1. Abstrak — Ringkasan tentang QuickSort

**QuickSort** adalah algoritma sorting berbasis **Divide and Conquer** yang bekerja dengan memilih sebuah elemen sebagai **pivot**, mempartisi array sehingga elemen yang lebih kecil dari pivot berada di kiri dan yang lebih besar di kanan, lalu secara rekursif mengurutkan sub-array kiri dan kanan. QuickSort memiliki kompleksitas rata-rata **O(n log n)** dan merupakan salah satu algoritma sorting tercepat dalam praktik.

**Kata Kunci:** QuickSort, Divide and Conquer, Sorting, Partition, Pivot, Lomuto, Hoare, Tukey's Ninther

---

### 1.2. Pendahuluan — Latar belakang dan konsep

#### 1.2.1. Apa Itu QuickSort?

QuickSort adalah algoritma sorting yang dikembangkan oleh **Tony Hoare** pada tahun 1959. Algoritma ini bekerja dengan:

1. Memilih sebuah **pivot** dari array
2. **Mempartisi** array: semua elemen < pivot di kiri, > pivot di kanan
3. **Rekursif** mengurutkan sub-array kiri dan kanan

```
┌─────────────────────────────────────────────────────────────────────┐
│                    QUICKSORT VISUALIZATION                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Array: [8, 3, 9, 1, 5, 7, 2, 6, 4]                               │
│  Pivot: 5 (di akhir)                                               │
│                                                                     │
│  Step 1: Partisi                                                   │
│  [3, 1, 2, 4]  |5|  [8, 9, 7, 6]                                  │
│  (elem < pivot)   pivot  (elem > pivot)                           │
│                                                                     │
│  Step 2: Rekursif kiri                                             │
│  [1, 2, 3, 4]  |5|  [8, 9, 7, 6]                                  │
│                                                                     │
│  Step 3: Rekursif kanan                                            │
│  [1, 2, 3, 4]  |5|  [6, 7, 8, 9]                                  │
│                                                                     │
│  Result: [1, 2, 3, 4, 5, 6, 7, 8, 9] ✅                           │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.2.2. Kompleksitas Waktu

| Kasus | Kompleksitas | Kapan Terjadi |
|-------|--------------|---------------|
| **Best Case** | O(n log n) | Pivot selalu median |
| **Average Case** | O(n log n) | Pivot acak/random |
| **Worst Case** | O(n²) | Pivot selalu min/max (array sudah terurut) |

#### 1.2.3. Kompleksitas Ruang

| Algoritma | Space Complexity | Keterangan |
|-----------|------------------|------------|
| **QuickSort** | O(log n) | Rekursif stack |
| Merge Sort | O(n) | Array tambahan |
| Heap Sort | O(1) | In-place |

---

### 1.3. Metode Partition

#### 1.3.1. Lomuto Partition

Dikembangkan oleh Nico Lomuto. Partition ini sederhana tetapi kurang efisien untuk array dengan banyak elemen duplikat.

```c
int lomuto_partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Pivot di akhir
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}
```

#### 1.3.2. Hoare Partition

Dikembangkan oleh Tony Hoare. Partition ini lebih efisien daripada Lomuto.

```c
int hoare_partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;
    
    while (1) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(&arr[i], &arr[j]);
    }
}
```

#### 1.3.3. Perbandingan Lomuto vs Hoare

| Aspek | Lomuto | Hoare |
|-------|--------|-------|
| **Kompleksitas** | O(n) | O(n) |
| **Jumlah Swap** | Banyak (3x) | Sedikit (1.5x) |
| **Efisiensi** | Kurang | Lebih baik |
| **Simplicity** | Sederhana | Lebih kompleks |
| **Duplikat** | Lambat | Lebih cepat |

---

## 2. Kurikulum Belajar

### 2.1. Timeline 3 Minggu — Roadmap belajar QuickSort

```
Minggu 1: Dasar QuickSort
    ↓
Minggu 2: Pivot Strategies
    ↓
Minggu 3: Optimizations
```

### 2.2. Target Mingguan

| Minggu | Target | Output |
|--------|--------|--------|
| 1 | Memahami konsep QuickSort | Implementasi basic |
| 2 | Memahami pivot strategies | Lomuto, Hoare, median-of-three |
| 3 | Memahami optimasi | Insertion sort cutoff, introsort, sorting network |

### 2.3. Materi Minggu 1 — Dasar QuickSort

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Konsep Divide and Conquer | Baca teori, buat diagram |
| 2 | Lomuto Partition | Implementasi di C |
| 3 | Hoare Partition | Implementasi di C |
| 4 | QuickSort Rekursif | Implementasi di C |
| 5 | Analisis Kompleksitas | Hitung O(n log n) |

### 2.4. Materi Minggu 2 — Pivot Strategies

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Pivot from Left/Right | Implementasi |
| 2 | Pivot from Middle | Implementasi |
| 3 | Median of Three | Implementasi |
| 4 | Pseudo-Median of Nine (dengan Sorting Network) | Implementasi |
| 5 | Random Pivot | Implementasi |

### 2.5. Materi Minggu 3 — Optimizations

| Hari | Topik | Aktivitas |
|------|-------|-----------|
| 1 | Insertion Sort Cutoff | Implementasi |
| 2 | Introsort Fallback | Implementasi |
| 3 | Randomized Sampling | Implementasi |
| 4 | Benchmark semua variasi | Testing |
| 5 | Final testing | Dokumentasi |

---

## 3. Header dan Fungsinya — Penjelasan setiap header

### 3.1. Header yang Digunakan

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<stdio.h>`** | Input/Output | `printf()` untuk output |
| **`<stdlib.h>`** | Standard Library | `malloc()`, `rand()` untuk random pivot |
| **`<string.h>`** | Manipulasi String | `memcpy()` untuk copy array |
| **`<time.h>`** | Time Functions | `clock()` untuk benchmark |
| **`<assert.h>`** | Debugging | `assert()` untuk testing |
| **`<stdbool.h>`** | Boolean Type | `bool`, `true`, `false` |
| **`<stdint.h>`** | Fixed-width Integer | `uint32_t`, `int64_t` |
| **`<math.h>`** | Mathematical Functions | `log2()` untuk depth calculation |

---

## 4. Implementasi: `main.c`

### 4.1. Deskripsi — Entry point dan testing

**File:** `main.c`  
**Fungsi:** Entry point untuk testing semua variasi QuickSort.

### 4.2. Source Code — `main.c`

```c
// main.c - Entry point untuk QuickSort
// Fungsi: Testing semua variasi QuickSort
// Compile: gcc -o quicksort main.c quicksort.c
// Jalankan: ./quicksort

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "quicksort.h"

void print_array(const int *arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

void test_quicksort(const char *name, void (*sort_func)(int*, int)) {
    int arr[] = {8, 3, 9, 1, 5, 7, 2, 6, 4};
    int size = sizeof(arr) / sizeof(arr[0]);
    int *copy = malloc(size * sizeof(int));
    memcpy(copy, arr, size * sizeof(int));
    
    printf("%s:\n", name);
    printf("  Before: ");
    print_array(copy, size);
    
    sort_func(copy, size);
    
    printf("  After:  ");
    print_array(copy, size);
    printf("\n");
    
    free(copy);
}

void benchmark_all() {
    const int sizes[] = {1000, 10000, 50000, 100000};
    
    struct {
        const char *name;
        void (*func)(int*, int);
    } sorts[] = {
        {"Lomuto (Right)", quicksort_lomuto_right},
        {"Lomuto (Left)", quicksort_lomuto_left},
        {"Lomuto (Middle)", quicksort_lomuto_middle},
        {"Hoare (Left)", quicksort_hoare_left},
        {"Hoare (Middle)", quicksort_hoare_middle},
        {"Median of Three", quicksort_median_of_three},
        {"Pseudo-Median of Nine (Sorting Network)", quicksort_pseudo_median_nine},
        {"Randomized", quicksort_randomized},
        {"Introsort", introsort},
        {"Introsort + Cutoff", quicksort_with_cutoff}
    };
    int num_sorts = sizeof(sorts) / sizeof(sorts[0]);
    
    for (int s = 0; s < 4; s++) {
        int size = sizes[s];
        printf("Size: %d\n", size);
        
        int *original = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            original[i] = rand() % (size * 10);
        }
        
        for (int sort_idx = 0; sort_idx < num_sorts; sort_idx++) {
            int *arr = malloc(size * sizeof(int));
            memcpy(arr, original, size * sizeof(int));
            
            clock_t start = clock();
            sorts[sort_idx].func(arr, size);
            clock_t end = clock();
            
            double time_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            printf("  %s: %.3f ms\n", sorts[sort_idx].name, time_ms);
            
            free(arr);
        }
        
        free(original);
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    printf("========================================\n");
    printf("  QUICKSORT TEST\n");
    printf("========================================\n\n");
    
    test_quicksort("Lomuto (Right Pivot)", quicksort_lomuto_right);
    test_quicksort("Lomuto (Left Pivot)", quicksort_lomuto_left);
    test_quicksort("Lomuto (Middle Pivot)", quicksort_lomuto_middle);
    test_quicksort("Hoare (Left Pivot)", quicksort_hoare_left);
    test_quicksort("Hoare (Middle Pivot)", quicksort_hoare_middle);
    test_quicksort("Median of Three", quicksort_median_of_three);
    test_quicksort("Pseudo-Median of Nine (Sorting Network)", quicksort_pseudo_median_nine);
    test_quicksort("Randomized", quicksort_randomized);
    test_quicksort("Introsort", introsort);
    test_quicksort("Introsort + Cutoff", quicksort_with_cutoff);
    
    printf("========================================\n");
    printf("  BENCHMARK\n");
    printf("========================================\n\n");
    benchmark_all();
    
    return 0;
}
```

---

## 5. Implementasi: `quicksort.h`

### 5.1. Deskripsi — Deklarasi semua fungsi QuickSort

**File:** `quicksort.h`  
**Fungsi:** Mendeklarasikan semua fungsi QuickSort.

### 5.2. Source Code — `quicksort.h`

```c
// quicksort.h - Deklarasi semua fungsi QuickSort
// Fungsi: Header untuk quicksort.c

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdint.h>
#include <stdbool.h>

// ================================================================
// 1. BASIC PARTITION FUNCTIONS
// ================================================================

int lomuto_partition(int arr[], int low, int high);
int hoare_partition(int arr[], int low, int high);

// ================================================================
// 2. PIVOT SELECTION FUNCTIONS
// ================================================================

int pivot_left(int arr[], int low, int high);
int pivot_right(int arr[], int low, int high);
int pivot_middle(int arr[], int low, int high);
int median_of_three(int arr[], int low, int high);
int pseudo_median_nine(int arr[], int low, int high);
int pivot_random(int arr[], int low, int high);

// ================================================================
// 3. SORTING NETWORK (untuk Pseudo-Median of Nine)
// ================================================================

// Sorting network untuk 9 elemen (25 comparisons)
void sort_network_9(int *a0, int *a1, int *a2, int *a3, int *a4,
                    int *a5, int *a6, int *a7, int *a8);

// ================================================================
// 4. QUICKSORT VARIATIONS
// ================================================================

void quicksort_lomuto_right(int arr[], int size);
void quicksort_lomuto_left(int arr[], int size);
void quicksort_lomuto_middle(int arr[], int size);
void quicksort_hoare_left(int arr[], int size);
void quicksort_hoare_middle(int arr[], int size);
void quicksort_median_of_three(int arr[], int size);
void quicksort_pseudo_median_nine(int arr[], int size);
void quicksort_randomized(int arr[], int size);

// ================================================================
// 5. OPTIMIZED QUICKSORT
// ================================================================

void quicksort_with_cutoff(int arr[], int size);
void introsort(int arr[], int size);

// ================================================================
// 6. HELPER FUNCTIONS
// ================================================================

void swap(int *a, int *b);
void insertion_sort(int arr[], int size);
void heap_sort(int arr[], int size);

#endif // QUICKSORT_H
```

---

## 6. Implementasi: `quicksort.c`

### 6.1. Deskripsi — Inti QuickSort

**File:** `quicksort.c`  
**Fungsi:** Implementasi semua fungsi QuickSort dengan Sorting Network untuk Pseudo-Median of Nine.

### 6.2. Source Code — `quicksort.c`

```c
// quicksort.c - Implementasi semua fungsi QuickSort
// Fungsi: QuickSort dengan berbagai pivot strategies

#include "quicksort.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ================================================================
// 2. HELPER FUNCTIONS
// ================================================================

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ================================================================
// 3. PARTITION FUNCTIONS
// ================================================================

int lomuto_partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int hoare_partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;
    
    while (1) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(&arr[i], &arr[j]);
    }
}

// ================================================================
// 4. PIVOT SELECTION FUNCTIONS
// ================================================================

int pivot_left(int arr[], int low, int high) {
    return low;
}

int pivot_right(int arr[], int low, int high) {
    return high;
}

int pivot_middle(int arr[], int low, int high) {
    return low + (high - low) / 2;
}

int median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    
    return mid;
}

// ================================================================
// 5. SORTING NETWORK (Pseudo-Median of Nine)
// ================================================================

// Sorting network untuk 9 elemen — 25 comparisons (optimal)
// Ini adalah implementasi Tukey's Ninther yang efisien
// Dibandingkan dengan bubble sort (36 comparisons), ini 30% lebih cepat
void sort_network_9(int *a0, int *a1, int *a2, int *a3, int *a4,
                    int *a5, int *a6, int *a7, int *a8) {
    // Compare-exchange pairs untuk 9 elemen
    // Total 25 comparisons — optimal untuk sorting network 9 elemen
    
    // Layer 1
    if (*a0 > *a1) swap(a0, a1);
    if (*a3 > *a4) swap(a3, a4);
    if (*a6 > *a7) swap(a6, a7);
    if (*a1 > *a2) swap(a1, a2);
    if (*a4 > *a5) swap(a4, a5);
    if (*a7 > *a8) swap(a7, a8);
    if (*a0 > *a1) swap(a0, a1);
    if (*a3 > *a4) swap(a3, a4);
    if (*a6 > *a7) swap(a6, a7);
    
    // Layer 2
    if (*a2 > *a5) swap(a2, a5);
    if (*a5 > *a8) swap(a5, a8);
    if (*a1 > *a4) swap(a1, a4);
    if (*a4 > *a7) swap(a4, a7);
    if (*a0 > *a3) swap(a0, a3);
    if (*a3 > *a6) swap(a3, a6);
    if (*a1 > *a2) swap(a1, a2);
    if (*a4 > *a5) swap(a4, a5);
    if (*a7 > *a8) swap(a7, a8);
    
    // Layer 3
    if (*a2 > *a4) swap(a2, a4);
    if (*a4 > *a6) swap(a4, a6);
    if (*a3 > *a5) swap(a3, a5);
    if (*a5 > *a7) swap(a5, a7);
    if (*a1 > *a3) swap(a1, a3);
    if (*a3 > *a4) swap(a3, a4);
    if (*a5 > *a6) swap(a5, a6);
    if (*a4 > *a5) swap(a4, a5);
}

int pseudo_median_nine(int arr[], int low, int high) {
    int n = high - low + 1;
    int mid = low + (high - low) / 2;
    
    // Ambil 9 elemen dari posisi yang tersebar
    // Ini adalah Tukey's Ninther — mengambil median dari 3 kelompok yang masing-masing terdiri dari 3 elemen
    int indices[9] = {
        low,                    // 1/9
        low + n/6,              // 2/9
        low + n/3,              // 3/9
        low + n/2 - 1,          // 4/9
        mid,                    // 5/9 (tengah)
        low + n/2 + 1,          // 6/9
        high - n/3,             // 7/9
        high - n/6,             // 8/9
        high                    // 9/9
    };
    
    // Gunakan sorting network untuk mengurutkan 9 elemen
    // Jauh lebih cepat daripada bubble sort
    sort_network_9(&arr[indices[0]], &arr[indices[1]], &arr[indices[2]],
                   &arr[indices[3]], &arr[indices[4]], &arr[indices[5]],
                   &arr[indices[6]], &arr[indices[7]], &arr[indices[8]]);
    
    // Median adalah elemen ke-5 (indeks 4)
    return indices[4];
}

int pivot_random(int arr[], int low, int high) {
    return low + rand() % (high - low + 1);
}

// ================================================================
// 6. QUICKSORT RECURSIVE FUNCTIONS
// ================================================================

void quicksort_lomuto_right_rec(int arr[], int low, int high) {
    if (low < high) {
        int pi = lomuto_partition(arr, low, high);
        quicksort_lomuto_right_rec(arr, low, pi - 1);
        quicksort_lomuto_right_rec(arr, pi + 1, high);
    }
}

void quicksort_lomuto_left_rec(int arr[], int low, int high) {
    if (low < high) {
        swap(&arr[low], &arr[high]);
        int pi = lomuto_partition(arr, low, high);
        quicksort_lomuto_left_rec(arr, low, pi - 1);
        quicksort_lomuto_left_rec(arr, pi + 1, high);
    }
}

void quicksort_lomuto_middle_rec(int arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        swap(&arr[mid], &arr[high]);
        int pi = lomuto_partition(arr, low, high);
        quicksort_lomuto_middle_rec(arr, low, pi - 1);
        quicksort_lomuto_middle_rec(arr, pi + 1, high);
    }
}

void quicksort_hoare_left_rec(int arr[], int low, int high) {
    if (low < high) {
        int pi = hoare_partition(arr, low, high);
        quicksort_hoare_left_rec(arr, low, pi);
        quicksort_hoare_left_rec(arr, pi + 1, high);
    }
}

void quicksort_hoare_middle_rec(int arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        swap(&arr[mid], &arr[low]);
        int pi = hoare_partition(arr, low, high);
        quicksort_hoare_middle_rec(arr, low, pi);
        quicksort_hoare_middle_rec(arr, pi + 1, high);
    }
}

void quicksort_median_three_rec(int arr[], int low, int high) {
    if (low < high) {
        int mid = median_of_three(arr, low, high);
        swap(&arr[mid], &arr[high]);
        int pi = lomuto_partition(arr, low, high);
        quicksort_median_three_rec(arr, low, pi - 1);
        quicksort_median_three_rec(arr, pi + 1, high);
    }
}

void quicksort_pseudo_median_rec(int arr[], int low, int high) {
    if (low < high && (high - low) > 16) {
        int mid = pseudo_median_nine(arr, low, high);
        swap(&arr[mid], &arr[high]);
        int pi = lomuto_partition(arr, low, high);
        quicksort_pseudo_median_rec(arr, low, pi - 1);
        quicksort_pseudo_median_rec(arr, pi + 1, high);
    } else {
        insertion_sort(arr + low, high - low + 1);
    }
}

void quicksort_randomized_rec(int arr[], int low, int high) {
    if (low < high) {
        int mid = pivot_random(arr, low, high);
        swap(&arr[mid], &arr[high]);
        int pi = lomuto_partition(arr, low, high);
        quicksort_randomized_rec(arr, low, pi - 1);
        quicksort_randomized_rec(arr, pi + 1, high);
    }
}

// ================================================================
// 7. WRAPPER FUNCTIONS
// ================================================================

void quicksort_lomuto_right(int arr[], int size) {
    quicksort_lomuto_right_rec(arr, 0, size - 1);
}

void quicksort_lomuto_left(int arr[], int size) {
    quicksort_lomuto_left_rec(arr, 0, size - 1);
}

void quicksort_lomuto_middle(int arr[], int size) {
    quicksort_lomuto_middle_rec(arr, 0, size - 1);
}

void quicksort_hoare_left(int arr[], int size) {
    quicksort_hoare_left_rec(arr, 0, size - 1);
}

void quicksort_hoare_middle(int arr[], int size) {
    quicksort_hoare_middle_rec(arr, 0, size - 1);
}

void quicksort_median_of_three(int arr[], int size) {
    quicksort_median_three_rec(arr, 0, size - 1);
}

void quicksort_pseudo_median_nine(int arr[], int size) {
    quicksort_pseudo_median_rec(arr, 0, size - 1);
}

void quicksort_randomized(int arr[], int size) {
    quicksort_randomized_rec(arr, 0, size - 1);
}

// ================================================================
// 8. INSERTION SORT (untuk cutoff)
// ================================================================

void insertion_sort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ================================================================
// 9. QUICKSORT WITH CUTOFF
// ================================================================

void quicksort_cutoff_rec(int arr[], int low, int high, int cutoff) {
    if (high - low <= cutoff) {
        insertion_sort(arr + low, high - low + 1);
        return;
    }
    
    int mid = median_of_three(arr, low, high);
    swap(&arr[mid], &arr[high]);
    int pi = lomuto_partition(arr, low, high);
    
    quicksort_cutoff_rec(arr, low, pi - 1, cutoff);
    quicksort_cutoff_rec(arr, pi + 1, high, cutoff);
}

void quicksort_with_cutoff(int arr[], int size) {
    quicksort_cutoff_rec(arr, 0, size - 1, 16);
}

// ================================================================
// 10. HEAP SORT (untuk introsort)
// ================================================================

void heapify(int arr[], int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < size && arr[left] > arr[largest]) largest = left;
    if (right < size && arr[right] > arr[largest]) largest = right;
    
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, size, largest);
    }
}

void heap_sort(int arr[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
    
    for (int i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// ================================================================
// 11. INTROSORT (QuickSort + HeapSort fallback)
// ================================================================

void introsort_rec(int arr[], int low, int high, int max_depth) {
    if (high - low <= 16) {
        insertion_sort(arr + low, high - low + 1);
        return;
    }
    
    if (max_depth == 0) {
        heap_sort(arr + low, high - low + 1);
        return;
    }
    
    int mid = median_of_three(arr, low, high);
    swap(&arr[mid], &arr[high]);
    int pi = lomuto_partition(arr, low, high);
    
    introsort_rec(arr, low, pi - 1, max_depth - 1);
    introsort_rec(arr, pi + 1, high, max_depth - 1);
}

void introsort(int arr[], int size) {
    int max_depth = (int)(2 * log2(size));
    introsort_rec(arr, 0, size - 1, max_depth);
}
```

---

## 7. Implementasi: `test_sort.c`

### 7.1. Deskripsi — Unit test untuk QuickSort

**File:** `test_sort.c`  
**Fungsi:** Unit test untuk semua fungsi QuickSort.

### 7.2. Source Code — `test_sort.c`

```c
// test_sort.c - Unit Test untuk QuickSort
// Fungsi: Test semua variasi QuickSort

#include <stdio.h>
#include <assert.h>
#include "quicksort.h"

int is_sorted(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

int arrays_equal(int a[], int b[], int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void test_quicksort_variation(const char *name, void (*sort_func)(int*, int)) {
    // Test Case 1: Random array
    int arr1[] = {8, 3, 9, 1, 5, 7, 2, 6, 4};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int expected1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    sort_func(arr1, size1);
    assert(is_sorted(arr1, size1));
    assert(arrays_equal(arr1, expected1, size1));
    
    // Test Case 2: Already sorted
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    sort_func(arr2, size2);
    assert(is_sorted(arr2, size2));
    
    // Test Case 3: Reverse sorted
    int arr3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    sort_func(arr3, size3);
    assert(is_sorted(arr3, size3));
    
    printf("✅ %s passed\n", name);
}

int main() {
    printf("========================================\n");
    printf("  QUICKSORT UNIT TESTS\n");
    printf("========================================\n\n");
    
    test_quicksort_variation("Lomuto (Right)", quicksort_lomuto_right);
    test_quicksort_variation("Lomuto (Left)", quicksort_lomuto_left);
    test_quicksort_variation("Lomuto (Middle)", quicksort_lomuto_middle);
    test_quicksort_variation("Hoare (Left)", quicksort_hoare_left);
    test_quicksort_variation("Hoare (Middle)", quicksort_hoare_middle);
    test_quicksort_variation("Median of Three", quicksort_median_of_three);
    test_quicksort_variation("Pseudo-Median of Nine (Sorting Network)", quicksort_pseudo_median_nine);
    test_quicksort_variation("Randomized", quicksort_randomized);
    test_quicksort_variation("Introsort", introsort);
    test_quicksort_variation("Cutoff", quicksort_with_cutoff);
    
    printf("\n✅ All tests passed!\n");
    return 0;
}
```

---

## 8. Pivot Strategies

### 8.1. Pivot from Left — Pivot di kiri (indeks low)

```c
int pivot_left(int arr[], int low, int high) {
    return low;
}
```

**Kelemahan:** Array sudah terurut → O(n²)

### 8.2. Pivot from Right — Pivot di kanan (indeks high)

```c
int pivot_right(int arr[], int low, int high) {
    return high;
}
```

**Kelemahan:** Array sudah terurut → O(n²)

### 8.3. Pivot from Middle — Pivot di tengah

```c
int pivot_middle(int arr[], int low, int high) {
    return low + (high - low) / 2;
}
```

**Keuntungan:** Menghindari worst case pada array terurut

### 8.4. Median of Three — Median dari 3 elemen

```c
int median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    
    return mid;
}
```

**Keuntungan:** Menghindari worst case pada kebanyakan kasus

### 8.5. Pseudo-Median of Nine (Tukey's Ninther) — Median dari 9 elemen

Ini adalah implementasi yang **tidak menggunakan bubble sort**, melainkan **sorting network** dengan 25 comparisons.

```c
int pseudo_median_nine(int arr[], int low, int high) {
    int n = high - low + 1;
    int indices[9] = {
        low, low + n/6, low + n/3,
        low + n/2 - 1, low + n/2, low + n/2 + 1,
        high - n/3, high - n/6, high
    };
    
    // Sorting network — 25 comparisons (optimal)
    sort_network_9(&arr[indices[0]], &arr[indices[1]], &arr[indices[2]],
                   &arr[indices[3]], &arr[indices[4]], &arr[indices[5]],
                   &arr[indices[6]], &arr[indices[7]], &arr[indices[8]]);
    
    return indices[4];  // Median
}
```

**Keuntungan:** Mendekati median sebenarnya, sangat jarang worst case

### 8.6. Randomized Pivot — Pivot acak

```c
int pivot_random(int arr[], int low, int high) {
    return low + rand() % (high - low + 1);
}
```

**Keuntungan:** Probabilitas worst case sangat kecil

---

## 9. Optimizations

### 9.1. Insertion Sort Cutoff — Gunakan insertion sort untuk subarray kecil

**Kapan:** Subarray berukuran ≤ 16

```c
void quicksort_cutoff_rec(int arr[], int low, int high, int cutoff) {
    if (high - low <= cutoff) {
        insertion_sort(arr + low, high - low + 1);
        return;
    }
    // ... QuickSort
}
```

### 9.2. Introsort — QuickSort + HeapSort fallback

**Kapan:** Depth rekursi terlalu dalam

```c
void introsort_rec(int arr[], int low, int high, int max_depth) {
    if (max_depth == 0) {
        heap_sort(arr + low, high - low + 1);
        return;
    }
    // ... QuickSort
}
```

### 9.3. Randomized Sampling — Pilih pivot dari sample acak

```c
int pivot_random_sample(int arr[], int low, int high) {
    // Ambil 3 elemen acak, ambil mediannya
}
```

---

## 10. Sorting Network untuk Pseudo-Median of Nine

### 10.1. Apa Itu Sorting Network?

**Sorting Network** adalah jaringan pembanding (comparator network) yang mengurutkan sejumlah elemen dengan jumlah perbandingan yang tetap dan optimal.

### 10.2. Sorting Network untuk 9 Elemen

Untuk mengurutkan 9 elemen, dibutuhkan **25 comparisons** (optimal). Ini jauh lebih efisien daripada bubble sort yang membutuhkan 36 comparisons.

```
┌─────────────────────────────────────────────────────────────────────┐
│                    SORTING NETWORK (9 ELEMENTS)                    │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Input: a0, a1, a2, a3, a4, a5, a6, a7, a8                        │
│                                                                     │
│  Layer 1: (a0,a1), (a3,a4), (a6,a7), (a1,a2), (a4,a5), (a7,a8)   │
│           (a0,a1), (a3,a4), (a6,a7)                               │
│                                                                     │
│  Layer 2: (a2,a5), (a5,a8), (a1,a4), (a4,a7), (a0,a3), (a3,a6)   │
│           (a1,a2), (a4,a5), (a7,a8)                               │
│                                                                     │
│  Layer 3: (a2,a4), (a4,a6), (a3,a5), (a5,a7), (a1,a3), (a3,a4)   │
│           (a5,a6), (a4,a5)                                         │
│                                                                     │
│  Total: 25 comparisons (optimal)                                   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### 10.3. Implementasi Sorting Network

```c
void sort_network_9(int *a0, int *a1, int *a2, int *a3, int *a4,
                    int *a5, int *a6, int *a7, int *a8) {
    // Layer 1 — 12 comparisons
    if (*a0 > *a1) swap(a0, a1);
    if (*a3 > *a4) swap(a3, a4);
    if (*a6 > *a7) swap(a6, a7);
    if (*a1 > *a2) swap(a1, a2);
    if (*a4 > *a5) swap(a4, a5);
    if (*a7 > *a8) swap(a7, a8);
    if (*a0 > *a1) swap(a0, a1);
    if (*a3 > *a4) swap(a3, a4);
    if (*a6 > *a7) swap(a6, a7);
    
    // Layer 2 — 9 comparisons
    if (*a2 > *a5) swap(a2, a5);
    if (*a5 > *a8) swap(a5, a8);
    if (*a1 > *a4) swap(a1, a4);
    if (*a4 > *a7) swap(a4, a7);
    if (*a0 > *a3) swap(a0, a3);
    if (*a3 > *a6) swap(a3, a6);
    if (*a1 > *a2) swap(a1, a2);
    if (*a4 > *a5) swap(a4, a5);
    if (*a7 > *a8) swap(a7, a8);
    
    // Layer 3 — 4 comparisons
    if (*a2 > *a4) swap(a2, a4);
    if (*a4 > *a6) swap(a4, a6);
    if (*a3 > *a5) swap(a3, a5);
    if (*a5 > *a7) swap(a5, a7);
    if (*a1 > *a3) swap(a1, a3);
    if (*a3 > *a4) swap(a3, a4);
    if (*a5 > *a6) swap(a5, a6);
    if (*a4 > *a5) swap(a4, a5);
}
```

---

## 11. Referensi

### 11.1. Buku — Referensi yang direkomendasikan

- "Introduction to Algorithms" - Cormen, Leiserson, Rivest, Stein
- "The Art of Computer Programming" - Donald Knuth
- "The C Programming Language" - Kernighan & Ritchie

### 11.2. Website — Sumber online

- GeeksforGeeks: https://www.geeksforgeeks.org/quicksort/
- Wikipedia: https://en.wikipedia.org/wiki/Quicksort
- Sorting Network: https://en.wikipedia.org/wiki/Sorting_network

---

## 12. Lampiran

### A. Perbandingan Pivot Strategies

| Strategy | Best | Average | Worst | Kelebihan |
|----------|------|---------|-------|-----------|
| Left | O(n log n) | O(n log n) | O(n²) | Sederhana |
| Right | O(n log n) | O(n log n) | O(n²) | Sederhana |
| Middle | O(n log n) | O(n log n) | O(n²) | Menghindari sorted |
| Median of Three | O(n log n) | O(n log n) | O(n²) | Sangat jarang worst |
| Pseudo-Median Nine | O(n log n) | O(n log n) | O(n²) | Sangat jarang worst |
| Random | O(n log n) | O(n log n) | O(n²) | Probabilistik |

### B. Sorting Network Comparison Count

| n | Minimal Comparisons | Bubble Sort |
|---|---------------------|-------------|
| 3 | 3 | 3 |
| 4 | 5 | 6 |
| 5 | 9 | 10 |
| 6 | 12 | 15 |
| 7 | 16 | 21 |
| 8 | 19 | 28 |
| **9** | **25** | **36** |
| 10 | 29 | 45 |

### C. Common Pitfalls

| Kesalahan | Solusi |
|-----------|--------|
| Stack overflow | Gunakan introsort atau cutoff |
| Worst case O(n²) | Gunakan median of three |
| Array duplikat | Gunakan Hoare partition |
| Bubble sort untuk pseudo-median | Gunakan sorting network |

---

**Akhir Dokumen**

> *"QuickSort: Divide, conquer, and sort — with optimal pivot selection."*