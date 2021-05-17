# [HackerRank_C] Tree: Height of a Binary Tree

> 2021년도 1학기 6주차 과제(자료구조 복습용 tree)



## 문제

- 링크 : https://www.hackerrank.com/challenges/tree-height-of-a-binary-tree/problem
- 문제 요약 : 트리의 height를 구하여라. (이 문제에서는 edge 수를 height라고 한다.)

![image](https://user-images.githubusercontent.com/59410565/118490415-e14f2e80-b758-11eb-9837-160d58f02eba.png)

![image](https://user-images.githubusercontent.com/59410565/118490476-ef9d4a80-b758-11eb-809c-41d6ce2c2aae.png)

## 풀이

```c
int getHeight(struct node* root) {
    // Write your code here
    if(!root)
        return -1;
    else{
        int left_h = getHeight(root->left);
        int right_h = getHeight(root->right);
        return 1 + (left_h > right_h ? left_h : right_h);
    }
}
```

1. 만약 root가 비어있다면, 노드의 끝이 전 노드라는 것이므로 방금 지나온 edge는 없다. 따라서 -1을 return 한다.
2. 재귀호출을 통해 LS와 RS의 높이를 구하고 더 큰 값에 1을 더한 뒤 return 한다.



## 결과

![image](https://user-images.githubusercontent.com/59410565/118491556-1314c500-b75a-11eb-925a-58889ed8de34.png)