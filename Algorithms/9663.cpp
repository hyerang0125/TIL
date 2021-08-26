#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int n, cnt = 0;
int list[15];

void N_Queen(int x) {
    bool flag;
    if (x == n)
        cnt++;
    else {
        for (int i = 0; i < n; i++) {
            list[x] = i;
            flag = true;
            for (int j = 0; j < x; j++) {
                if (list[x] == list[j] || abs(list[x] - list[j]) == (x - j)) {
                    flag = false;
                    break;
                }
            }
            if (flag) N_Queen(x + 1);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n;
    N_Queen(0);

    cout << cnt;
    return 0;
}