#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int visited[101];
vector<int>com[101];
int cnt = 0;

void search(int node) {
    for (int i = 0; i < com[node].size(); i++) {
        if (visited[com[node][i]] == 0) {
            visited[com[node][i]] = node;
            cnt++;
            search(com[node][i]);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    memset(visited, 0, sizeof(visited));
    int n, v, num1, num2;
    cin >> n;
    cin >> v;
    for (int i = 0; i < v; i++) {
        cin >> num1 >> num2;
        com[num1].push_back(num2);
        com[num2].push_back(num1);
    }

    visited[1] = 1;
    search(1);

    cout << cnt;

    return 0;
}