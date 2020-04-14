/*
 * File: _coder_regconize2_api.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 20-Mar-2020 16:56:54
 */

#ifndef _CODER_REGCONIZE2_API_H
#define _CODER_REGCONIZE2_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_regconize2_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T regconize2(real_T X[560]);
extern void regconize2_api(const mxArray * const prhs[1], const mxArray *plhs[1]);
extern void regconize2_atexit(void);
extern void regconize2_initialize(void);
extern void regconize2_terminate(void);
extern void regconize2_xil_terminate(void);

#endif

/*
 * File trailer for _coder_regconize2_api.h
 *
 * [EOF]
 */
