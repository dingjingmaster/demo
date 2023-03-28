/*************************************************************************
> FileName: main.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 27 Mar 2023 10:16:43 AM CST
 ************************************************************************/
#include <stdio.h>
#include <malloc.h>

#include "common.h"


#define  DSM_FILE	CURDIR"/file/file1.xlsx.dsm"


int main( )
{
	FILE  *file = NULL;

	CDG_FILE_HEADER   *pCdgHead	= NULL;

	printf( "\n" );
	printf( "CDG_FILE_HEADER length = %d\n", sizeof( CDG_FILE_HEADER ) );
	printf( "DSM file : %s\n", DSM_FILE );

	// (1) malloc pCdgHead
	pCdgHead = (CDG_FILE_HEADER *)malloc( sizeof( CDG_FILE_HEADER ) );
	if ( pCdgHead == NULL )
	{
		printf( "malloc pCdgHead : error\n" );
		return 1;
	}
	else
		printf( "malloc pCdgHead : OK\n" );

	// (2) open DSM file
	file = fopen( DSM_FILE, "r" );
	if ( file == NULL )
	{
		printf( "open DSM file : error\n" );
		free( pCdgHead );
		return 2;
	}
	else
		printf( "open DSM file : OK\n" );

	// (3) read CdgHead
	size_t  length;

	length = fread( pCdgHead, 1, sizeof( CDG_FILE_HEADER ), file );
	printf( "read DSM file length = %d\n", length );

	if ( length < sizeof( CDG_FILE_HEADER ) )
	{
		printf( "length < sizeof( CDG_FILE_HEADER )\n" );
		free( pCdgHead );
		fclose( file );
		return 3;
	}

	fclose( file );

	// (4) Read
	printf( "\n[ DSM file Head ] \n" );

	printf( "\n" );
	printf( "dwmyid    : %X\n", pCdgHead->dwmyid 	);
	printf( "dwversion : %d\n", pCdgHead->dwversion );
	printf( "dwentype  : %d\n", pCdgHead->dwentype 	);

	// (5) Decode
	DecodeAES( (char*)pCdgHead + 12,
	           sizeof(CDG_FILE_HEADER) - 12,
			   (unsigned char *)CDG_FILEHEAD_PASS );

	// (6) Head
	printf( "\n" );
	printf( "szFileId   : %s\n", pCdgHead->szFileId 	);
	printf( "szFileName : %s\n", pCdgHead->szFileName );
	printf( "szpassword : %s\n", pCdgHead->szpassword 	);

	printf( "\n" );
	printf( "dwenleng     : %d\n", pCdgHead->dwenleng 	);
	printf( "dwfileleng   : %d\n", pCdgHead->dwfileleng    );

	printf( "\n" );
	printf( "szTemplateId   : %s\n", pCdgHead->szTemplateId );
	printf( "dwTemplateType : %d\n", pCdgHead->dwTemplateType );



	printf( "\n" );
	printf( "szUserName : %s\n", pCdgHead->szUserName );
	printf( "szUserId   : %s\n", pCdgHead->szUserId   );
	printf( "szSecInfo  : %s\n", pCdgHead->szSecInfo  );

	printf( "\n" );
	printf( "szFileMakeTime   : %s\n", pCdgHead->szFileMakeTime );
	printf( "szFileCreateTime : %s\n", pCdgHead->szFileCreateTime );
	printf( "szFileUpdateTime : %s\n", pCdgHead->szFileUpdateTime );
	printf( "szFileAccessTime : %s\n", pCdgHead->szFileAccessTime );
	printf( "dwValidateDate   : %d\n", pCdgHead->dwValidateDate );



	printf( "\n" );
	free( pCdgHead );

	return 0;
}

