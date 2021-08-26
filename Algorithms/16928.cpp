#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h> 

using namespace std;
int moveN[101];
int check[101];

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	memset(check, 0, sizeof(check));
	for (int i = 1; i < 101; i++) {
		moveN[i] = i;
	}

	int n, m; cin >> n >> m;
	int temp1, temp2;
	for (int i = 0; i < n + m; i++) {
		cin >> temp1 >> temp2;
		moveN[temp1] = temp2;
	}

	check[1] = 0;
	queue<int> q;
	q.push(1);
	while (!q.empty()) {
		temp1 = q.front();
		q.pop();
		for (int i = 1; i <= 6; i++) {
			temp2 = temp1 + i;
			if (temp2 > 100) continue;
			temp2 = moveN[temp2];
			if (!check[temp2]) {
				check[temp2] = check[temp1] + 1;
				q.push(temp2);
			}
		}
	}

	cout << check[100];

	return 0;
}