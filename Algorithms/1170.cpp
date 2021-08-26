#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

bool btn[10];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    memset(btn, true, sizeof(btn));
    
    bool flag;
    string num, strnum;
    int n, m, temp, cnt; cin >> n >> m;
    for (int i = 0; i < m; i++) {
        cin >> temp;
        btn[temp] = false;
    }

    num = to_string(n);
    cnt = abs(n - 100);

    for (int i = 0; i <= 1000000; i++) {
        strnum = to_string(i);
        flag = true;
        for (int j = 0; j < strnum.length(); j++) {
            if (!btn[strnum[j] - '0']) {
                flag = false;
                break;
            }
        }
        if (flag) {
            temp = abs(n - i) + strnum.length();
            cnt = min(cnt, temp);
        }
    }

    cout << cnt;

    return 0;
}