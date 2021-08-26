#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

int map[501][501];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m, b; cin >> n >> m >> b;
    int minN = 9999999999, maxN = 0;
    for(int i=0;i<n;i++)
        for (int j = 0; j < m; j++) {
            cin >> map[i][j];
            minN = min(minN, map[i][j]);
            maxN = max(maxN, map[i][j]);
        }

    int height, time, build, remove;
    height = -1, time = 0x7f7f7f7f;
    for (int k = minN; k <= maxN; k++) {
        build = 0;
        remove = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (map[i][j] - k < 0) {
                    build += map[i][j] - k;
                }
                else {
                    remove += map[i][j] - k;
                }
            }
        }

        if (remove + b >= build) {
            if (time >= 2 * remove + build) {
                time = 2 * remove + build;
                height = k;
            }
        }

    }

    cout << time << " " << height;

    return 0;
}