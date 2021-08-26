#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m; cin >> n >> m;
    int temp, sum = 0;

    for(int i=0; i<n; i++){
        cin >> temp;
        sum += temp;
    }

    for(int i=0; i<m; i++){
        cin >> temp;
        sum -= temp;
    }

    cout << sum;

    return 0;
}