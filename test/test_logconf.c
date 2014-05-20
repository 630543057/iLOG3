#include "LOGCONF.h"

int test_logconf()
{
	LOG	*g = NULL ;
	char	buffer[ 64 + 1 ] = "" ;
	long	buflen = sizeof(buffer) - 1 ;
	
	g = CreateLogHandleFromConfig( "$HOME$/etc/test_logconf.conf" , NULL ) ;
	if( g == NULL )
	{
		printf( "�������ļ�������־���ʧ��\n" );
		return -1;
	}
	
	/*
	SetLogOutput( g , LOG_OUTPUT_NOSET , "$HOME$/log/test_logconf_1.log" , LOG_NO_OUTPUTFUNC );
	*/
	
	printf( "������־����ɹ�\n" );
	
	DebugLog( g , __FILE__ , __LINE__ , "hello LOGCONF" );
	InfoLog( g , __FILE__ , __LINE__ , "hello LOGCONF" );
	WarnLog( g , __FILE__ , __LINE__ , "hello LOGCONF" );
	ErrorLog( g , __FILE__ , __LINE__ , "hello LOGCONF" );
	FatalLog( g , __FILE__ , __LINE__ , "hello LOGCONF" );
	
	DebugHexLog( g , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	InfoHexLog( g , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	WarnHexLog( g , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	ErrorHexLog( g , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	FatalHexLog( g , __FILE__ , __LINE__ , buffer , buflen , "������[%ld]" , buflen );
	
	DestroyLogHandle( g );
	printf( "������־���\n" );
	
	return 0;
}

int main()
{
	return -test_logconf();
}
