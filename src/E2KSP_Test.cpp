// 2021 <helce> Svyatoslav Stupak
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"E2KSP_Test.h"
#include  <limits>


//-----------------------------------------------------------------------------
#if FL_CPU_E2K
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace E2KSP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef	uint64_t CounterType;


//=============================================================================
// SSE
//=============================================================================

static void check_result( float ret, float ans )
{
    FL_LOG( "check: %f %f\n", ret, ans );
    FL_ASSERT( ret == ans );
}

//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define SSE_S_IR8_8(op) \
  A = op(A,A);          \
  B = op(B,B);	        \
  C = op(C,C);	        \
  D = op(D,D);	        \
  E = op(E,E);	        \
  F = op(F,F);	        \
  G = op(G,G);	        \
  H = op(H,H);

#define SSE_S_IR8_0(op, op_str, name) \
static uint64_t SSE_S_IR8_##name( CounterType LoopCount,\
                     float answer, float f0, float f1 ) \
{                                                       \
  float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
  float fp0 = f0;                                       \
  float fp1 = f1;                                      \
    TimerClass	timer;			                              \
    timer.Begin();				                                \
  __m128 A = _mm_load1_ps (&fp0);                        \
  __m128 B = _mm_load1_ps (&fp0);                        \
  __m128 C = _mm_load1_ps (&fp0);                        \
  __m128 D = _mm_load1_ps (&fp0);                        \
  __m128 E = _mm_load1_ps (&fp1);                        \
  __m128 F = _mm_load1_ps (&fp1);                        \
  __m128 G = _mm_load1_ps (&fp1);                        \
  __m128 H = _mm_load1_ps (&fp1);                        \
  for (CounterType loop = 0; loop < LoopCount; loop++) {\
	SSE_S_IR8_8( op )	                                  \
	SSE_S_IR8_8( op )	                                  \
	SSE_S_IR8_8( op )	                                  \
	SSE_S_IR8_8( op )	                                  \
	SSE_S_IR8_8( op )	                                  \
  }                                                     \
  ret0 = _mm_cvtss_f32(A);                              \
  ret1 = _mm_cvtss_f32(B);                              \
  ret2 = _mm_cvtss_f32(C);                              \
  ret3 = _mm_cvtss_f32(D);                              \
  ret4 = _mm_cvtss_f32(E);                              \
  ret5 = _mm_cvtss_f32(F);                              \
  ret6 = _mm_cvtss_f32(G);                              \
  ret7 = _mm_cvtss_f32(H);                              \
    timer.End();		                                      \
    timer.Dump( op_str );                                 \
    FL_LOG( "%f %f %f %f %f %f %f %f\n",                  \
       ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7 );\
    check_result( ret0, answer );	                        \
    check_result( ret1, answer );	                        \
    check_result( ret2, answer );	                        \
    check_result( ret3, answer );	                        \
    check_result( ret4, answer );	                        \
    check_result( ret5, answer );	                        \
    check_result( ret6, answer );	                        \
    check_result( ret7, answer );	                        \
    return	timer.Result();	                              \
}

#define SSE_S_IR8(op,name) SSE_S_IR8_0(op, #op, name)

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static const char*	Instruction_Title[]= {
    "fmuls (mulss) (32bit x1) n8",
    "fadds (addss) (32bit x1) n8",
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

FloatTest::FloatTest()
{
    FL_ASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
    LoopCount= DEFAULT_LOOP;
    ClearResult();

    SetOp( RESULT_SSE_MULSS_IR8,		PER_LOOP_INST * 1, 1	);
    SetOp( RESULT_SSE_ADDSS_IR8,		PER_LOOP_INST * 1, 1	);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

SSE_S_IR8( _mm_mul_ss,  mulss_ir8 );
SSE_S_IR8( _mm_add_ss,  addss_ir8 );

void FloatTest::Run()
{
    ClearResult();
    unsigned int	Loop= LoopCount;

  FL_LOG( "SSE loop=%d\n", Loop );

  //------------------------------------------------------
  // IR8
  //------------------------------------------------------

    SetResult( RESULT_SSE_MULSS_IR8,	SSE_S_IR8_mulss_ir8( Loop, 0.0f,	0.9999f, 0.9998f	) );
    Progress++;

    SetResult( RESULT_SSE_ADDSS_IR8,	SSE_S_IR8_addss_ir8( Loop,  std::numeric_limits<float>::infinity(),  1e-35f, 1e-35f		) );
    Progress++;

  //------------------------------------------------------
  //------------------------------------------------------

    DoneFlag= true;
}


const char*	FloatTest::GetInstructionName( unsigned int result_index ) const
{
    FL_ASSERT( result_index < GetResultCount() );
    //FL_ASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
    return	Instruction_Title[result_index];
}

const char*	FloatTest::GetTestName() const
{
    if( !IsMultithread() ){
	return	"SSE (SP fp)";
    }else{
	return	"SSE (SP fp) multi-thread";
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // e2k
//-----------------------------------------------------------------------------
