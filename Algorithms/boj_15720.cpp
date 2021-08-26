#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int b, c, d, sum, dis; 
    
    cin>>b>>c>>d;
    int *blist = new int[b];
    int *clist = new int[c];
    int *dlist = new int[d];

    sum = dis = 0;
    for(int i=0; i<b; i++){
        cin >> blist[i];
        sum += blist[i];
    }
    for (int i = 0; i < c; i++){
        cin >> clist[i];
        sum += clist[i];
    }
    for (int i = 0; i < d; i++){
        cin >> dlist[i];
        sum += dlist[i];
    }

    sort(blist, blist + b, greater<>());
    sort(clist, clist + c, greater<>());
    sort(dlist, dlist + d, greater<>());

    int num = min(b,min(c,d));
    for(int i=0; i<num; i++){
        dis += blist[i] * 0.9;
        dis += clist[i] * 0.9;
        dis += dlist[i] * 0.9;
    }

    for(int i=num; i<b; i++)
        dis += blist[i];
    for (int i = num; i < c; i++)
        dis += clist[i];
    for (int i = num; i < d; i++)
        dis += dlist[i];

    cout << sum << "\n" << dis;

    return 0;
}