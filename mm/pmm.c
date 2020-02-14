/*
    pmm.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#include <mm/pmm.h>

/*********************
 * Public Memory API *
 *********************/
void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes) {
	for (uint32_t i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memset(void* dest, int val, size_t len) {
    for (uint8_t *temp = dest; len--;)
        *temp++ = val;
}

/*******************
 * Private PMM API *
 *******************/

/* Physical Memory Allocation */
uint64_t* pmalloc(size_t bytes) {
  uint64_t pages = bytes / PAGESIZE;
  index_t* index = getFreeIndicies(pages);

  if (index == NULL) return NULL; // no contiguous bits found

  uint64_t pagesMarked = 0;
  uint64_t address = 0;
  uint64_t pagesToAllocate = 0; // allocate in next entry

  for (uint64_t i = 0; i < index->entries; i++) {
    uint64_t entry = bitmap[i];

    if (pagesMarked > 0) {

      for (uint64_t s = 0; s < pagesToAllocate; s++) {
        sbit(bitmap, i * s);
      }

    } else {

      if (index->row != i) {
        continue;
      } else {
        for (uint64_t j = 0; j < 63; j++) {

          if (j == index->bit) {
            // set bits j + pages with a for loop
            // calculate the address of index->bit
            // address = ((((i - 1) * 64) * PAGESIZE) + (j * PAGESIZE)) + MEMBASE
            if (j + pages > 63) {
              uint64_t bitsLeftInEntry = 64 - j;
              pagesToAllocate = pages - bitsLeftInEntry;

              for (uint64_t k = 0; k < bitsLeftInEntry; k++) {
                sbit(bitmap, i * j); // you have to submit the current bit in the bitmap
              }

              break; // move to next entry

            } else {
              sbit(bitmap, i * j);
              pagesMarked++;

              if (pagesMarked == pages) {
                // calculate the address
                address = ((((i - 1) * 64) * PAGESIZE) + (j * PAGESIZE)) + MEMBASE;
                goto done;
              }
            }
          } 
        }
      }
    }
  }

  return NULL;

  done:
    return (uint64_t*)address;
}

void pmfree(void* ptr, size_t bytes) {
  uint64_t pages = bytes / PAGESIZE;

  UNUSED(ptr);
}