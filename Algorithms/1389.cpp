#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h> 

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int n, m; cin >> n >> m;
	vector<vector<int>> list(n + 1);
	vector<int> baicon_num(n + 1, 0);

	int temp1, temp2;
	for (int i = 0; i < m; i++) {
		cin >> temp1 >> temp2;
		list[temp1].push_back(temp2);
		list[temp2].push_back(temp1);
	}

	for (int i = 1; i <= n; i++) {
		queue<pair<int, int>> q;
		vector<bool> check(n + 1, false);
		q.push({ i,0 });
		check[i] = true;
		while (!q.empty()) {
			int p1, num;
			p1 = q.front().first;
			num = q.front().second;
			q.pop();

			for (int j = 0; j < list[p1].size(); j++) {
				int next = list[p1][j];
				if (check[next]) continue;

				check[next] = true;
				baicon_num[j] = num + 1;
				q.push({ next,num + 1 });
			}
		}
	}

	int temp = 1;
	for (int i = 1; i <= n; i++)
		if (baicon_num[temp] > baicon_num[i])
			temp = i;

	cout << temp;

	return 0;
}