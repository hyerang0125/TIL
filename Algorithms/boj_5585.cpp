#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int coin[6] = {500, 100, 50, 10, 5, 1};
    int price, i, sum;

    cin >> price;
    price = 1000 - price;

    i = sum = 0;
    while(price > 0){
        if(price >= coin[i]){
            price -= coin[i];
            sum++;
        }
        else i++;
    }

    cout << sum;

    return 0;
}