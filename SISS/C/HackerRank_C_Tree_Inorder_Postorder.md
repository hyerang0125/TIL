# [HackerRank_C] Tree: Inorder Traversal

> 2021년도 1학기 5주차 과제(자료구조 복습용 tree)



## 1. InOrder

[링크] https://www.hackerrank.com/challenges/tree-inorder-traversal/problem

[설명]

![image](https://user-images.githubusercontent.com/59410565/117756060-65d11700-b258-11eb-8656-d0457dccb97c.png)

Inorder(중위 순회)는 루트의 왼쪽 서브 트리를 방문한 뒤 루트를 방문하고 오른쪽 서브 트리를 가장 나중에 방문한다. 왼쪽 서브 트리와 오른쪽 서브 트리에 대해서 재귀적으로 이 과정을 반복한다. 



### 풀이

```c
void inOrder( struct node *root) {
    if(root){
        inOrder(root->left);
        printf("%d ",root->data);
        inOrder(root->right);
    }
}
```

1. root가 존재할 때, 왼쪽 서브 트리를 방문한다.
2. 왼쪽 서브 트리의 방문이 끝나면 현재 데이터를 출력한다.
3. 오른쪽 서브 트리를 방문한다.



### 실행 결과

![image](https://user-images.githubusercontent.com/59410565/117756246-cbbd9e80-b258-11eb-9fde-0be4044de491.png)



## 2. Postorder

[링크] https://www.hackerrank.com/challenges/tree-postorder-traversal/problem

[설명]

![image](https://user-images.githubusercontent.com/59410565/117756560-52727b80-b259-11eb-8b6f-13f64cb1a5a1.png)

Postorder(후위 순회)는 먼저 루트의 왼쪽 서브 트리를 모두 방문한 뒤 오른쪽 서브 트리를 방문하고, 루트를 가장 나중에 방문한다. 왼쪽 서브 트리와 오른쪽 서브 트리에 대해서는 다시 재귀적으로 후위 순회를 수행한다.



### 풀이

```c
void postOrder( struct node *root) {
    if(root){
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ",root->data);
    }
}
```

1. root가 존재할 때 왼쪽 서브 트리를 호출한다.
2. 왼쪽 서브 트리의 방문이 끝나면 오른쪽 서브 트리를 호출한다.
3. 오른쪽 서브 트리의 방문이 끝나면 현재(root)의 데이터를 출력한다.



### 실행 결과

![image](https://user-images.githubusercontent.com/59410565/117756795-cd3b9680-b259-11eb-9a54-0747d6a5fee9.png)

