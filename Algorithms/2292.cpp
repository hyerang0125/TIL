#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h> 

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int n, pos; cin >> n;
	
	pos = 1;
	for (int sum = 1; sum < n; pos++) {
		sum += 6 * pos;
	}

	printf("%d", pos);

	return 0;
}