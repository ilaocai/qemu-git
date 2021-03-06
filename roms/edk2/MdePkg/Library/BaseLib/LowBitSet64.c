/** @file
  Math worker functions.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BaseLibInternals.h"

/**
  Returns the bit position of the lowest bit set in a 64-bit value.

  This function computes the bit position of the lowest bit set in the 64-bit
  value specified by Operand. If Operand is zero, then -1 is returned.
  Otherwise, a value between 0 and 63 is returned.

  @param  Operand The 64-bit operand to evaluate.

  @retval 0..63  The lowest bit set in Operand was found.
  @retval -1     Operand is zero.


**/
INTN
EFIAPI
LowBitSet64 (
  IN      UINT64  Operand
  )
{
  INTN  BitIndex;

  if (Operand == 0) {
    return -1;
  }

  for (BitIndex = 0;
       (Operand & 1) == 0;
       BitIndex++, Operand = RShiftU64 (Operand, 1))
  {
  }

  return BitIndex;
}
