// 2021 <helce> Svyatoslav Stupak
// vim:ts=4 sw=4 noet:

#ifndef	E2KSP_TEST_H_
#define	E2KSP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"
#include  <xmmintrin.h>

namespace E2KSP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
    enum {
	DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
	PER_LOOP_INST	=	5 * 8,		// 40
	PER_LOOP_INST_9	=	5 * 9,		// 45
    };
    enum {
	RESULT_SSE_MULSS_IR8,
	RESULT_SSE_ADDSS_IR8,
	
	RESULT_SSE_MULPS_IR8,
	RESULT_SSE_ADDPS_IR8,
	RESULT_SSE_MULPS_ADDPS_IR8,
	
	RESULT_SSE_ML_AD_ADDPS_IR9,
	
	RESULT_SSE_MULSS_IRS4,
	RESULT_SSE_ADDSS_IRS4,

	RESULT_SSE_MULPS_IRS4,
	RESULT_SSE_ADDPS_IRS4,
	
	RESULT_MAX,
    };
public:

    FloatTest();

    void	Run() override;
    unsigned int	GetResultInfo( InfoType index ) const override
    {
	switch( index ){
	case InfoType::INFO_COUNT:
	    return	RESULT_MAX;
	case InfoType::INFO_PROGRESS:
	    return	RESULT_MAX;
	case InfoType::INFO_LOOP:
	    return	LoopCount;
	}
	FL_ASSERT( 0 );
	return	0;
    }

    const char*		GetTestName() const override;
    const char*		GetInstructionName( unsigned int result_index ) const override;
    unsigned int	GetLoopType() const override
    {
	return	LOOPTYPE_SP;
    }
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif
