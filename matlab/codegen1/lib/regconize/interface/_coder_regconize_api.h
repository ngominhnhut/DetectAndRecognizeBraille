/*
 * File: _coder_regconize_api.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 15-Mar-2020 21:23:05
 */

#ifndef _CODER_REGCONIZE_API_H
#define _CODER_REGCONIZE_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_regconize_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T regconize(real_T X[560]);
extern void regconize_api(const mxArray * const prhs[1], const mxArray *plhs[1]);
extern void regconize_atexit(void);
extern void regconize_initialize(void);
extern void regconize_terminate(void);
extern void regconize_xil_terminate(void);

#endif

/*
 * File trailer for _coder_regconize_api.h
 *
 * [EOF]
 */
