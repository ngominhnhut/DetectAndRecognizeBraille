//
// File: bsxfun.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 20-Mar-2020 16:56:54
//

// Include Files
#include "rt_nonfinite.h"
#include "regconize2.h"
#include "bsxfun.h"

// Function Definitions

//
// Arguments    : const double a[560]
//                const double b[560]
//                double c[560]
// Return Type  : void
//
void b_bsxfun(const double a[560], const double b[560], double c[560])
{
  int k;
  for (k = 0; k < 560; k++) {
    c[k] = a[k] * b[k];
  }
}

//
// Arguments    : const double a[560]
//                double c[560]
// Return Type  : void
//
void bsxfun(const double a[560], double c[560])
{
  int k;
  for (k = 0; k < 560; k++) {
    c[k] = a[k];
  }
}

//
// Arguments    : const double a[560]
//                double c[560]
// Return Type  : void
//
void c_bsxfun(const double a[560], double c[560])
{
  int k;
  for (k = 0; k < 560; k++) {
    c[k] = a[k] + -1.0;
  }
}

//
// File trailer for bsxfun.cpp
//
// [EOF]
//
