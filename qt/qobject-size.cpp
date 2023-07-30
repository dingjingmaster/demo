/*************************************************************************
> FileName: qobject-size.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 31 Jul 2023 04:37:52 AM CST
 ************************************************************************/
#include <QString>
#include <QStringList>

#include <stdio.h>


int main (int argc, char* argv[])
{
    QStringList ls;

    printf ("sizeof\n");
    printf (" int           : %d\n", sizeof (int));
    printf (" float         : %d\n", sizeof (float));
    printf (" long          : %d\n", sizeof (long));
    printf (" double        : %d\n", sizeof (double));
    printf (" long long     : %d\n", sizeof (long long));

    printf (" void*         : %d\n", sizeof (void*));
    printf (" nullptr       : %d\n", sizeof (nullptr));
    printf (" QString       : %d\n", sizeof (QString));
    printf (" QStringList   : %d\n", sizeof (QStringList));
    printf ("\nvalue\n");
    printf (" NULL          : %d\n", NULL);
    printf (" nullptr       : %d\n", nullptr);



    return 0;
}
