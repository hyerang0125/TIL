# Binary Search Trees(BST)

- 힙(heap)은 최대값 또는 최소값을 탐색할 때는 O(log n)의 시간만이 걸리기 때문에 배열, 리스트 등 순차 자료구조보다 빠르다.
- 그러나 최대값/최소값이 아닌 일반 원소를 탐색할 경우 힙의 모든 노드를 탐색해야 하므로 선형 자료 구조와 마찬가지로 O(n)의 시간이 걸린다.
- **이진 탐색 트리(binary search tree)**는 이진 탐색 알고리즘을 트리에 적용한 자료구조로, **임의의 원소를 탐색할 때 거리는 평균 시간은 트리 높이에 비례한 O(log n)**이다.

```
[이진 탐색 트리]
1. 트리 내의 모든 원소는 중복 없이 유일해야 한다.
2. 루트 노드는 자신의 왼쪽 서브 트리에 존재하는 모든 노드보다 값이 커야 하고 오른쪽 서브 트리의 모든 노드보다 작아햐 한다.
3. 왼쪽과 오른쪽 서브 트리 또한 이진 탐색 트리여야 한다.
```

![image](https://user-images.githubusercontent.com/59410565/119081816-c837cd80-ba37-11eb-9221-0a72a54bf698.png)

첫 번째 트리와 두 번째 트리는 이진 탐색 트리의 조건을 만족하지만 세 번째 트리는 21이 30의 오른쪽에 있고, 15가 18의 오른쪽에 있기 때문에 이 조건을 만족하지 못한다.



### Search a key in BST

```c
//재귀적 이진 탐색 트리 : O(log n) + recursion overhead
node_ptr rBST(node_ptr tree, int item){
	if(!tree) return NULL;
	if(tree == tree->data) return tree;
	if(item < tree->data) return rBST(tree->left, item);
	return rBST(tree->right, item);
}
```

- 트리 포인터인 tree가 NULL이면 NULL이 반환되고 종료된다.
- NULL이 아닌 경우 tree->data와 item을 비교하고 성공하면 tree 포인터를 반환한다.
- item이 tree->data 보다 작으면 다시 왼쪽 서브 트리에 대해 이진 탐색을 진행한다. (재귀호출)
- item이 tree->data 보다 크면 다시 오른쪽 서브 트리에 대해 이진 탐색을 진행한다. (재귀호출)
- 단말 노드까지 내려가서도 탐색 값을 찾지 못하면 NULL이 반환되며 이것이 재귀 호출의 종결 시점이 된다.

```c
//반복문 이진 탐색 트리 : O(log n)
node_ptr iBST(node_ptr, int item){
	while(tree){
		if(item == tree->data) return tree;
		if(item < tree->data) tree = tree->left;
		else tree = tree->right;
	}
	return NULL;
}
```

- tree  포인터가 NULL이 아닌 동안 탐색이 반복된다.
- 재귀적 이진 탐색과 유사하게 탐색 키와 노드의 값을 비교하여 탐색 키를 찾을 때까지 단말 노드 방향으로 이동한다.
- 단말 노드까지 이동하여 찾지 못하면 탐색이 종료된다.



### Insertion in BST

![image](https://user-images.githubusercontent.com/59410565/119083272-9a07bd00-ba3a-11eb-9b83-a7a7ece53cfb.png)

```c
void insert_BST(tree_ptr *node, int item){
	tree_ptr ptr, temp = overlap_check(*node, item); //ladtly found node
    if(temp || !(*noded)){ //no overlap, or empty tree
        ptr = (tree_ptr)malloc(sizeof(node));
        if(IS_FULL(ptr)){
            printf("The memory is full\n"); exit(1);
        }
        ptr->data = item;
        ptr->left = ptr->right = NULL;
        if(*node){
            if(item < temp->data) temp->left = ptr;
            else temp->right = ptr;
        }
        else *node = ptr; //empty tree
    }
}

tree_ptr overlap_check(tree_ptr tree, int item){
    tree_ptr temp = tree;
    while(temp){
        if(key == temp->data) return NULL; //overlapped
        if(key < temp->data) temp = temp->left;
        else temp = temp->right;
    }
    return temp;
}
```

- overlap_check() 함수를 통해 트리에 중복되는 노드가 있다면 NULL을 반환한다.
- 만약 중복되는 노드가 없다면 key가 data보다 작으면 왼쪽으로 이동한다.
- key가 data보다 크면 오른쪽으로 이동한다.
- temp를 반환하고 temp의 데이터가 item보다 크면 temp의 왼쪽에 ptr을 저장한다.
- temp의 데이터가 item보다 작으면 temp의 오른쪽에 ptr을 저장한다.



### Delete from BST

> BST에서 노드의 삭제 연산은 삭젷하려는 노드의 자식 개수에 따라 알고리즘이 달라진다.

![image](https://user-images.githubusercontent.com/59410565/119084368-9ecd7080-ba3c-11eb-8ae8-dc749b5d4c06.png)

##### 1. leaf node -> NULL (단말노드)

- 그 부모 노드의 해당 링크에 NULL 값을 할당한다.

##### 2. one child (자식이 하나인 노드)

- 삭제 노드(A)의 자식 노드를 A의 부모 노드의 링크에 연결한다.

![image](https://user-images.githubusercontent.com/59410565/119084397-ad1b8c80-ba3c-11eb-8df4-8260116bd6a8.png)

##### 3. two child (자식이 둘인 노드)

- 삭제 노드(A)의 왼쪽 서브 트리에서 최대값을 갖는 노드나 오른쪽 서브 트리에서 최소값을 갖는 노드가 A를 대체할 수 있다. 트리 노드 탐색 시간을 줄이기 위해서는 트리의 높이가 낮아지는 경우를 선택하는 것이 좋다.



# Balance BST

- 이진 탐색 트리에서 탐색할 때 한쪽 방향으로만 노드가 연결되어 있는 편중 트리(skewed tree)가 최악의 경우이다. 해당 시간 복잡도는 O(n)이다.
- 이진 탐색 트리에서 노드 탐색 시간을 줄일 수 있는 방법은 트리의 높이를 가능하면 작게 하는 것이다.
- n개의 노드를 갖는 BST의 최저 높이는 O(log n)이고 이것이 최상의 경우이다. 
- 이와 같이 트리의 높이를 O(log n)로 유지하는 트리를 균형 이진 탐색 트리(balanced BST)라고 한다.
- 균형 이진 탐색 트리의 예로 AVL 트리, 2-3-4 트리, red-black 트리 등이 있다.



### AVL Tree

- 노드의 균형 인수(balanced factor, BF)는 좌우 서브 트리의 높이의 차이를 말한다.
- 즉, AVL 트리의 모든 노드의 균형 인수는 -1, 0, 1 중 하나이다.
- BF(i) = height(왼쪽 서브 트리(i)) - height(오른쪽 서브 트리(i))

![image](https://user-images.githubusercontent.com/59410565/119084793-5ebabd80-ba3d-11eb-821e-8c1ff994a688.png)

왼쪽 트리의 경우 AVL 트리는 맞지만, 오른쪽 트리의 경우 7의 균형인수가 2이므로 (왼쪽 높이(3) - 오른쪽 높이(1)) AVL 트리가 아니다.



### AVL Tree with Min Number of Nodes

- 높이가 h인 AVL 트리의 최소 노드 수를 다음과 같이 정의할 수 있다.

$$
N_{h} = N_{h-1} + N_{h-2} + 1
$$

- AVL 트리의 높이는 log n을 넘지 않으므롤 AVL 트리에서 연산 시간은 O(log n)이 된다.



### Insertion in AVL tree

#### LL(left-left) : single rotation(단일 회전)

![image](https://user-images.githubusercontent.com/59410565/119085626-0dabc900-ba3f-11eb-9fe7-f677c4d5724a.png)

- 시계 방향으로 돌린다. 
- k1이 루트가 되고, k1의 자식을 k2가 가져간다.

#### RR(right-right) : single rotation(단일 회전)

![image](https://user-images.githubusercontent.com/59410565/119085691-26b47a00-ba3f-11eb-90ce-a668479e50c1.png)

- 반시계 방향으로 돌린다.
- k2가 루트가 되고, k2의 자식을 k1이 가져간다.

#### LR(left-right) : double rotation(이중 회전)

![image](https://user-images.githubusercontent.com/59410565/119086014-c83bcb80-ba3f-11eb-949d-9033a510ad55.png)

- 위에서부터 k3-k2-k1 노드가 된다.
- k2가 새로운 노드가 된다.
- k2의 왼쪽 서브 트리를 k1이 가져가고, 오른쪽 서브 트리를 k3이 가져간다.

#### RL(right-left) : double rotation(이중 회전)

![image](https://user-images.githubusercontent.com/59410565/119086128-fde0b480-ba3f-11eb-9b4e-fe6382fbe975.png)

- 위에서부터 k1-k3-k2 노드가 된다.
- 이하 LR과 동일하다.



# B - Tree

> 참고 및 이미지 : https://hyungjoon6876.github.io/jlog/2018/07/20/btree.html

- 많은 데이터가 저장되어 있을 때 더 효율적으로 데이터를 사용하기 위해서는 디스크에서 가져오는 시간을 줄여야 한다.
- AVL 트리의 경우 자식이 최대 2개이기 때문에 n이 증가할수록, 빠르게 높이가 증가한다.
- 따라서 자식의 수가 많아지더라도 높이가 천천히 증가하는 B-Tree가 등장하게 되었다.



### Define : M-way B-Tree

- 노드내 최대 데이터 수가 2개라면 2차 B-Tree, 3개라면 3차 B-Tree라고 할 수 있다. 즉, 노드의 수가 m이면 m차 B-Tree가 된다.

**[조건]**

1. 노드의 데이터 수가 n개라면 자식 노드의 개수는 n+1개이다.
2. B-Tree는 BST에서 파생된 트리임으로 BST의 기준 또한 모두 만족해야 한다. (루트를 기준으로 왼쪽 서브 트리에 작은 값, 오른쪽 서브 트리에 큰 값)
3. 루트는 적어도 2개 이상의 자식을 가져야 한다.
4. 루트 노드를 제외한 모든 노드는 적어도 m/2개의 데이터를 갖고 있어야 한다. (예를 들어 4차 B-Tree는 노드 당 최소 2개의 데이터, 효율과 융통성을 위해 올림을 사용함.)
5. leaf 노드로 가는 경로의 길이는 모두 같아야한다. 즉, leaf 노드는 모두 같은 레벨에 존재해야 한다. 

```
//B-Tree의 조건 5번 불충족 예시	
    	*
	  / | \
	 *  *  *       <- 중간에 leaf 노드
   /  \      \
  *    *      *    <- leaf
```

- **m = 3(2-3 Tree)**

->  min children(내부 노드) = ceil(3/2)

-> Degree(internal) = 2 or 3, Degree(root) = 0(자식이 없을 수 있음), 2, 3

-> # key in a leaf node = 1, 2

- **m = 4(2-3-4 Tree)**

-> min children = ceil(4/2)

-> Degree(internal) = 2 or 3 or 4, Degree(root) = 0, 2, 3, 4

-> # key in a leaf node = 1, 2, 3

- **m = 5**

-> min children = ceil(5/2)

-> Degree(internal) = 3 or 4 or 5, Degree(root) = 0, 2, 3, 4, 5

-> # key in a node = 2, 3, 4

**즉, 차수가 m인 B-Tree는 자식이 ceil(m/2) ~ m개 사이이고, key는 ceil(m/2) - 1 ~ m-1개 사이이다.**



### Insertion in B-Tree

![image](https://user-images.githubusercontent.com/59410565/119095346-21f6c280-ba4d-11eb-8b05-3b198806a25d.png)

1. 초기 삽입시에는 root 노드를 생성한다.
2. 데이터를 탐색해 해당하는 leaf 노드에 데이터를 삽입한다.
3. leaf 노드 데이터가 가득 차 있으면 노드를 분리한다. (노드 데이터 개수 == 차수) 이때 정렬된 노드를 기준으로 중간값을 부모 노드로 가진다. (insert 7 예시 참고)
4. 분리한 서브트리가 B-Tree 조건에 맞지 않는다면 다시 부모 노드로 올라가며 merge 한다. (insert 12 예시 참고)



### Delete from B-Tree

#### leaf 노드인 경우 1

![image](https://user-images.githubusercontent.com/59410565/119095822-bbbe6f80-ba4d-11eb-9f97-4145cc8ef36b.png)

- 데이터를 삭제하여도 B-Tree 를 유지한다.

#### leaf 노드인 경우 2

![image](https://user-images.githubusercontent.com/59410565/119095931-e27ca600-ba4d-11eb-8349-1156b7b5eae3.png)

- 데이터를 삭제하면 B-Tree의 구조가 깨진다. 따라서 삭제한 노드의 부모 노드로 올라가며 데이터를 가져와  merge 한다.
- root 노드까지 올라가며 B-Tree 조건을 충족시킬 때까지 반복한다.

#### leaf 노드가 아닌 경우 1

![image](https://user-images.githubusercontent.com/59410565/119096124-253e7e00-ba4e-11eb-9935-729f73e1d24b.png)

- 먼저 노드에서 데이터를 삭제하고 왼쪽 서브트리에서 최대값을 노드에 위치시킨다.
- B-Tree의 조건이 맞을 때까지 부모 노드에서 자식 노드로 값을 가져오고 merge한다.

#### leaf 노드가 아닌 경우 2

![image](https://user-images.githubusercontent.com/59410565/119096283-5ae36700-ba4e-11eb-9b64-b794a28da910.png)