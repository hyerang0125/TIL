#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int dp[1000001];
int visited[1000001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    memset(dp, 0, sizeof(dp));
    memset(visited, 0, sizeof(visited));

    int n; cin >> n;
    
    dp[1] = 0;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + 1;
        visited[i] = i - 1;

        if (i % 2 == 0 && dp[i] > dp[i / 2] + 1) {
            dp[i] = dp[i / 2] + 1;
            visited[i] = i / 2;
        }

        if (i % 3 == 0 && dp[i] > dp[i / 3] + 1) {
            dp[i] = dp[i / 3] + 1;
            visited[i] = i / 3;
        }
    }

    printf("%d\n", dp[n]);
    while (n > 0) {
        printf("%d ", n);
        n = visited[n];
    }

    return 0;
}