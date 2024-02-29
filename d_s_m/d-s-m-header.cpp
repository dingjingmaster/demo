/*************************************************************************
> FileName: main.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 27 Mar 2023 10:16:43 AM CST
 ************************************************************************/
#include <stdio.h>
#include <malloc.h>

#include "common.h"


#define  D_S_M_FILE   CURDIR"/file/file1.xlsx.dsm"


int main(int argc, char* argv[])
{
    const char* dFile = D_S_M_FILE;

    if (argc >= 2) {
        dFile = argv[1];
    }

    FILE  *file = NULL;

    C_D_G_FILE_HEADER   *pCHead = NULL;

    printf("\n" );
    printf("C_D_G_FILE_HEADER length = %d\n", sizeof(C_D_G_FILE_HEADER));
    printf("D_S_M file : %s\n", dFile);

    // (1) malloc pCHead
    pCHead = (C_D_G_FILE_HEADER*) malloc (sizeof(C_D_G_FILE_HEADER));
    if (pCHead == NULL) {
        printf ("malloc pC.dgHead : error\n");
        return 1;
    }
    else {
        printf ("malloc pC.dgHead : OK\n");
    }

    // (2) open DSM file
    file = fopen (dFile, "r");
    if (file == NULL) {
        printf("open D.SM file : error\n");
        free(pCHead);
        return 2;
    }
    else {
        printf("open D.SM file : OK\n");
    }

    // (3) read CdgHead
    size_t  length;

    length = fread(pCHead, 1, sizeof(C_D_G_FILE_HEADER), file);
    printf("read D.SM file length = %d\n", length);

    if (length < sizeof(C_D_G_FILE_HEADER)) {
        printf("length < sizeof(C.D.G._F.I.L.E._H.E.A.D.E.R)\n");
        free(pCHead);
        fclose(file);
        return 3;
    }

    fclose(file);

    // (4) Read
    printf("\n[ D.S.M. file Head ] \n");

    printf("\n" );
    printf("dwmyid    : %X\n", pCHead->dwmyid);
    printf("dwversion : %d\n", pCHead->dwversion);
    printf("dwentype  : %d\n", pCHead->dwentype);

    // (5) Decode
    DecAES((char*)pCHead + 12,
               sizeof(C_D_G_FILE_HEADER) - 12,
               (unsigned char *)C_D_G_FILEHEAD_PASS);

    // (6) Head
    printf("\n");
    printf("szFileId   : %s\n", pCHead->szFileId);
    printf("szFileName : %s\n", pCHead->szFileName);
    printf("szpassword : %s\n", pCHead->szpassword);

    printf("\n" );
    printf("dwenleng     : %d\n", pCHead->dwenleng);
    printf("dwfileleng   : %d\n", pCHead->dwfileleng);

    printf("\n" );
    printf("szTemplateId   : %s\n", pCHead->szTemplateId);
    printf("dwTemplateType : %d\n", pCHead->dwTemplateType);



    printf("\n");
    printf("szUserName : %s\n", pCHead->szUserName);
    printf("szUserId   : %s\n", pCHead->szUserId);
    printf("szSecInfo  : %s\n", pCHead->szSecInfo);

    printf("\n");
    printf("szFileMakeTime   : %s\n", pCHead->szFileMakeTime);
    printf("szFileCreateTime : %s\n", pCHead->szFileCreateTime);
    printf("szFileUpdateTime : %s\n", pCHead->szFileUpdateTime);
    printf("szFileAccessTime : %s\n", pCHead->szFileAccessTime);
    printf("dwValidateDate   : %d\n", pCHead->dwValidateDate);



    printf("\n");
    free(pCHead);

    return 0;
}

