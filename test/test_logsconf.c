#include "LOGSCONF.h"

int test_logsconf( char *program )
{
	LOGS	*gs = NULL ;
	char	buffer[ 64 + 1 ] = "" ;
	long	buflen = sizeof(buffer) - 1 ;
	
#if ( defined _WIN32 )
	if( strstr( program , ".exe" ) )
	{
		strstr( program , ".exe" )[0] = '\0' ;
	}
#endif
	gs = CreateLogsHandleFromConfig( program , ".conf" ) ;
	/*
	if( gs == NULL )
	{
		printf( "�������ļ�������־�������ʧ��\n" );
		return -1;
	}
	*/
	printf( "������־������ϳɹ�\n" );
	
	DebugLogs( gs , __FILE__ , __LINE__ , "hello LOGSCONF" );
	InfoLogs( gs , __FILE__ , __LINE__ , "hello LOGSCONF" );
	WarnLogs( gs , __FILE__ , __LINE__ , "hello LOGSCONF" );
	ErrorLogs( gs , __FILE__ , __LINE__ , "hello LOGSCONF" );
	FatalLogs( gs , __FILE__ , __LINE__ , "hello LOGSCONF" );
	
	DebugHexLogs( gs , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	InfoHexLogs( gs , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	WarnHexLogs( gs , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	ErrorHexLogs( gs , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	FatalHexLogs( gs , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	
	DestroyLogsHandle( gs );
	printf( "������־�������\n" );
	
	return 0;
}

int main( int argc , char *argv[] )
{
	return -test_logsconf( argv[0] );
}
