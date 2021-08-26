#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h> 

using namespace std;

int maxdp[3][2];
int mindp[3][2];

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int n, temp1, temp2, temp3; cin >> n;
	
	maxdp[0][0] = maxdp[1][0] = maxdp[2][0] = 0;
	mindp[0][0] = mindp[1][0] = mindp[2][0] = 0;

	for (int i = 1; i <= n; i++) {
		cin >> temp1 >> temp2 >> temp3;
		maxdp[0][1] = max(maxdp[0][0], maxdp[1][0]) + temp1;
		maxdp[1][1] = max(maxdp[0][0], max(maxdp[1][0], maxdp[2][0])) + temp2;
		maxdp[2][1] = max(maxdp[1][0], maxdp[2][0]) + temp3;

		maxdp[0][0] = maxdp[0][1];
		maxdp[1][0] = maxdp[1][1];
		maxdp[2][0] = maxdp[2][1];

		mindp[0][1] = min(mindp[0][0], mindp[1][0]) + temp1;
		mindp[1][1] = min(mindp[0][0], min(mindp[1][0], mindp[2][0])) + temp2;
		mindp[2][1] = min(mindp[1][0], mindp[2][0]) + temp3;

		mindp[0][0] = mindp[0][1];
		mindp[1][0] = mindp[1][1];
		mindp[2][0] = mindp[2][1];

	}

	cout << max(maxdp[0][1], max(maxdp[1][1], maxdp[2][1])) << " " << min(mindp[0][1], min(mindp[1][1], mindp[2][1]));

	return 0;
}