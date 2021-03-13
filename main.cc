#include <iostream>

int vi[29][23][17];

int main()
{
    memset(vi, 0xff, sizeof(vi));
    for (int i = 0; i < 11339; ++i)
    {
        int x = i % 29;
        int y = i % 23;
        int z = i % 17;
        if (vi[x][y][z] != -1)
        {
            printf("%d %d\n", i, vi[x][y][z]);
        }
        else
        {
            vi[x][y][z] = i;
        }
    }

    return 0;
}