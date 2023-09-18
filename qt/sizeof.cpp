#include <QString>
#include <cstdio>

int main ()
{
    QString ls1;
    QString ls2 = "1111";

    printf ("void*: %d\n", sizeof(void*));
    printf ("nullptr: %d\n", sizeof(nullptr));
    printf ("QString: %d -- %d -- %d\n", sizeof(QString), sizeof(ls1), sizeof(ls2));

    return 0;
}
