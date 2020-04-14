/*
 * File: _coder_Recog_Braille_api.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 14-Apr-2020 07:25:02
 */

#ifndef _CODER_RECOG_BRAILLE_API_H
#define _CODER_RECOG_BRAILLE_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_Recog_Braille_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T Recog_Braille(real_T X[560]);
extern void Recog_Braille_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[1]);
extern void Recog_Braille_atexit(void);
extern void Recog_Braille_initialize(void);
extern void Recog_Braille_terminate(void);
extern void Recog_Braille_xil_terminate(void);

#endif

/*
 * File trailer for _coder_Recog_Braille_api.h
 *
 * [EOF]
 */
