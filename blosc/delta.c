/*********************************************************************
  Blosc - Blocked Shuffling and Compression Library

  Author: Francesc Alted <francesc@blosc.org>
  Creation date: 2015-12-18

  See LICENSES/BLOSC.txt for details about copyright and rights to use.
**********************************************************************/

#include "blosc.h"
#include "delta.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))


int delta_encoder8(void* filters_chunk, int32_t offset, int32_t nbytes,
                   uint8_t* src, uint8_t* dest) {
  int i;
  int32_t rbytes = *(int32_t*)(filters_chunk + 4);
  int32_t mbytes;
  uint8_t* dref = (uint8_t*)filters_chunk + BLOSC_MAX_OVERHEAD;

  nbytes += offset;
  mbytes = MIN(nbytes, rbytes);

  /* Encode delta */
  for (i = offset; i < mbytes; i++) {
    dest[i] = src[i] - dref[i];
  }

  /* Copy the leftovers */
  if (nbytes > rbytes) {
    for (i = rbytes; i < nbytes; i++) {
      dest[i] = src[i];
    }
  }

  /* This can't never fail */
  return 0;
}


int delta_decoder8(void* filters_chunk, int32_t offset, int32_t nbytes, uint8_t* dest) {
  int i;
  uint8_t* dref = (uint8_t*)filters_chunk + BLOSC_MAX_OVERHEAD;
  int32_t rbytes = *(int32_t*)(filters_chunk + 4);
  int32_t mbytes;

  nbytes += offset;
  mbytes = MIN(nbytes, rbytes);

  /* Decode delta */
  for (i = offset; i < mbytes; i++) {
    dest[i] += dref[i];
  }

  /* The leftovers are in-place already */

  /* This can't never fail */
  return 0;
}
