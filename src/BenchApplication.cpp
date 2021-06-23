// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	<minilib/DateTime.h>
#include	"BenchApplication.h"
#include	"TestBase.h"
#include	<stdio.h>
#include	<stdlib.h>

#define	USE_MAX_EXPORT	0

using namespace flatlib;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	BenchApplication::save_size( ut::BinaryBuffer& buffer, const void* ptr, unsigned int size )
{
	void*	str= buffer.Alloc( size );
	memcpy( str, ptr, size );
}


void	BenchApplication::save_line( ut::BinaryBuffer& buffer, const char* ptr )
{
	unsigned int	size= static_cast<unsigned int>(strlen( ptr ));
	void*	str= buffer.Alloc( size );
	memcpy( str, ptr, size );
}


void	BenchApplication::save_format( ut::BinaryBuffer& buffer, const char* format, va_list arg )
{
	const int	MAX_BUFFER_PATH= 1024 * 4;
	char	str_buffer[MAX_BUFFER_PATH];
	vsprintf_s( str_buffer, MAX_BUFFER_PATH-1, format, arg );
	save_line( buffer, str_buffer );
}


void	BenchApplication::print( ut::BinaryBuffer& buffer, const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	save_format( buffer, format, ap );
	va_end( ap );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct ArraySize {
	template<size_t N>
	constexpr static size_t	GetArraySize( const char (&data)[N] )
	{
		return	N;
	}
};

const char*	SkipSpace( const char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

const char*	GetWord( char* buffer, size_t buffer_size, const char* ptr )
{
	char*	str= buffer;
	char*	end_str= buffer + buffer_size -1;
	ptr= SkipSpace( ptr );
	if( *ptr == '"' ){
		for( ptr++ ; *ptr && *ptr != '"' && str < end_str ;){
			*str++= *ptr++;
		}
		if( *ptr == '"' ){
			ptr++;
		}
	}else{
		for(; *ptr && *ptr > ' ' && str < end_str ;){
			*str++= *ptr++;
		}
	}
	*str++= '\0';
	return	ptr;
}

static void	format_GFLOPS( ut::BinaryBuffer&buffer, const char* text, double flops )
{
	if( flops != 0.0 ){
		BenchApplication::print( buffer, "%s: %8.3f GFLOPS\n", text, flops / 1000.0 );
	}else{
		BenchApplication::print( buffer, "%s: -\n", text );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


void BenchApplication::ExportCPUInfo( ut::BinaryBuffer& buffer ) const
{
	auto	arch= Info.GetArch();

	print( buffer, "ARCH: %s\n", Info.GetArchNameLong() );
	print( buffer, "FPU :" );


	switch( arch ){
	case CPUArch::CPU_ARM6:
	case CPUArch::CPU_ARM7:
		if( Info.HasInstructionSet( CPUFeature::ARM_VFPV4 ) ){
			print( buffer, " VFPv4" );
		}else{
			print( buffer, " VFPv3" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_NEON ) ){
			print( buffer, "-D32 NEON" );
		}else{
			print( buffer, "-D16" );
		}
		print( buffer, "\n" );
		break;
	case CPUArch::CPU_ARM64:
		print( buffer, " ASIMD(AArch64 NEON)" );
		if( Info.HasInstructionSet( CPUFeature::ARM_FPHP ) ){
			print( buffer, " FPHP" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SIMDHP ) ){
			print( buffer, " ASIMDHP" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SIMDDP ) ){
			print( buffer, " DOTPROD" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SVE ) ){
			print( buffer, " SVE" );
		}
		print( buffer, "\n" );
		break;
	case CPUArch::CPU_X86:
	case CPUArch::CPU_X64:
		if( Info.HasInstructionSet( CPUFeature::IA_SSE ) ){
			print( buffer, " SSE" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE2 ) ){
			print( buffer, " SSE2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSSE3 ) ){
			print( buffer, " SSSE3" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE41 ) ){
			print( buffer, " SSE4.1" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE42 ) ){
			print( buffer, " SSE4.2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){
			print( buffer, " AVX" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX2 ) ){
			print( buffer, " AVX2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){
			print( buffer, " FMA3" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_FMA4 ) ){
			print( buffer, " FMA4" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_F16C ) ){
			print( buffer, " F16C" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ){
			print( buffer, " AVX512F" );
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512BW ) ){
				print( buffer, "/BW" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512DQ ) ){
				print( buffer, "/DQ" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512VL ) ){
				print( buffer, "/VL" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512VNNI ) ){
				print( buffer, "/VNNI" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512BF16 ) ){
				print( buffer, "/BF16" );
			}
		}
		print( buffer, "\n" );
		break;
	case CPUArch::CPU_MIPS32:
	case CPUArch::CPU_MIPS64:
		if( Info.HasInstructionSet( CPUFeature::MIPS_PS ) ){
			print( buffer, " PS" );
		}
		if( Info.HasInstructionSet( CPUFeature::MIPS_F64 ) ){
			print( buffer, " F64" );
		}
		if( Info.HasInstructionSet( CPUFeature::MIPS_MSA ) ){
			print( buffer, " MSA" );
		}
		print( buffer, "\n" );
		break;
	case CPUArch::CPU_E2K:
		print( buffer, " E2K FPU");
		print( buffer, "\n" );
		break;
	default:
		print( buffer, "\n" );
		break;
	}

	print( buffer, "Name: %s\n", Info.GetDeviceName() );
	print( buffer, "CPU Thread: %2d\n", Info.GetTotalThreadCount() );
	print( buffer, "CPU Core  : %2d\n", Info.GetPhysicalCoreCount() );
	print( buffer, "CPU Group : %2d\n", Info.GetCoreGroupCount() );
	{
		unsigned int 	group_count= Info.GetCoreGroupCount();
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			print( buffer, "  Group %d: Thread=%2d  Clock=%f GHz  (mask:%llx)\n", gi, Info.GetThreadCount( gi ), Info.GetCoreClock( gi )/1000000.0, Info.GetAffinityMask( gi ) );
		}
	}

	switch( arch ){
	case CPUArch::CPU_ARM5:
	case CPUArch::CPU_ARM6:
	case CPUArch::CPU_ARM7:
	case CPUArch::CPU_ARM64:
		print( buffer, "NEON   : %s\n", Info.HasInstructionSet( CPUFeature::ARM_NEON ) ? "yes" : "no" );
		print( buffer, "FMA    : %s\n", Info.HasInstructionSet( CPUFeature::ARM_VFPV4 ) ? "yes" : "no" );
		print( buffer, "FPHP   : %s\n", Info.HasInstructionSet( CPUFeature::ARM_FPHP ) ? "yes" : "no" );
		print( buffer, "SIMDHP : %s\n", Info.HasInstructionSet( CPUFeature::ARM_SIMDHP ) ? "yes" : "no" );
		print( buffer, "DotProd: %s\n", Info.HasInstructionSet( CPUFeature::ARM_SIMDDP ) ? "yes" : "no" );
		break;
	case CPUArch::CPU_X86:
	case CPUArch::CPU_X64:
		print( buffer, "SSE   : %s\n", Info.HasInstructionSet( CPUFeature::IA_SSE2 ) ? "yes" : "no" );
		print( buffer, "AVX   : %s\n", Info.HasInstructionSet( CPUFeature::IA_AVX ) ? "yes" : "no" );
		print( buffer, "FMA   : %s\n", Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ? "yes" : "no" );
		print( buffer, "F16C  : %s\n", Info.HasInstructionSet( CPUFeature::IA_F16C ) ? "yes" : "no" );
		print( buffer, "AVX512: %s\n", Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ? "yes" : "no" );
		break;
	case CPUArch::CPU_MIPS32:
	case CPUArch::CPU_MIPS64:
		print( buffer, "FPU-PS: %s\n", Info.HasInstructionSet( CPUFeature::MIPS_PS ) ? "yes" : "no" );
		print( buffer, "MSA   : %s\n", Info.HasInstructionSet( CPUFeature::MIPS_MSA ) ? "yes" : "no" );
		break;
	default:
		break;
	}

}


void BenchApplication::ExportFlops( ut::BinaryBuffer& buffer ) const
{
	print( buffer, "\nTotal:\n" );
	format_GFLOPS( buffer, "SingleThread HP max", GetMaxMFLOPS( LOOPTYPE_HP, false ) );
	format_GFLOPS( buffer, "SingleThread SP max", GetMaxMFLOPS( LOOPTYPE_SP, false ) );
	format_GFLOPS( buffer, "SingleThread DP max", GetMaxMFLOPS( LOOPTYPE_DP, false ) );
	format_GFLOPS( buffer, "MultiThread  HP max", GetTotalMFLOPS( LOOPTYPE_HP, true  ) );
	format_GFLOPS( buffer, "MultiThread  SP max", GetTotalMFLOPS( LOOPTYPE_SP, true  ) );
	format_GFLOPS( buffer, "MultiThread  DP max", GetTotalMFLOPS( LOOPTYPE_DP, true  ) );


	unsigned int	group_count= Info.GetCoreGroupCount();
	for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
		print( buffer, "\nGroup %d:  Thread=%d  Clock=%f GHz  (mask:%llx)\n", gi, Info.GetThreadCount( gi ), Info.GetCoreClock( gi )/1000000.0, Info.GetAffinityMask( gi ) );
		format_GFLOPS( buffer, "  SingleThread HP max", GetMFLOPS( LOOPTYPE_HP, false, gi ) );
		format_GFLOPS( buffer, "  SingleThread SP max", GetMFLOPS( LOOPTYPE_SP, false, gi ) );
		format_GFLOPS( buffer, "  SingleThread DP max", GetMFLOPS( LOOPTYPE_DP, false, gi ) );
		format_GFLOPS( buffer, "  MultiThread  HP max", GetMFLOPS( LOOPTYPE_HP, true,  gi ) );
		format_GFLOPS( buffer, "  MultiThread  SP max", GetMFLOPS( LOOPTYPE_SP, true,  gi ) );
		format_GFLOPS( buffer, "  MultiThread  DP max", GetMFLOPS( LOOPTYPE_DP, true,  gi ) );
	}
	print( buffer, "\n" );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	BenchApplication::ExportLine( ut::BinaryBuffer& buffer, const ResultLine& line )
{
	if( line.IsActive() ){
#if USE_MAX_EXPORT
		print( buffer, "%-34s: %8.3f  %9.1f  %9.1f  (%5.1f %3.1f) %9.1f\n",
				line.Title,
				line.Time,
				line.Flops,
				line.Ops,
				line.Fop,
				line.Ipc,
				line.Max
			);
#else
		print( buffer, "%-34s: %8.3f  %9.1f  %9.1f  (%5.1f %3.1f)\n",
				line.Title,
				line.Time,
				line.Flops,
				line.Ops,
				line.Fop,
				line.Ipc
			);
#endif
	}else{
#if USE_MAX_EXPORT
		print( buffer, "%-34s:        -          -          -       -   -          -\n", line.Title );
#else
		print( buffer, "%-34s:        -          -          -       -   -\n", line.Title );
#endif
	}
}


void	BenchApplication::ExportData( ut::BinaryBuffer& buffer, const ResultData& data )
{
	unsigned int	count= data.GetSize();
	unsigned int	group= data.GetCoreGroup();
	print( buffer, "\n* Group %d:  Thread=%d  Clock=%f GHz  (mask:%llx)\n", group, data.IsMultithread() ? Info.GetThreadCount( group ) : 1, Info.GetCoreClock( group )/1000000.0, Info.GetAffinityMask( group ) );
	print( buffer, "* %s\n", data.GetTitle() );
	//				0123456789012345678901234567890123: ________  _________  _________ (___ ___) _________
#if USE_MAX_EXPORT
	print( buffer, "                                      TIME(s)   MFLOPS      MOPS     FOP   IPC  max MFLOPS\n" );
#else
	print( buffer, "                                      TIME(s)   MFLOPS      MOPS     FOP   IPC\n" );
#endif
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		ExportLine( buffer, data.Get( ci ) );
	}
	print( buffer, "\n" );
}



void	BenchApplication::ExportLog( ut::BinaryBuffer& buffer ) const
{
	ut::BinaryBuffer	info;
	if( *DateTimeStr ){
		print( buffer, "Date: %s\n", DateTimeStr );
	}
	ExportCPUInfo( info );
	ExportFlops( info );
	save_size( buffer, info.GetTop(), info.GetSize() );

	unsigned int	bcount= GetDataCount();
	for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
		ExportData( buffer, GetData( bi ) );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
T DateTime
N ProcessorName
D Data
L Line
*/

void	BenchApplication::LoadLine( ResultLine& line, const char* ptr )
{
	const int	WORD_BUFFER_SIZE= 64;
	char	word_buffer[WORD_BUFFER_SIZE];
	ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
	ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
	if( strcmp( line.Title, word_buffer ) != 0 ){
		return;
	}
	FL_ASSERT( (&line.Max - &line.Time + 1) == 6 );
	double*	dptr= &line.Time;
	for( int i= 0 ; i< 6 ; i++ ){
		ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
		*dptr++= atof( word_buffer );
	}
}

void	BenchApplication::LoadFile( const char* file_name )
{
	FILE*	fp= fopen( file_name, "rb" );
	if( !fp ){
		return;
	}
	unsigned int	data_count= GetDataCount();
	const int	LINE_BUFFER_SIZE= 256;
	char		line_buffer[LINE_BUFFER_SIZE];
	const int	WORD_BUFFER_SIZE= 64;
	char		word_buffer[WORD_BUFFER_SIZE];
	unsigned int	line_index= 0;
	ResultData*	current_data= nullptr;
	for(; fgets( line_buffer, LINE_BUFFER_SIZE-2, fp ) ;){
		const char*	ptr= SkipSpace( line_buffer );
		if( *ptr == '#' || *ptr == '\n' ){
			continue;
		}
		if( *ptr == 'T' ){
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( DateTimeStr, ArraySize::GetArraySize( DateTimeStr )-2, ptr );
			continue;
		}
		if( *ptr == 'D' ){
			line_index= 0;
			current_data= nullptr;
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			unsigned int	line_count= atoi( word_buffer );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			unsigned int	core_group= atoi( word_buffer );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			for( unsigned int bi= 0 ; bi< data_count ; bi++ ){
				ResultData&	data= GetData( bi );
				if( data.GetCoreGroup() == core_group && data.GetSize() == line_count && strcmp( data.GetTitle(), word_buffer ) == 0 ){
					current_data= &data;
					line_index= 0;
					break;
				}
			}
		}
		if( current_data ){
			if( *ptr == 'L' ){
				ResultLine&	line= current_data->Get( line_index++ );
				LoadLine( line, ptr );
			}
		}
	}
	fclose( fp );
}

void	BenchApplication::SaveLine( ut::BinaryBuffer& buffer, const ResultLine& line ) const
{
	print( buffer, "L \"%s\" %f %f %f %f %f %f\n",
			line.Title,
			line.Time,
			line.Flops,
			line.Ops,
			line.Fop,
			line.Ipc,
			line.Max
		);
}

void	BenchApplication::SaveData( ut::BinaryBuffer& buffer, const ResultData& data ) const
{
	unsigned int	count= data.GetSize();
	unsigned int	group= data.GetCoreGroup();
	print( buffer, "# Group=%d Thread=%d Clock=%d Mask=%llx\n", group, Info.GetThreadCount( group ), Info.GetCoreClock( group ), Info.GetAffinityMask( group ) );
	print( buffer, "D %d %d %d \"%s\"\n", data.GetBenchIndex(), count, group, data.GetTitle() );
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		SaveLine( buffer, data.Get( ci ) );
	}
}

void	BenchApplication::SaveFile( const char* file_name ) const
{
	ut::BinaryBuffer	buffer;
	if( *DateTimeStr ){
		print( buffer, "T \"%s\"\n", DateTimeStr );
	}

	unsigned int	bcount= GetDataCount();
	for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
		SaveData( buffer, GetData( bi ) );
	}
	{
		FILE*	fp= fopen( file_name, "wb" );
		if( fp ){
			fwrite( buffer.GetTop(), buffer.GetSize(), 1, fp );
			fclose( fp );
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

BenchApplication::BenchApplication()
{
	*DateTimeStr= '\0';
}


void	BenchApplication::Init( unsigned int bench_count )
{
	DataArray.Init( bench_count );
}


void	BenchApplication::InitData( unsigned int btype, ITestBase* bench )
{
	ResultData&	data= GetData( btype );
	data.SetInfo( bench->GetTestName(), bench->GetLoopType(), btype, bench->IsMultithread(), bench->GetCoreGroup(), Info.GetCoreClock( bench->GetCoreGroup() ) );

	unsigned int	title_size= bench->GetResultCount();
	data.InitSize( title_size );
	for( unsigned int ti= 0 ; ti< title_size ; ti++ ){
		ResultLine&	line= data.Get( ti );
		line.Title= bench->GetInstructionName( ti );
	}
}


double	BenchApplication::GetMFLOPS( unsigned int loop_type, bool multithread, unsigned int group ) const
{
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type && data.GetCoreGroup() == group ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				return	line.Max;
			}
		}
	}
	return	0.0;
}

double	BenchApplication::GetTotalMFLOPS( unsigned int loop_type, bool multithread ) const
{
	double	total= 0.0f;
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				total+= line.Max;
			}
		}
	}
	return	total;
}

double	BenchApplication::GetMaxMFLOPS( unsigned int loop_type, bool multithread ) const
{
	double	max_flops= 0.0f;
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				if( line.Max > max_flops ){
					max_flops= line.Max;
				}
			}
		}
	}
	return	max_flops;
}



void BenchApplication::UpdateResult( unsigned int btype, ITestBase* bench )
{
	ResultData&	data= GetData( btype );
	data.UpdateBegin();
	unsigned int	count= bench->GetResultInfo( InfoType::INFO_COUNT );
	double			loop= bench->GetResultInfo( InfoType::INFO_LOOP );
	unsigned int	group= bench->GetCoreGroup();
	unsigned int	kclock= Info.GetCoreClock( group );
	double			mclock= kclock / 1000.0;
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		double	time= bench->GetResult( ci ) / 1000000.0;
		double	float_op= bench->GetLoopOp( ci );
		double	inst_fop= bench->GetInstFop( ci );
		//printf( "float_op=%f  inst_fop=%f\n", float_op, inst_fop );
		double	flops= 0.0;
		double	ops= 0.0;
		double	ipc= 0.0;
		if( time >= 1e-5 ){
			flops= (float_op * loop / 1000000.0) / time;		// MFLOPS
			ops= (float_op * loop / inst_fop / 1000000.0) / time;	// MOPS
			if( kclock ){
				ipc= ops / mclock;
			}
		}
		data.Update( time, flops, ops, inst_fop, ipc );
	}
	data.UpdateEnd();
}


void	BenchApplication::UpdateTimestamp()
{
	time::DateTime	time;
	time::GetLocalTime( time );
	size_t	length= ArraySize::GetArraySize( DateTimeStr );
	snprintf( DateTimeStr, length-2, "%04d%02d%02d %02d%02d%02d", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second );
	DateTimeStr[length-1]= '\0';
}





