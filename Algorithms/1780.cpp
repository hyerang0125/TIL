#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h> 

using namespace std;

int cnt[3];
int arr[2187][2187];

void paper(int size, int x, int y) {
	bool check = true;
	for (int i = x; i < x + size; i++) {
		for (int j = y; j < y + size; j++) {
			if (arr[i][j] != arr[x][y]) {
				check = false;
				break;
			}
		}
	}
	if (check) {
		cnt[arr[x][y] + 1]++;
		return;
	}
	for (int i = x; i < x + size; i += size / 3) {
		for (int j = y; j < y + size; j += size / 3)
			paper(size / 3, i, j);
	}

}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int n; cin >> n;
	for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++)
			cin >> arr[i][j];
	
	paper(n, 0, 0);

	for (int i = 0; i < 3; i++)
		cout << cnt[i] << "\n";

	return 0;
}