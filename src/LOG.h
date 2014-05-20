#ifndef _H_LOG_
#define _H_LOG_

/*
 * iLOG3 - log function library written in c
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 * LastVersion	: v1.0.6
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#ifdef __cplusplus
extern "C" {
#endif

#if ( defined _WIN32 )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC		_declspec(dllexport)
#endif
#elif ( defined __unix ) || ( defined __linux__ )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#if ( defined _WIN32 )
#include <windows.h>
#include <share.h>
#include <io.h>
#include <fcntl.h>
#elif ( defined __unix ) || ( defined __linux__ )
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <pthread.h>
#endif

#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
struct tm *localtime_r(const time_t *timep, struct tm *result);
#endif

/* ��������ֵ�� */
#define LOG_RETURN_ERROR_ALLOC		-11	/* �����ڴ�ʧ�� */
#define LOG_RETURN_ERROR_INTERNAL	-12	/* �ڲ����� */
#define LOG_RETURN_ERROR_ALLOC_MAX	-13	/* �ڴ�ʹ�ó��� */
#define LOG_RETURN_ERROR_PARAMETER	-14	/* �������� */
#define LOG_RETURN_ERROR_NOTSUPPORT	-17	/* �ݲ�֧�� */
#define LOG_RETURN_ERROR_CREATEFILE	-21	/* �����ļ�ʧ�� */
#define LOG_RETURN_ERROR_OPENFILE	-22	/* ���ļ�ʧ�� */
#define LOG_RETURN_ERROR_WRITEFILE	-23	/* д�ļ�ʧ�� */

/********************************************************/
/*                    ����Ϊ��������                    */
/********************************************************/

/* ��־������� */
#define LOG_OUTPUT_NOSET	-1	/* ������ */
#define LOG_OUTPUT_STDOUT	1	/* ��׼��� */
#define LOG_OUTPUT_STDERR	2	/* ��׼������� */
#define LOG_OUTPUT_SYSLOG	3	/* UNIX&Linux��syslog �� Windows��WINDOWS EVENT */
#define LOG_OUTPUT_FILE		11	/* �ļ� */
#define LOG_OUTPUT_CALLBACK	21	/* ��ʹ���Զ�����־����ص������� */

/* ��־�ȼ��� */
#define LOG_LEVEL_DEBUG		0	/* ���Եȼ� */
#define LOG_LEVEL_INFO		1	/* ��ͨ��Ϣ�ȼ� */
#define LOG_LEVEL_WARN		2	/* ����ȼ� */
#define LOG_LEVEL_ERROR		3	/* ����ȼ� */
#define LOG_LEVEL_FATAL		4	/* ���ش���ȼ� */
#define LOG_LEVEL_NOLOG		5	/* ����Ҫ�����־ */

#define LOG_LEVEL_DEFAULT	LOG_LEVEL_NOLOG

/* ����־�����Ϻ� */
#define LOG_STYLE_DATE		1	/* ����"YYYY-MM-DD" */
#define LOG_STYLE_DATETIME	2	/* ����ʱ��"YYYY-MM-DD hh:mm:ss" */
#define LOG_STYLE_DATETIMEMS	4	/* ����ʱ�����"YYYY-MM-DD hh:mm:ss.6ms"������ʱ����껥�⣬�����Զ�ѡ����Ϣ�����ģ� */
#define LOG_STYLE_LOGLEVEL	8	/* ��־�ȼ� */
#define LOG_STYLE_PID		16	/* ����id */
#define LOG_STYLE_TID		32	/* �߳�id */
#define LOG_STYLE_SOURCE	64	/* "Դ�����ļ���:Դ�����к�" */
#define LOG_STYLE_FORMAT	128	/* Ӧ����־�� */
#define LOG_STYLE_NEWLINE	256	/* ���� */
#define LOG_STYLE_CUSTLABEL1	512	/* �Զ����ǩ1 */
#define LOG_STYLE_CUSTLABEL2	1024	/* �Զ����ǩ2 */
#define LOG_STYLE_CUSTLABEL3	2048	/* �Զ����ǩ3 */
#define LOG_STYLE_CALLBACK	4096	/* ��ʹ���Զ�������־���ص������� */

#define LOG_STYLES_DEFAULT	0

/* ������ */
#ifndef MAXLEN_FILENAME
#define MAXLEN_FILENAME		256
#endif

typedef struct tagLOG LOG ;
typedef struct tagLOGBUF LOGBUF ;

/* ������־������� */
_WINDLL_FUNC LOG *CreateLogHandle();
_WINDLL_FUNC void DestroyLogHandle( LOG *g );

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
_WINDLL_FUNC LOG *CreateLogHandleG();
_WINDLL_FUNC void DestroyLogHandleG();
#endif

/* �Զ���򿪡�������ر���־�������� */
typedef int funcOpenLog( LOG *g , char *log_pathfilename , void **open_handle );
typedef int funcWriteLog( LOG *g , void **open_handle , int log_level , char *buf , long len , long *writelen );
typedef int funcChangeTest( LOG *g , void **test_handle );
typedef int funcCloseLog( LOG *g , void **open_handle );

#define LOG_NO_OUTPUTFUNC	NULL , NULL , NULL , NULL , NULL , NULL

/* �Զ�������־��������� */
typedef int funcLogStyle( LOG *g , LOGBUF *logbuf , char *c_filename , long c_fileline , int log_level , char *format , va_list valist );

#define LOG_NO_STYLEFUNC	NULL

/* ����������ú��� */
_WINDLL_FUNC int SetLogOutput( LOG *g , int output , char *log_pathfilename , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogLevel( LOG *g , int log_level );
_WINDLL_FUNC int SetLogStyles( LOG *g , long log_styles , funcLogStyle *pfuncLogStyle );

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
_WINDLL_FUNC int SetLogOutputG( int output , char *log_pathfilename , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogLevelG( int log_level );
_WINDLL_FUNC int SetLogStylesG( long log_styles , funcLogStyle *pfuncLogStyles );
#endif

/* д��־���� */
_WINDLL_FUNC int WriteLog( LOG *g , char *c_filename , long c_fileline , int log_level , char *format , ... );
_WINDLL_FUNC int DebugLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int InfoLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WarnLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int ErrorLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int FatalLog( LOG *g , char *c_filename , long c_fileline , char *format , ... );
/*
ʹ��ʾ��
InfoLog( g , __FILE__ , __LINE__ , "xxx������ɣ������[%d]" , nret );
*/

/* д��־�����Ŀɱ������ */
#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
#define WRITELOG( _g_ , _log_level_ , ... )	WriteLog( _g_ , __FILE__ , __LINE__ , _log_level_ , __VA_ARGS__ );
#define DEBUGLOG( _g_ , ... )			DebugLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define INFOLOG( _g_ , ... )			InfoLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define WARNLOG( _g_ , ... )			WarnLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define ERRORLOG( _g_ , ... )			ErrorLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
#define FATALLOG( _g_ , ... )			FatalLog( _g_ , __FILE__ , __LINE__ , __VA_ARGS__ );
/*
ʹ��ʾ��
INFOLOG( g , "xxx������ɣ������[%d]" , nret );
*/
#endif

/* дʮ�����ƿ���־���� */
_WINDLL_FUNC int WriteHexLog( LOG *g , char *c_filename , long c_fileline , int log_level , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int DebugHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int InfoHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WarnHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int ErrorHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int FatalHexLog( LOG *g , char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
/* дʮ�����ƿ���־�Ŀɱ������ */
#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
#define WRITEHEXLOG( _g_ , _log_level_ , _buf_ , _buf_size_ , ... )	WriteHexLog( _g_ , __FILE__ , __LINE__ , _log_level_ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define DEBUGHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			DebugHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define INFOHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			InfoHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define WARNHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			WarnHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define ERRORHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			ErrorHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define FATALHEXLOG( _g_ , _buf_ , _buf_size_ , ... )			FatalHexLog( _g_ , __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#endif

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
/* д��־�����������̱߳��ش洢��ȱʡ��־����ĺ������ϰ汾�� */
_WINDLL_FUNC int WriteLogG( char *c_filename , long c_fileline , int log_level , char *format , ... );
_WINDLL_FUNC int DebugLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int InfoLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int WarnLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int ErrorLogG( char *c_filename , long c_fileline , char *format , ... );
_WINDLL_FUNC int FatalLogG( char *c_filename , long c_fileline , char *format , ... );
/*
ʹ��ʾ��
InfoLogG( __FILE__ , __LINE__ , "xxx������ɣ������[%d]" , nret );
*/
/* д��־�����Ŀɱ�����꣨�����̱߳��ش洢��ȱʡ��־����ĺ������ϰ汾�� */
#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
#define WRITELOGG( _log_level_ , ... )	WriteLogG( __FILE__ , __LINE__ , _log_level_ , __VA_ARGS__ );
#define DEBUGLOGG( ... )		DebugLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define INFOLOGG( ... )			InfoLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define WARNLOGG( ... )			WarnLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define ERRORLOGG( ... )		ErrorLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
#define FATALLOGG( ... )		FatalLogG( __FILE__ , __LINE__ , __VA_ARGS__ );
/*
ʹ��ʾ��
INFOLOGG( "xxx������ɣ������[%d]" , nret );
*/
#endif
#endif

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
/* дʮ�����ƿ���־�����������̱߳��ش洢��ȱʡ��־����ĺ������ϰ汾�� */
_WINDLL_FUNC int WriteHexLogG( char *c_filename , long c_fileline , int log_level , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int DebugHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int InfoHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int WarnHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int ErrorHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
_WINDLL_FUNC int FatalHexLogG( char *c_filename , long c_fileline , char *buffer , long buflen , char *format , ... );
/* дʮ�����ƿ���־�Ŀɱ�����꣨�����̱߳��ش洢��ȱʡ��־����ĺ������ϰ汾�� */
#if ( defined __STDC_VERSION__ ) && ( __STDC_VERSION__ >= 199901 )
#define WRITEHEXLOGG( _log_level_ , _buf_ , _buf_size_ , ... )	WriteHexLogG( __FILE__ , __LINE__ , _log_level_ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define DEBUGHEXLOGG( _buf_ , _buf_size_ , ... )		DebugHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define INFOHEXLOGG( _buf_ , _buf_size_ , ... )			InfoHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define WARNHEXLOGG( _buf_ , _buf_size_ , ... )			WarnHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define ERRORHEXLOGG( _buf_ , _buf_size_ , ... )		ErrorHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#define FATALHEXLOGG( _buf_ , _buf_size_ , ... )		FatalHexLogG( __FILE__ , __LINE__ , _buf_ , _buf_size_ , __VA_ARGS__ );
#endif
#endif

/********************************************************/
/*                    ����Ϊ�߼�����                    */
/********************************************************/

/* �Զ�������־�ȼ��ص��������� */
typedef int funcFilterLog( LOG *g , void **open_handle , int log_level , char *buf , long len );

/* �Զ����ǩ���� */
#define LOG_MAXCNT_CUST_LABEL		3
/* �Զ����ǩ��󳤶� */
#define LOG_MAXLEN_CUST_LABEL		32

/* ��־ѡ�� */
#define LOG_OPTION_OPEN_AND_CLOSE		1	/* ÿ�ζ�����־��д��־���ر���־ */
#define LOG_OPTION_CHANGE_TEST			2	/* ����ļ��䶯 */
#define LOG_OPTION_OPEN_ONCE			4	/* ��־��һ�β��ر� */
#define LOG_OPTION_SET_OUTPUT_BY_FILENAME	8	/* �Զ������ļ�������������� */
							/* "#stdout" -> LOG_OUTPUT_STDOUT */
							/* "#stderr" -> LOG_OUTPUT_STDERR */
							/* "#syslog" -> LOG_OUTPUT_SYSLOG */
#define LOG_OPTION_FILENAME_APPEND_DOT_LOG	16	/* ��־����ļ������Զ�����".log" */

#define LOG_OPTION_OPEN_DEFAULT			LOG_OPTION_CHANGE_TEST

/* ��־ת��ģʽ */
#define LOG_ROTATEMODE_NONE		0	/* ��ת�� */
#define LOG_ROTATEMODE_SIZE		1	/* ����־�ļ���Сת�����ͺ���SetLogRotateSize���ʹ�ã�ת���ļ�����ʽ"ԭ��־�ļ���.���" */
#define LOG_ROTATEMODE_PER_DAY		2	/* ��ÿ��ת����ת���ļ�����ʽ"ԭ��־�ļ���.����������������" */
#define LOG_ROTATEMODE_PER_HOUR		3	/* ��Сʱת����ת���ļ�����ʽ"ԭ��־�ļ���.����������������_Сʱ" */

/* ��־ת��ȱʡֵ */
#define LOG_ROTATE_SIZE_FILE_COUNT_DEFAULT		99999999
#define LOG_ROTATE_SIZE_PRESSURE_FACTOR_DEFAULT		2

/* �Զ�����־ת��ǰ��ص��������� */
typedef int funcBeforeRotateFile( LOG *g , char *rotate_log_pathfilename );
typedef int funcAfterRotateFile( LOG *g , char *rotate_log_pathfilename );

/* ��������Сȱʡֵ */
#define LOG_BUFSIZE_DEFAULT		(1024)		/* ȱʡ����־��������С */
#define LOG_BUFSIZE_MAX			(16*1024)	/* �������־��������С */
#define LOG_HEXLOG_BUFSIZE_DEFAULT	(1024*1024)	/* ȱʡʮ�����ƿ���־��������С */
#define LOG_HEXLOG_BUFSIZE_MAX		(16*1024*1024)	/* ���ʮ�����ƿ���־��������С */

/* �߼�����������ú��� */
_WINDLL_FUNC int SetLogOptions( LOG *g , int log_options );
_WINDLL_FUNC int SetLogFileChangeTest( LOG *g , long interval );
_WINDLL_FUNC int SetLogCustLabel( LOG *g , int index , char *cust_label );
_WINDLL_FUNC int SetLogRotateMode( LOG *g , int rotate_mode );
_WINDLL_FUNC int SetLogRotateSize( LOG *g , long log_rotate_size );
_WINDLL_FUNC int SetLogRotatePressureFactor( LOG *g , long pressure_factor );
_WINDLL_FUNC int SetBeforeRotateFileFunc( LOG *g , funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetAfterRotateFileFunc( LOG *g , funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetFilterLogFunc( LOG *g , funcFilterLog *pfuncFilterLog );
_WINDLL_FUNC int SetLogBufferSize( LOG *g , long log_bufsize , long max_log_bufsize );
_WINDLL_FUNC int SetHexLogBufferSize( LOG *g , long hexlog_bufsize , long max_log_hexbufsize );
_WINDLL_FUNC int SetLogOutputFuncDirectly( LOG *g , funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogStyleFuncDirectly( LOG *g , funcLogStyle *pfuncLogStyle );

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
_WINDLL_FUNC int SetLogOptionsG( int log_options );
_WINDLL_FUNC int SetLogFileChangeTestG( long interval );
_WINDLL_FUNC int SetLogCustLabelG( int index , char *cust_label );
_WINDLL_FUNC int SetLogRotateModeG( int rotate_mode );
_WINDLL_FUNC int SetLogRotateSizeG( long log_rotate_size );
_WINDLL_FUNC int SetLogRotatePressureFactorG( long pressure_factor );
_WINDLL_FUNC int SetBeforeRotateFileFuncG( funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetAfterRotateFileFuncG( funcAfterRotateFile *pfuncAfterRotateFile );
_WINDLL_FUNC int SetFilterLogFuncG( funcFilterLog *pfuncFilterLog );
_WINDLL_FUNC int SetLogBufferSizeG( long log_bufsize , long max_log_bufsize );
_WINDLL_FUNC int SetHexLogBufferSizeG( long hexlog_bufsize , long max_log_hexbufsize );
_WINDLL_FUNC int SetLogOutputFuncDirectlyG( funcOpenLog *pfuncOpenLogFirst , funcOpenLog *pfuncOpenLog , funcWriteLog *pfuncWriteLog , funcChangeTest *pfuncChangeTest , funcCloseLog *pfuncCloseLog , funcCloseLog *pfuncCloseLogFinally );
_WINDLL_FUNC int SetLogStyleFuncDirectlyG( funcLogStyle *pfuncLogStyle );
#endif

#define TEST_LOGLEVEL_NOTENOUGH(_log_level_,_g_)	( (_log_level_) < (_g_)->log_level )
#define TEST_ATTRIBUTE(_param_,_attr_)			( ( _param_ & _attr_ ) == _attr_ )
#define TEST_NOT_ATTRIBUTE(_param_,_attr_)		( ( _param_ & _attr_ ) != _attr_ )
#define TEST_STDSTREAM(_fd_)				( (_fd_) != FD_NULL && ( (_fd_) == STDOUT_HANDLE || (_fd_) == STDERR_HANDLE ) )
#define TEST_FILEFD(_fd_)				( (_fd_) != FD_NULL && (_fd_) != STDOUT_HANDLE && (_fd_) != STDERR_HANDLE )

#if ( defined _WIN32 ) || ( defined __linux__ ) || ( defined _AIX )
/* �õ������̱߳��ش洢��ȱʡ��־����ĺ����汾 */
_WINDLL_FUNC LOG *GetGlobalLOG();
_WINDLL_FUNC void SetGlobalLOG( LOG *g );
#endif

/********************************************************/
/*                    ����Ϊ���ӿ���                    */
/********************************************************/

/* ƽ̨�� */
#if ( defined _WIN32 )
#define LOG_NEWLINE		"\r\n"
#define LOG_NEWLINE_LEN		2
#define SNPRINTF		_snprintf
#define VSNPRINTF		_vsnprintf
#define STDOUT_HANDLE		1
#define STDERR_HANDLE		2
#define FD_NULL			-1
#define WRITE			_write
#define STAT			_stat
#define ACCESS			_access
#define ACCESS_OK		00
#define RENAME			rename
#elif ( defined __unix ) || ( defined __linux__ )
#define LOG_NEWLINE		"\n"
#define LOG_NEWLINE_LEN		1
#define SNPRINTF		snprintf
#define VSNPRINTF		vsnprintf
#define STDOUT_HANDLE		STDOUT_FILENO
#define STDERR_HANDLE		STDERR_FILENO
#define FD_NULL			-1
#define WRITE			write
#define STAT			stat
#define ACCESS			access
#define ACCESS_OK		F_OK
#define RENAME			rename
#endif

/* ����� */
#if ( defined _WIN32 )
#define SYSTEMTIME2TIMEVAL_USEC(_syst_,_tv_) \
		(_tv_).tv_usec = (_syst_).wMilliseconds * 1000 ;
#define SYSTEMTIME2TM(_syst_,_stime_) \
		(_stime_).tm_year = (_syst_).wYear - 1900 ; \
		(_stime_).tm_mon = (_syst_).wMonth - 1 ; \
		(_stime_).tm_mday = (_syst_).wDay ; \
		(_stime_).tm_hour = (_syst_).wHour ; \
		(_stime_).tm_min = (_syst_).wMinute ; \
		(_stime_).tm_sec = (_syst_).wSecond ;
#define LOCALTIME(_tt_,_stime_) \
	{ \
		SYSTEMTIME	stNow ; \
		GetLocalTime( & stNow ); \
		SYSTEMTIME2TM( stNow , (_stime_) ); \
	}
#elif ( defined __unix ) || ( defined __linux__ )
#define LOCALTIME(_tt_,_stime_) \
	localtime_r(&(_tt_),&(_stime_));
#endif

#ifndef STRCMP
#define STRCMP(_a_,_C_,_b_) ( strcmp(_a_,_b_) _C_ 0 )
#define STRNCMP(_a_,_C_,_b_,_n_) ( strncmp(_a_,_b_,_n_) _C_ 0 )
#endif

#ifndef STRICMP
#if ( defined _WIN32 )
#define STRICMP(_a_,_C_,_b_) ( stricmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strnicmp(_a_,_b_,_n_) _C_ 0 )
#elif ( defined __unix ) || ( defined __linux__ )
#define STRICMP(_a_,_C_,_b_) ( strcasecmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strncasecmp(_a_,_b_,_n_) _C_ 0 )
#endif
#endif

#define OFFSET_BUFPTR(_logbuf_,_offset_len_) \
	if( _offset_len_ != -1 ) \
	{ \
		(_logbuf_)->bufptr += _offset_len_ ; \
		(_logbuf_)->buf_remain_len -= _offset_len_ ; \
	}

_WINDLL_FUNC int SetOpenFlag( LOG *g , char open_flag );
_WINDLL_FUNC char IsLogOpened( LOG *g );

_WINDLL_FUNC int GetLogLevel( LOG *g );

_WINDLL_FUNC LOGBUF *GetLogBuffer( LOG *g );
_WINDLL_FUNC LOGBUF *GetHexLogBuffer( LOG *g );

_WINDLL_FUNC long FormatLogBuffer( LOG *g , LOGBUF *logbuf , char *format , ... );
_WINDLL_FUNC long FormatLogBufferV( LOG *g , LOGBUF *logbuf , char *format , va_list valist );
_WINDLL_FUNC long MemcatLogBuffer( LOG *g , LOGBUF *logbuf , char *append , long len );

/* ��־�������ṹ */
struct tagLOGBUF
{
	long			max_buf_size ;
	long			buf_size ;
	char			*bufbase ;
	char			*bufptr ;
	long			buf_remain_len ;
} ;

/* ��־����ṹ */
struct tagLOG
{
	/* ��־�ļ��� */
	int			output ;
	char			log_pathfilename[ MAXLEN_FILENAME + 1 ];
	funcOpenLog		*pfuncOpenLogFirst ;
	funcOpenLog		*pfuncOpenLog ;
	funcWriteLog		*pfuncWriteLog ;
	funcChangeTest		*pfuncChangeTest ;
	funcCloseLog		*pfuncCloseLog ;
	funcCloseLog		*pfuncCloseLogFinally ;
	void			*open_handle ;
	void			*test_handle ;
	
	int			fd ;
#if ( defined _WIN32 )
	HANDLE			hFile ;
#endif
	
	char			open_flag ;
	
	/* ��־�ȼ� */
	int			log_level ;
	
	funcFilterLog		*pfuncFilterLog ;
	
	/* ����־��� */
	long			log_styles ;
	funcLogStyle		*pfuncLogStyle ;
	/* �����и�ʽ��񣬹�����ʽ�������飬����д��־ʱ�����������־������ */
	funcLogStyle		*pfuncLogStyles[ 30 + 1 ] ;
	int			style_func_count ;
	
	/* ��־ѡ�� */
	int			log_options ;
	
	long			file_change_test_interval ;
	long			file_change_test_no ;
	struct STAT		file_change_stat ;
	
	/* �Զ����ǩ */
	char			cust_label[LOG_MAXCNT_CUST_LABEL][ LOG_MAXLEN_CUST_LABEL + 1 ] ;
	
	/* ��־ת����С */
	int			rotate_mode ;
	long			log_rotate_size ;
	long			rotate_file_no ;
	long			rotate_file_count ;
	long			pressure_factor ;
	
	long			skip_count ;
	
	funcAfterRotateFile	*pfuncAfterRotateFile ;
	funcBeforeRotateFile	*pfuncBeforeRotateFile ;
	
	/* ����־������ */
	LOGBUF			logbuf ;
	
	/* ʮ��������־������ */
	LOGBUF			hexlogbuf ;
	
	/* һ������ */
	struct timeval		cache1_tv ;
	struct tm		cache1_stime ;
	
	/* �������� */
	struct timeval		cache2_logstyle_tv ;
	struct tm		cache2_logstyle_stime ;
	char			cache2_logstyle_date_buf[ 10 + 1 ] ;
	long			cache2_logstyle_date_buf_len ;
	char			cache2_logstyle_datetime_buf[ 19 + 1 ] ;
	long			cache2_logstyle_datetime_buf_len ;
	
	unsigned long		cache2_logstyle_pid ;
	char			cache2_logstyle_pid_buf[ 20 + 1 ] ;
	long			cache2_logstyle_pid_buf_len ;
	
	unsigned long		cache2_logstyle_tid ;
	char			cache2_logstyle_tid_buf[ 20 + 1 ] ;
	long			cache2_logstyle_tid_buf_len ;
	
	struct timeval		cache2_rotate_tv ;
	struct tm		cache2_rotate_stime ;
	
	/* ת���ļ��� */
#if ( defined _WIN32 )
	HANDLE			rotate_lock ;
#elif ( defined __unix ) || ( defined __linux__ )
	int			rotate_lock ;
	struct flock		lock ;
#endif
} ;

/********************************************************/
/*                    ����Ϊ�����ӿ�                    */
/********************************************************/

/* ���ø������� */
_WINDLL_FUNC int ConvertLogOutput_atoi( char *output_desc , int *p_log_output );
_WINDLL_FUNC int ConvertLogLevel_atoi( char *log_level_desc , int *p_log_level );
_WINDLL_FUNC int ConvertLogLevel_itoa( int log_level , char **log_level_desc );
_WINDLL_FUNC int ConvertLogStyle_atol( char *line_style_desc , long *p_line_style );
_WINDLL_FUNC int ConvertLogOption_atol( char *log_option_desc , long *p_log_option );
_WINDLL_FUNC int ConvertLogRotateMode_atoi( char *rotate_mode_desc , int *p_rotate_mode );
_WINDLL_FUNC int ConvertBufferSize_atol( char *bufsize_desc , long *p_bufsize );

#ifdef __cplusplus
}
#endif

#endif

