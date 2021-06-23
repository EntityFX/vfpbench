// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"E2KSP_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_E2K
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace E2KSP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef	uint32_t	CounterType;

FloatTest::FloatTest()
{
    FL_ASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
    LoopCount= DEFAULT_LOOP;
    ClearResult();
    FL_PRINT("ctr");
}

void FloatTest::Run()
{
    FL_PRINT("0\n");
    ClearResult();
    unsigned int	Loop= LoopCount;

    FL_LOG( "E2K loop=%d\n", Loop );

    float	sum= 0;
    for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
	sum+= 2.0f;
    }
    
    FL_PRINT("1");
    
    Progress++;


    //------------------------------------------------------
    //------------------------------------------------------

    DoneFlag= true;
}

static const char*	Instruction_Title[]= {

    "FPU muld (32bit x1) n8",
    "FPU addd (32bit x1) n8",

    "FPU muld (32bit x1) n1",
    "FPU addd (32bit x1) n1",
};


const char*	FloatTest::GetInstructionName( unsigned int result_index ) const
{
    FL_ASSERT( result_index < GetResultCount() );
    FL_ASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
    return	Instruction_Title[result_index];
}

const char*	FloatTest::GetTestName() const
{
    if( !IsMultithread() ){
	return	"FPU (SP fp)";
    }else{
	return	"FPU (SP fp) multi-thread";
    }
}

}

#endif