#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stack>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
int list[100001];
char check[100001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    memset(list, true, sizeof(list));
    int n, len = 0, maxl = 0, cnt = 0, maxday = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> list[i];
    }

    for (int i = 0; i <= n; i++) {
        if (list[i] < 0) {
            len++;
            continue;
        }
        if (len == 0) continue;
        maxl = max(maxl, len);
        for (int j = i - len - 1; j > i - len * 3 - 1; j--) {
            if (j < 0) break;
            check[j] = '*';
        }
        len = 0;
    }

    for (int i = 0; i <= n; i++) {
        if (list[i] < 0) {
            len++;
            continue;
        }
        if (len == maxl) {
            for (int j = i - len - 1; j > i - len * 4 - 1; j--) {
                if (j < 0) break;
                if (check[j] == '*') continue;
                cnt++;
            }
            maxday = max(maxday, cnt);
        }
        len = cnt = 0;
    }

    
    for (int i = 0; i < n; i++) {
        if (check[i] == '*')
            cout << check[i] << " ";
        else
            cout << "- ";
    }
    cout << "\n";
    

    for (int i = 0; i < n; i++)
        if (check[i] == '*') ++maxday;

    cout << maxday;

    return 0;
}