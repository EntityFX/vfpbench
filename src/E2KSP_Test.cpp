// 2021 <helce> Svyatoslav Stupak
// vim:ts=4 sw=4 noet:

#include <minilib/CoreLib.h>
#include <minilib/SystemInfo.h>
#include "TimerClass.h"
#include "E2KSP_Test.h"
#include <limits>

//-----------------------------------------------------------------------------
#if FL_CPU_E2K
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace E2KSP
{
  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------

  typedef uint64_t CounterType;

  //=============================================================================
  // SSE
  //=============================================================================

  static void check_result(float ret, float ans)
  {
    FL_LOG("check: %f %f\n", ret, ans);
    FL_ASSERT(ret == ans);
  }

  //-----------------------------------------------------------------------------
  // Single Interleave 8
  //-----------------------------------------------------------------------------

#define SSE_S_IR8_8(op) \
  A = op(A, A);         \
  B = op(B, B);         \
  C = op(C, C);         \
  D = op(D, D);         \
  E = op(E, E);         \
  F = op(F, F);         \
  G = op(G, G);         \
  H = op(H, H);

#define SSE_S_IR8_0(op, op_str, name)                                \
  static uint64_t SSE_S_IR8_##name(CounterType LoopCount,            \
                                   float answer, float f0, float f1) \
  {                                                                  \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;            \
    float fp0 = f0;                                                  \
    float fp1 = f1;                                                  \
    TimerClass timer;                                                \
    timer.Begin();                                                   \
    __m128 A = _mm_load1_ps(&fp0);                                   \
    __m128 B = _mm_load1_ps(&fp0);                                   \
    __m128 C = _mm_load1_ps(&fp0);                                   \
    __m128 D = _mm_load1_ps(&fp0);                                   \
    __m128 E = _mm_load1_ps(&fp1);                                   \
    __m128 F = _mm_load1_ps(&fp1);                                   \
    __m128 G = _mm_load1_ps(&fp1);                                   \
    __m128 H = _mm_load1_ps(&fp1);                                   \
    for (CounterType loop = 0; loop < LoopCount; loop++)             \
    {                                                                \
      SSE_S_IR8_8(op)                                                \
      SSE_S_IR8_8(op)                                                \
      SSE_S_IR8_8(op)                                                \
      SSE_S_IR8_8(op)                                                \
      SSE_S_IR8_8(op)                                                \
    }                                                                \
    ret0 = _mm_cvtss_f32(A);                                         \
    ret1 = _mm_cvtss_f32(B);                                         \
    ret2 = _mm_cvtss_f32(C);                                         \
    ret3 = _mm_cvtss_f32(D);                                         \
    ret4 = _mm_cvtss_f32(E);                                         \
    ret5 = _mm_cvtss_f32(F);                                         \
    ret6 = _mm_cvtss_f32(G);                                         \
    ret7 = _mm_cvtss_f32(H);                                         \
    timer.End();                                                     \
    timer.Dump(op_str);                                              \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                              \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);          \
    check_result(ret0, answer);                                      \
    check_result(ret1, answer);                                      \
    check_result(ret2, answer);                                      \
    check_result(ret3, answer);                                      \
    check_result(ret4, answer);                                      \
    check_result(ret5, answer);                                      \
    check_result(ret6, answer);                                      \
    check_result(ret7, answer);                                      \
    return timer.Result();                                           \
  }

#define SSE_S_IR8(op, name) SSE_S_IR8_0(op, #op, name)

#define SSE_S_IRS4_8(op) \
  A = op(A, E);          \
  B = op(B, F);          \
  C = op(C, G);          \
  D = op(D, H);          \
  E = op(E, A);          \
  F = op(F, B);          \
  G = op(G, C);          \
  H = op(H, D);

#define SSE_S_IRS4_0(op, op_str, name)                                \
  static uint64_t SSE_S_IRS4_##name(CounterType LoopCount,            \
                                    float answer, float f0, float f1) \
  {                                                                   \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;             \
    float fp0 = f0;                                                   \
    float fp1 = f1;                                                   \
    TimerClass timer;                                                 \
    timer.Begin();                                                    \
    __m128 A = _mm_load1_ps(&fp0);                                    \
    __m128 B = _mm_load1_ps(&fp0);                                    \
    __m128 C = _mm_load1_ps(&fp0);                                    \
    __m128 D = _mm_load1_ps(&fp0);                                    \
    __m128 E = _mm_load1_ps(&fp1);                                    \
    __m128 F = _mm_load1_ps(&fp1);                                    \
    __m128 G = _mm_load1_ps(&fp1);                                    \
    __m128 H = _mm_load1_ps(&fp1);                                    \
    for (CounterType loop = 0; loop < LoopCount; loop++)              \
    {                                                                 \
      SSE_S_IRS4_8(op)                                                \
      SSE_S_IRS4_8(op)                                                \
      SSE_S_IRS4_8(op)                                                \
      SSE_S_IRS4_8(op)                                                \
      SSE_S_IRS4_8(op)                                                \
    }                                                                 \
    ret0 = _mm_cvtss_f32(A);                                          \
    ret1 = _mm_cvtss_f32(B);                                          \
    ret2 = _mm_cvtss_f32(C);                                          \
    ret3 = _mm_cvtss_f32(D);                                          \
    ret4 = _mm_cvtss_f32(E);                                          \
    ret5 = _mm_cvtss_f32(F);                                          \
    ret6 = _mm_cvtss_f32(G);                                          \
    ret7 = _mm_cvtss_f32(H);                                          \
    timer.End();                                                      \
    timer.Dump(op_str);                                               \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                               \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);           \
    check_result(ret0, answer);                                       \
    check_result(ret1, answer);                                       \
    check_result(ret2, answer);                                       \
    check_result(ret3, answer);                                       \
    check_result(ret4, answer);                                       \
    check_result(ret5, answer);                                       \
    check_result(ret6, answer);                                       \
    check_result(ret7, answer);                                       \
    return timer.Result();                                            \
  }

#define SSE_S_IRS4(op, name) SSE_S_IRS4_0(op, #op, name)

  //-----------------------------------------------------------------------------
  // Single Interleave 8 MAD
  //-----------------------------------------------------------------------------

#define SSE_M_IR8_8(op1, op2) \
  A = op1(A, A);              \
  B = op2(B, B);              \
  C = op1(C, C);              \
  D = op2(D, D);              \
  E = op1(E, E);              \
  F = op2(F, F);              \
  G = op1(G, G);              \
  H = op2(H, H);

#define SSE_M_IR8_0(op1, op2, op1_str, op2_str, name)       \
  static uint64_t SSE_M_IR8_##name(CounterType LoopCount,   \
                                   float answer)            \
  {                                                         \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;   \
    float fp0 = 1.0;                                        \
    float fp1 = 2.0;                                        \
    TimerClass timer;                                       \
    timer.Begin();                                          \
    __m128 A = _mm_load1_ps(&fp0);                          \
    __m128 B = _mm_load1_ps(&fp0);                          \
    __m128 C = _mm_load1_ps(&fp0);                          \
    __m128 D = _mm_load1_ps(&fp0);                          \
    __m128 E = _mm_load1_ps(&fp1);                          \
    __m128 F = _mm_load1_ps(&fp1);                          \
    __m128 G = _mm_load1_ps(&fp1);                          \
    __m128 H = _mm_load1_ps(&fp1);                          \
    for (CounterType loop = 0; loop < LoopCount; loop++)    \
    {                                                       \
      SSE_M_IR8_8(op1, op2)                                 \
      SSE_M_IR8_8(op1, op2)                                 \
      SSE_M_IR8_8(op1, op2)                                 \
      SSE_M_IR8_8(op1, op2)                                 \
      SSE_M_IR8_8(op1, op2)                                 \
    }                                                       \
    ret0 = _mm_cvtss_f32(A);                                \
    ret1 = _mm_cvtss_f32(B);                                \
    ret2 = _mm_cvtss_f32(C);                                \
    ret3 = _mm_cvtss_f32(D);                                \
    ret4 = _mm_cvtss_f32(E);                                \
    ret5 = _mm_cvtss_f32(F);                                \
    ret6 = _mm_cvtss_f32(G);                                \
    ret7 = _mm_cvtss_f32(H);                                \
    timer.End();                                            \
    timer.Dump(op1_str);                                    \
    timer.Dump(op2_str);                                    \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                     \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7); \
    check_result(ret0, answer);                             \
    check_result(ret1, answer);                             \
    check_result(ret2, answer);                             \
    check_result(ret3, answer);                             \
    check_result(ret4, answer);                             \
    check_result(ret5, answer);                             \
    check_result(ret6, answer);                             \
    check_result(ret7, answer);                             \
    return timer.Result();                                  \
  }

#define SSE_M_IR8(op1, op2, name) SSE_M_IR8_0(op1, op2, #op1, #op2, name)

  //-----------------------------------------------------------------------------
  // Single Interleave 8 MAD op3
  //-----------------------------------------------------------------------------

#define SSE_M_IR9_9(op1, op2, op3) \
  A = op1(A, A);                   \
  B = op2(B, B);                   \
  C = op3(C, C);                   \
  D = op1(D, D);                   \
  E = op2(E, E);                   \
  F = op3(F, F);                   \
  G = op1(G, G);                   \
  H = op2(H, H);                   \
  J = op3(J, J);

#define SSE_M_IR9_0(op1, op2, op3, op_str1, op_str2, op_str3, name) \
  static uint64_t SSE_M_IR9_##name(CounterType LoopCount,           \
                                   float answer)                    \
  {                                                                 \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;           \
    float fp0 = 2.0;                                                \
    float fp1 = 5.0;                                                \
    TimerClass timer;                                               \
    timer.Begin();                                                  \
    __m128 A = _mm_load1_ps(&fp0);                                  \
    __m128 B = _mm_load1_ps(&fp1);                                  \
    __m128 C = _mm_load1_ps(&fp0);                                  \
    __m128 D = _mm_load1_ps(&fp1);                                  \
    __m128 E = _mm_load1_ps(&fp0);                                  \
    __m128 F = _mm_load1_ps(&fp1);                                  \
    __m128 G = _mm_load1_ps(&fp0);                                  \
    __m128 H = _mm_load1_ps(&fp1);                                  \
    __m128 J = _mm_load1_ps(&fp0);                                  \
    for (CounterType loop = 0; loop < LoopCount; loop++)            \
    {                                                               \
      SSE_M_IR9_9(op1, op2, op3)                                    \
      SSE_M_IR9_9(op1, op2, op3)                                    \
      SSE_M_IR9_9(op1, op2, op3)                                    \
      SSE_M_IR9_9(op1, op2, op3)                                    \
      SSE_M_IR9_9(op1, op2, op3)                                    \
    }                                                               \
    ret0 = _mm_cvtss_f32(A);                                        \
    ret1 = _mm_cvtss_f32(B);                                        \
    ret2 = _mm_cvtss_f32(C);                                        \
    ret3 = _mm_cvtss_f32(D);                                        \
    ret4 = _mm_cvtss_f32(E);                                        \
    ret5 = _mm_cvtss_f32(F);                                        \
    ret6 = _mm_cvtss_f32(G);                                        \
    ret7 = _mm_cvtss_f32(H);                                        \
    timer.End();                                                    \
    timer.Dump(op_str1);                                            \
    timer.Dump(op_str2);                                            \
    timer.Dump(op_str3);                                            \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                             \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);         \
    check_result(ret0, answer);                                     \
    check_result(ret1, answer);                                     \
    check_result(ret2, answer);                                     \
    check_result(ret3, answer);                                     \
    check_result(ret4, answer);                                     \
    check_result(ret5, answer);                                     \
    check_result(ret6, answer);                                     \
    check_result(ret7, answer);                                     \
    return timer.Result();                                          \
  }

#define SSE_M_IR9(op1, op2, op3, name) SSE_M_IR9_0(op1, op2, op3, #op1, #op2, #op3, name)

  //-----------------------------------------------------------------------------
  // Single Interleave 8 FMA
  //-----------------------------------------------------------------------------

#define SSE_S_FMA_IR8_8(op) \
  A = op(A, A, A);          \
  B = op(B, B, B);          \
  C = op(C, C, C);          \
  D = op(D, D, D);          \
  E = op(E, E, E);          \
  F = op(F, F, F);          \
  G = op(G, G, G);          \
  H = op(H, H, H);

#define SSE_S_FMA_IR8_0(op, op_str, name)                                   \
  static uint64_t SSE_S_FMA_IR8_##name(CounterType LoopCount, float answer) \
  {                                                                         \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;                   \
    float fp0 = 2.0;                                                        \
    float fp1 = 5.0;                                                        \
    TimerClass timer;                                                       \
    timer.Begin();                                                          \
    __m128 A = _mm_load1_ps(&fp0);                                          \
    __m128 B = _mm_load1_ps(&fp1);                                          \
    __m128 C = _mm_load1_ps(&fp0);                                          \
    __m128 D = _mm_load1_ps(&fp1);                                          \
    __m128 E = _mm_load1_ps(&fp0);                                          \
    __m128 F = _mm_load1_ps(&fp1);                                          \
    __m128 G = _mm_load1_ps(&fp0);                                          \
    __m128 H = _mm_load1_ps(&fp1);                                          \
    for (CounterType loop = 0; loop < LoopCount; loop++)                    \
    {                                                                       \
      SSE_S_FMA_IR8_8(op)                                                   \
      SSE_S_FMA_IR8_8(op)                                                   \
      SSE_S_FMA_IR8_8(op)                                                   \
      SSE_S_FMA_IR8_8(op)                                                   \
      SSE_S_FMA_IR8_8(op)                                                   \
    }                                                                       \
    ret0 = _mm_cvtss_f32(A);                                                \
    ret1 = _mm_cvtss_f32(B);                                                \
    ret2 = _mm_cvtss_f32(C);                                                \
    ret3 = _mm_cvtss_f32(D);                                                \
    ret4 = _mm_cvtss_f32(E);                                                \
    ret5 = _mm_cvtss_f32(F);                                                \
    ret6 = _mm_cvtss_f32(G);                                                \
    ret7 = _mm_cvtss_f32(H);                                                \
    timer.End();                                                            \
    timer.Dump(op_str);                                                     \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                                     \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);                 \
    check_result(ret0, answer);                                             \
    check_result(ret1, answer);                                             \
    check_result(ret2, answer);                                             \
    check_result(ret3, answer);                                             \
    check_result(ret4, answer);                                             \
    check_result(ret5, answer);                                             \
    check_result(ret6, answer);                                             \
    check_result(ret7, answer);                                             \
    return timer.Result();                                                  \
  }
#define SSE_S_FMA_IR8(op, name) SSE_S_FMA_IR8_0(op, #op, name)

  //-----------------------------------------------------------------------------
  // Single Interleave Register 12 FMA
  //-----------------------------------------------------------------------------

#define SSE_S_FMA_IR12_12(op) \
  M0 = op(M0, M9, M8);        \
  M1 = op(M1, M9, M8);        \
  M2 = op(M2, M9, M8);        \
  M3 = op(M3, M9, M8);        \
  M4 = op(M4, M9, M8);        \
  M5 = op(M5, M9, M8);        \
  M6 = op(M6, M9, M8);        \
  M7 = op(M7, M9, M8);        \
  M10 = op(M10, M9, M8);      \
  M11 = op(M11, M9, M8);      \
  M12 = op(M12, M9, M8);      \
  M13 = op(M13, M9, M8);

#define SSE_S_FMA_IR12_0(op, op_str, name)                                   \
  static uint64_t SSE_S_FMA_IR12_##name(CounterType LoopCount, float answer) \
  {                                                                          \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;                    \
    float fpz = 0.0;                                                         \
    float fp0 = 1.0;                                                         \
    float fp1 = 2.0;                                                         \
    TimerClass timer;                                                        \
    timer.Begin();                                                           \
    __m128 M0 = _mm_load1_ps(&fpz);                                          \
    __m128 M1 = _mm_load1_ps(&fpz);                                          \
    __m128 M2 = _mm_load1_ps(&fp0);                                          \
    __m128 M3 = _mm_load1_ps(&fp1);                                          \
    __m128 M4 = _mm_load1_ps(&fp0);                                          \
    __m128 M5 = _mm_load1_ps(&fp1);                                          \
    __m128 M6 = _mm_load1_ps(&fp0);                                          \
    __m128 M7 = _mm_load1_ps(&fp1);                                          \
    __m128 M8 = _mm_load1_ps(&fp0);                                          \
    __m128 M9 = _mm_load1_ps(&fp1);                                          \
    __m128 M10 = _mm_load1_ps(&fpz);                                         \
    __m128 M11 = _mm_load1_ps(&fpz);                                         \
    __m128 M12 = _mm_load1_ps(&fpz);                                         \
    __m128 M13 = _mm_load1_ps(&fpz);                                         \
    for (CounterType loop = 0; loop < LoopCount; loop++)                     \
    {                                                                        \
      SSE_S_FMA_IR12_12(op)                                                  \
      SSE_S_FMA_IR12_12(op)                                                  \
      SSE_S_FMA_IR12_12(op)                                                  \
      SSE_S_FMA_IR12_12(op)                                                  \
      SSE_S_FMA_IR12_12(op)                                                  \
    }                                                                        \
    ret0 = _mm_cvtss_f32(M0);                                                \
    ret1 = _mm_cvtss_f32(M1);                                                \
    ret2 = _mm_cvtss_f32(M2);                                                \
    ret3 = _mm_cvtss_f32(M3);                                                \
    ret4 = _mm_cvtss_f32(M4);                                                \
    ret5 = _mm_cvtss_f32(M5);                                                \
    ret6 = _mm_cvtss_f32(M6);                                                \
    ret7 = _mm_cvtss_f32(M7);                                                \
    timer.End();                                                             \
    timer.Dump(op_str);                                                      \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                                      \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);                  \
    check_result(ret0, answer);                                              \
    check_result(ret1, answer);                                              \
    check_result(ret2, answer);                                              \
    check_result(ret3, answer);                                              \
    check_result(ret4, answer);                                              \
    check_result(ret5, answer);                                              \
    check_result(ret6, answer);                                              \
    check_result(ret7, answer);                                              \
    return timer.Result();                                                   \
  }

#define SSE_S_FMA_IR12(op, name) SSE_S_FMA_IR12_0(op, #op, name)

  //-----------------------------------------------------------------------------
  // Single Interleave Register 12 FMA+SSE
  //-----------------------------------------------------------------------------

#define SSE_M_FMA_IR12_12(op1, op2) \
  M0 = op1(M0, M9, M8);             \
  M1 = op2(M9, M1);                 \
  M2 = op1(M2, M9, M8);             \
  M3 = op2(M9, M3);                 \
  M4 = op1(M4, M9, M8);             \
  M5 = op2(M9, M5);                 \
  M6 = op1(M6, M9, M8);             \
  M7 = op2(M9, M7);                 \
  M10 = op1(M10, M9, M8);            \
  M11 = op2(M9, M11);                \
  M12 = op1(M12, M9, M8);            \
  M13 = op2(M9, M13);

#define SSE_M_FMA_IR12_0(op1, op2, op_str1, op_str2, name)                   \
  static uint64_t SSE_M_FMA_IR12_##name(CounterType LoopCount, float answer) \
  {                                                                          \
    float ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;                    \
    float fp0 = 1.0;                                                         \
    float fp1 = 2.0;                                                         \
    TimerClass timer;                                                        \
    timer.Begin();                                                           \
    __m128 M0 = _mm_load1_ps(&fp0);                                          \
    __m128 M1 = _mm_load1_ps(&fp1);                                          \
    __m128 M2 = _mm_load1_ps(&fp0);                                          \
    __m128 M3 = _mm_load1_ps(&fp1);                                          \
    __m128 M4 = _mm_load1_ps(&fp0);                                          \
    __m128 M5 = _mm_load1_ps(&fp1);                                          \
    __m128 M6 = _mm_load1_ps(&fp0);                                          \
    __m128 M7 = _mm_load1_ps(&fp1);                                          \
    __m128 M8 = _mm_load1_ps(&fp0);                                          \
    __m128 M9 = _mm_load1_ps(&fp1);                                          \
    __m128 M10 = _mm_load1_ps(&fp0);                                         \
    __m128 M11 = _mm_load1_ps(&fp1);                                         \
    __m128 M12 = _mm_load1_ps(&fp0);                                         \
    __m128 M13 = _mm_load1_ps(&fp1);                                         \
    for (CounterType loop = 0; loop < LoopCount; loop++)                     \
    {                                                                        \
      SSE_M_FMA_IR12_12(op1, op2)                                            \
      SSE_M_FMA_IR12_12(op1, op2)                                            \
      SSE_M_FMA_IR12_12(op1, op2)                                            \
      SSE_M_FMA_IR12_12(op1, op2)                                            \
      SSE_M_FMA_IR12_12(op1, op2)                                            \
    }                                                                        \
    ret0 = _mm_cvtss_f32(M0);                                                \
    ret1 = _mm_cvtss_f32(M1);                                                \
    ret2 = _mm_cvtss_f32(M2);                                                \
    ret3 = _mm_cvtss_f32(M3);                                                \
    ret4 = _mm_cvtss_f32(M4);                                                \
    ret5 = _mm_cvtss_f32(M5);                                                \
    ret6 = _mm_cvtss_f32(M6);                                                \
    ret7 = _mm_cvtss_f32(M7);                                                \
    timer.End();                                                             \
    timer.Dump(op_str1);                                                     \
    timer.Dump(op_str2);                                                     \
    FL_LOG("%f %f %f %f %f %f %f %f\n",                                      \
           ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7);                  \
    check_result(ret0, answer);                                              \
    check_result(ret1, answer);                                              \
    check_result(ret2, answer);                                              \
    check_result(ret3, answer);                                              \
    check_result(ret4, answer);                                              \
    check_result(ret5, answer);                                              \
    check_result(ret6, answer);                                              \
    check_result(ret7, answer);                                              \
    return timer.Result();                                                   \
  }
#define SSE_M_FMA_IR12(op1, op2, name) SSE_M_FMA_IR12_0(op1, op2, #op1, #op2, name)

  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------

  static const char *Instruction_Title[] = {
      "fmuls (mulss) (32bit x1) n8",
      "fadds (addss) (32bit x1) n8",
      "vfmaddss (32bit x1) n8",
      "vfmaddss (32bit x1) n12",
      "vfma+mlss (32bit x1) n12",
      "vfma+adss (32bit x1) n12",

      "fmulps (mulps) (32bit x4) n8",
      "faddps (addps) (32bit x4) n8",
      "mul+addps (32bit x4) n8",
      "vfmaddps (32bit x4) n8",
      "vfmaddps (32bit x4) n12",
      "vfma+mlps (32bit x4) n12",
      "vfma+adps (32bit x4) n12",

      "ml+ad+addps (32bit x4) n9",

      "fmulss (mulss) (32bit x1) ns4",
      "faddss (addss) (32bit x1) ns4",

      "fmulps (mulps) (32bit x4) ns4",
      "faddps (addps) (32bit x4) ns4",
  };

  //-----------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------

  FloatTest::FloatTest()
  {
    FL_ASSERT(RESULT_MAX <= RESULT_BUFFER_MAX);
    LoopCount = DEFAULT_LOOP;
    ClearResult();

    SetOp(RESULT_SSE_MULSS_IR8, PER_LOOP_INST * 1, 1);
    SetOp(RESULT_SSE_ADDSS_IR8, PER_LOOP_INST * 1, 1);
    SetOp(RESULT_SSE_FMADDSS_IR8, PER_LOOP_INST * 2, 2);
    SetOp(RESULT_SSE_FMADDSS_IR12, PER_LOOP_INST_12 * 2, 2);
    SetOp2f(RESULT_SSE_FMA_MULSS_IR12, PER_LOOP_INST_12, (2 + 1) / 2.0f);
    SetOp2f(RESULT_SSE_FMA_ADDSS_IR12, PER_LOOP_INST_12, (2 + 1) / 2.0f);

    SetOp(RESULT_SSE_MULPS_IR8, PER_LOOP_INST * 4, 4);
    SetOp(RESULT_SSE_ADDPS_IR8, PER_LOOP_INST * 4, 4);
    SetOp(RESULT_SSE_MULPS_ADDPS_IR8, PER_LOOP_INST * 4, 4);
    SetOp(RESULT_SSE_FMADDPS_IR8, PER_LOOP_INST * 8, 8);
    SetOp(RESULT_SSE_FMADDPS_IR12, PER_LOOP_INST_12 * 8, 8);
    SetOp2f(RESULT_SSE_FMA_MULPS_IR12, PER_LOOP_INST_12, (8 + 4) / 2.0f);
    SetOp2f(RESULT_SSE_FMA_ADDPS_IR12, PER_LOOP_INST_12, (8 + 4) / 2.0f);

    SetOp(RESULT_SSE_ML_AD_ADDPS_IR9, PER_LOOP_INST_9 * 4, 4);

    SetOp(RESULT_SSE_MULSS_IRS4, PER_LOOP_INST * 1, 1);
    SetOp(RESULT_SSE_ADDSS_IRS4, PER_LOOP_INST * 1, 1);

    SetOp(RESULT_SSE_MULPS_IRS4, PER_LOOP_INST * 4, 4);
    SetOp(RESULT_SSE_ADDPS_IRS4, PER_LOOP_INST * 4, 4);
  }

  //-----------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------

  SSE_S_IR8(_mm_mul_ss, mulss_ir8);
  SSE_S_IR8(_mm_add_ss, addss_ir8);
  SSE_S_FMA_IR8(_mm_fmadd_ss, fmaddss_ir8);   // FMA3
  SSE_S_FMA_IR12(_mm_fmadd_ss, fmaddss_ir12); // FMA3

  SSE_M_FMA_IR12(_mm_fmadd_ss, _mm_mul_ss, fmaddss_mulss_ir12); // FMA3
  SSE_M_FMA_IR12(_mm_fmadd_ss, _mm_add_ss, fmaddss_addss_ir12); // FMA3

  SSE_S_IR8(_mm_mul_ps, mulps_ir8);
  SSE_S_IR8(_mm_add_ps, addps_ir8);
  SSE_M_IR8(_mm_mul_ps, _mm_add_ps, mulps_addps_ir8);

  SSE_S_FMA_IR8(_mm_fmadd_ps, fmaddps_ir8);   // FMA3
  SSE_S_FMA_IR12(_mm_fmadd_ps, fmaddps_ir12); // FMA3

  SSE_M_FMA_IR12(_mm_fmadd_ps, _mm_mul_ps, fmaddps_mulps_ir12); // FMA3
  SSE_M_FMA_IR12(_mm_fmadd_ps, _mm_add_ps, fmaddps_addps_ir12); // FMA3

  SSE_M_IR9(_mm_mul_ps, _mm_add_ps, _mm_add_ps, ml_ad_addps_ir9);

  SSE_S_IRS4(_mm_mul_ss, mulss_irs4);
  SSE_S_IRS4(_mm_add_ss, addss_irs4);

  SSE_S_IRS4(_mm_mul_ps, mulps_irs4);
  SSE_S_IRS4(_mm_add_ps, addps_irs4);

  void FloatTest::Run()
  {
    ClearResult();
    unsigned int Loop = LoopCount;

    FL_LOG("SSE loop=%d\n", Loop);

    float sum = 0;
    float sum_2 = 0;
    for (unsigned int i = 0; i < Loop * 5; i++)
    {
      sum += 2.0f;
      sum_2 += 10.0f;
    }

    //------------------------------------------------------
    // IR8
    //------------------------------------------------------

    SetResult(RESULT_SSE_MULSS_IR8, SSE_S_IR8_mulss_ir8(Loop, 10.0f, 0.9999f, 0.9998f));
    Progress++;

    SetResult(RESULT_SSE_ADDSS_IR8, SSE_S_IR8_addss_ir8(Loop, 10.0f, 1e-35f, 1e-35f));
    Progress++;

    SetResult(RESULT_SSE_FMADDSS_IR8, SSE_S_FMA_IR8_fmaddss_ir8(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMADDSS_IR12, SSE_S_FMA_IR12_fmaddss_ir12(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMA_MULSS_IR12, SSE_M_FMA_IR12_fmaddss_mulss_ir12(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMA_ADDSS_IR12, SSE_M_FMA_IR12_fmaddss_addss_ir12(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_MULPS_IR8, SSE_S_IR8_mulps_ir8(Loop, 10.0f, 0.9999f, 0.9998f));
    Progress++;

    SetResult(RESULT_SSE_ADDPS_IR8, SSE_S_IR8_addps_ir8(Loop, 10.0f, 1e-35f, 1e-35f));
    Progress++;

    SetResult(RESULT_SSE_MULPS_ADDPS_IR8, SSE_M_IR8_mulps_addps_ir8(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMADDPS_IR8, SSE_S_FMA_IR8_fmaddps_ir8(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMADDPS_IR12, SSE_S_FMA_IR12_fmaddps_ir12(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMA_MULPS_IR12, SSE_M_FMA_IR12_fmaddps_mulps_ir12(Loop, 10.0f));
    Progress++;

    SetResult(RESULT_SSE_FMA_ADDPS_IR12, SSE_M_FMA_IR12_fmaddps_addps_ir12(Loop, 10.0f));
    Progress++;

    //------------------------------------------------------
    //------------------------------------------------------

    //------------------------------------------------------
    // IR9
    //------------------------------------------------------

    SetResult(RESULT_SSE_ML_AD_ADDPS_IR9, SSE_M_IR9_ml_ad_addps_ir9(Loop, 10.0f));
    Progress++;

    //------------------------------------------------------
    // IRS4
    //------------------------------------------------------
    SetResult(RESULT_SSE_MULSS_IRS4, SSE_S_IRS4_mulss_irs4(Loop, 0.0f, 0.9999f, 0.9998f));
    Progress++;

    SetResult(RESULT_SSE_ADDSS_IRS4, SSE_S_IRS4_addss_irs4(Loop, std::numeric_limits<float>::infinity(), 1e-35f, 1e-35f));
    Progress++;

    SetResult(RESULT_SSE_MULPS_IRS4, SSE_S_IRS4_mulps_irs4(Loop, 0.0f, 0.9999f, 0.9998f));
    Progress++;

    SetResult(RESULT_SSE_ADDPS_IRS4, SSE_S_IRS4_addps_irs4(Loop, std::numeric_limits<float>::infinity(), 1e-35f, 1e-35f));
    Progress++;

    DoneFlag = true;
  }

  const char *FloatTest::GetInstructionName(unsigned int result_index) const
  {
    FL_ASSERT(result_index < GetResultCount());
    //FL_ASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
    return Instruction_Title[result_index];
  }

  const char *FloatTest::GetTestName() const
  {
    if (!IsMultithread())
    {
      return "SSE (SP fp)";
    }
    else
    {
      return "SSE (SP fp) multi-thread";
    }
  }

  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // e2k
//-----------------------------------------------------------------------------
