/*
 * File: _coder_Recog_Braille_api.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 14-Apr-2020 07:25:02
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_Recog_Braille_api.h"
#include "_coder_Recog_Braille_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131466U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "Recog_Braille",                     /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[560];
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[560];
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *X, const
  char_T *identifier))[560];
static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[560]
 */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[560]
{
  real_T (*y)[560];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[560]
 */
  static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[560]
{
  real_T (*ret)[560];
  static const int32_T dims[1] = { 560 };

  emlrtCheckBuiltInR2012b(sp, (const emlrtMsgIdentifier *)msgId, src, "double",
    false, 1U, *(int32_T (*)[1])&dims[0]);
  ret = (real_T (*)[560])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *X
 *                const char_T *identifier
 * Return Type  : real_T (*)[560]
 */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *X, const
  char_T *identifier))[560]
{
  real_T (*y)[560];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(X), &thisId);
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
 *                int32_T nlhs
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void Recog_Braille_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[1])
{
  real_T (*X)[560];
  real_T Recog;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  (void)nlhs;
  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  X = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "X");

  /* Invoke the target function */
  Recog = Recog_Braille(*X);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(Recog);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Recog_Braille_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  Recog_Braille_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Recog_Braille_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Recog_Braille_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_Recog_Braille_api.c
 *
 * [EOF]
 */
