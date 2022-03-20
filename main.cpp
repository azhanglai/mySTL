#include "util.h"
#include <cstdio>

int main() {
    int num = 1;
    char str = 'a'; 
    
    laistl::pair<int, char> my_pair = laistl::make_pair(num, str);

    printf("my_pair.first = %d\n", my_pair.first);
    printf("my_pair.second = %c\n", my_pair.second);

    return 0;
}
