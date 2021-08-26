#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

bool visited[1001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, num, strike, ball, cnt = 0, s_cnt, b_cnt;
    string temp, num_str;
    memset(visited, true, sizeof(visited));
    for (int i = 123; i < 1000; i++) {
        temp = to_string(i);
        if (temp[0] == temp[1] || temp[1] == temp[2] || temp[0] == temp[2])
            visited[i] = false;
        if (temp[0] == '0' || temp[1] == '0' || temp[2] == '0')
            visited[i] = false;
    }

    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> num >> strike >> ball;
        num_str = to_string(num);
        for (int j = 123; j < 1000; j++) {
            s_cnt = b_cnt = 0;
            if (visited[j]) {
                temp = to_string(j);

                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        if (x == y && num_str[x] == temp[y]) {
                            s_cnt++; continue;
                        }
                        if (x != y && num_str[x] == temp[y]) {
                            b_cnt++; continue;
                        }
                    }
                }

                if (strike != s_cnt || ball != b_cnt)
                    visited[j] = false;
            }
        }
    }

    for (int i = 123; i < 1000; i++)
        if (visited[i]) cnt++;

    cout << cnt;

	return 0;
}