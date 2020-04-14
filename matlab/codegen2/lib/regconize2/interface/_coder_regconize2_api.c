/*
 * File: _coder_regconize2_api.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 20-Mar-2020 16:56:54
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_regconize2_api.h"
#include "_coder_regconize2_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131450U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "regconize2",                        /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId))[560];
static real_T (*c_emlrt_marshallIn(const mxArray *src, const emlrtMsgIdentifier *
  msgId))[560];
static real_T (*emlrt_marshallIn(const mxArray *X, const char_T *identifier))
  [560];
static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */

/*
 * Arguments    : const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[560]
 */
static real_T (*b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId))[560]
{
  real_T (*y)[560];
  y = c_emlrt_marshallIn(emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
/*
 * Arguments    : const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[560]
 */
  static real_T (*c_emlrt_marshallIn(const mxArray *src, const
  emlrtMsgIdentifier *msgId))[560]
{
  real_T (*ret)[560];
  static const int32_T dims[1] = { 560 };

  emlrtCheckBuiltInR2012b(emlrtRootTLSGlobal, msgId, src, "double", false, 1U,
    dims);
  ret = (real_T (*)[560])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray *X
 *                const char_T *identifier
 * Return Type  : real_T (*)[560]
 */
static real_T (*emlrt_marshallIn(const mxArray *X, const char_T *identifier))
  [560]
{
  real_T (*y)[560];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(emlrtAlias(X), &thisId);
  emlrtDestroyArray(&X);
  return y;
}
/*
 * Arguments    : const real_T u
 * Return Type  : const mxArray *
 */
  static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *y;
  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const mxArray * const prhs[1]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void regconize2_api(const mxArray * const prhs[1], const mxArray *plhs[1])
{
  real_T (*X)[560];
  real_T Recog;

  /* Marshall function inputs */
  X = emlrt_marshallIn(emlrtAlias((const mxArray *)prhs[0]), "X");

  /* Invoke the target function */
  Recog = regconize2(*X);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(Recog);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void regconize2_atexit(void)
{
  mexFunctionCreateRootTLS();
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  regconize2_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void regconize2_initialize(void)
{
  mexFunctionCreateRootTLS();
  emlrtClearAllocCountR2012b(emlrtRootTLSGlobal, false, 0U, 0);
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void regconize2_terminate(void)
{
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_regconize2_api.c
 *
 * [EOF]
 */
