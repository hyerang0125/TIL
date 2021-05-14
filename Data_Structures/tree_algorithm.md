# Tree Algorithm

- 트리(tree)는 루트(root)와 루트의 서브 트리(sub-tree)로 구성된 계층형(hierarchical) 자료구조이다.
- 최소 하나 이상의 노드가 있어야 한다.
- 서브 트리는 부모 노드를 제거한 후 남아있는 부분 트리를 말한다.
- 트리에서 노드의 자식 개수에 제한이 없으나 이진 트리(binary tree)의 각 노드는 최대 2 개까지만 자식 노드를 가질 수 있다.
- 트리의 종류에는 루트가 있는 트리(rooted tree)와 루트가 없는 트리(non-rooted tree)가 있다.



## 0. Terminologies(트리 용어)

- degree of node(노드의 차수) : 특정 노드에 연결된 서브 트리의 수
- degree of tree(트리의 차수) : 모든 노드의 차수 중 최대값
- leaf / terminal node(단말 노드) : 자식 노드가 없는 노드(차수가 0인 노드)
- level(레벨) : 루트(레벨 1)에서 단말 노드 방향으로 단계별로 1씩 증가
- height / depth of tree(깊이) : 해당 노드에서 루트에 이르는 가장 긴 경로에서 간선의 수
- parent & child node(부모 & 자식 노드) : 레벨 차이가 1인 노드
- sibling node(형제 노드) : 부모가 동일한 자식 노드
- ancestor node(조상 노드) : 특정 노드에서 루트까지 경로에 존재하는 모든 노드
- descendent node(자손 노드) : 특정 노드의 서브 트리에 존재하는 모든 노드
- internal node(내부 노드) : 자식이 최소한 1개 이상인 노드 (= inner node)
- external node(외부 노드) : 자식이 없는 단말 노드 (= leaf node)



## 1. Representation of binary tree(이진 트리의 표현)

- 이진 트리의 배열 표현

이진 트리의 루트에서 단말 노드로 내려가면서 왼쪽에서 오른쪽 노드 방향으로 배열의 1번부터 순서대로 저장한다. 배열의  인덱스 0번은 사용하지 않는데 그 이유는 인덱스 계산을 통해 자식 노드와 부모 노드 간의 관계를 표현하기 위함이다.

**[최상]**: 포화 이진 트리를 배열에 저장할 경우 / 이진 트리의 노드가 A[i]에 저장되어 있다면 이 노드의 부모 노드는 A[i/2]에 저장되어 있다.

**[최악]**: 한쪽 방향으로 치우친 편중 트리(skewed tree). 많은 공간이 비어 있고 공간 효율이 좋지 않다.

![image](https://user-images.githubusercontent.com/59410565/118261693-e14bf600-b4ee-11eb-8f40-fa21c914728a.png)



- 이진 트리의 연결 리스트 표현

이진 트리를 표현하면 트리의 형태에 상관 없이 저장 효율을 일정하게 유지할 수 있다. 또한 링크 포인터의 변경만으로 노드의 추가와 삭제가 용이한 장점이 있다.

```c
//이진 트리 노드의 선언

typedef struct node * node_ptr;
struct node{
	int data;
	node_ptr left;
	node_ptr right;
};
node_ptr tree;
```

이진 트리에 대한 탐색은 루트 포인터(tree)를 통하여 시작된다. 

![image](https://user-images.githubusercontent.com/59410565/118261895-1fe1b080-b4ef-11eb-88ab-1394576fc908.png)

이진 트리 노드의 구조는 다음과 같다.

![image](https://user-images.githubusercontent.com/59410565/118261728-f2950280-b4ee-11eb-8d93-5db2136067cb.png)

- Child-Sibling Tree : 왼쪽으로 가면 자식, 오른쪽으로 가면 형제 노드가 있다. 최대 자식의 수는 2이다. 



## 2. Binary Tree(이진 트리)

- 어느 트리나 이진 트리의 구조로 변환이 가능하다.
- Binary tree vs generic tree

|             | Binary tree | Tree |
| :---------: | :---------: | :--: |
| empty tree  |   allowed   |  x   |
| child order |  sensitive  |  x   |

![image](https://user-images.githubusercontent.com/59410565/118260671-8bc31980-b4ed-11eb-8d83-971d26d142a5.png)

- 이진 트리에서 레벨 i에 존재할 수 있는 최대 노드의 수는 2^(i-1)이다. (단, i >= 1)
- 포화 이진 트리(Full binary tree)의 최대 높이는 2^h -1이다. (단, h >= 0)



## 3. Full Binary Tree(포화 이진 트리)

- 단말 노드를 제외하고 모든 노드의 차수가 2인 트리
- 단말 노드의 수(n0)는 차수가 2인 노드의 수(n2)에 1을 더한 것과 같다.

> n0 = n2 + 1

![image](https://user-images.githubusercontent.com/59410565/118261208-350a0f80-b4ee-11eb-828f-0fe9f9e71c39.png)

- Height(FBT)

$$
O(log n)
$$

- Representation

1. Array(배열)

![image](https://user-images.githubusercontent.com/59410565/118261628-c7121800-b4ee-11eb-8d99-d681703a5c9e.png)

2. 



## 4. Complete Binary Tree(완전 이진 트리)

- 루트부터 단말 노드 방향으로, 그리고 왼쪽에서 오른쪽 방향으로 빈 노드가 없이 순서대로 모든 노드가 존재하는 이진 트리

![image](https://user-images.githubusercontent.com/59410565/118261366-62ef5400-b4ee-11eb-8428-4dcf592fc9d7.png)



## 5. Tree Traversal(이진 트리의 탐색)

- Inorder(LVR) : (3 + 5) -> Infix
- Postorder(LRV) : (3 5 +) -> postfix
- Preorder(VLR) : (+ 3 5) -> prefix

![image](https://user-images.githubusercontent.com/59410565/118262218-8a92ec00-b4ef-11eb-9748-a46411030ac0.png)

cf> L = LS, R = RS, V = root

#### expr : A * B + C - D / E

![image](https://user-images.githubusercontent.com/59410565/118262371-c4fc8900-b4ef-11eb-8157-db1d162747f4.png)

```c
//postOrder
void postorder(tree_ptr ptr){
	if(ptr){
		postorder(ptr->left);
		postorder(ptr->right);
		printf("%d", ptr->data);
	}
}

//inOrder
void inorder(tree_ptr ptr){
    if(ptr){
        inorder(ptr->left);
        printf("%d", ptr->data);
        inorder(ptr->right);
    }
}

//preOrder
void preorder(tree_ptr ptr){
    if(ptr){
        printf("%d", ptr->data);
        preorder(ptr->left);
        preorder(ptr->right);
    }
}
```



- level order : 루트에서 시작해서 같은 레벨의 모든 노드를 왼쪽에서 오른쪽 방향으로 모두 방문한 뒤 그 다음 하위 헤벨로 내려가서 이 과정을 반복한다. (큐를 이용)

```
1. 루트 노드를 큐에 넣는다.
2. 큐에서 노드를 꺼내어 출력하고 그 자식 노드가 있으면 큐에 넣는다.
3. 큐에 꺼낼 노드가 남아 있을 떄까지 (2)번 과정을 반복한다.
```

![image](https://user-images.githubusercontent.com/59410565/118262965-9a5f0000-b4f0-11eb-9673-47d9cb30941b.png)

```c
// levelOrder
void level_order(tree_ptr ptr){
	tree_ptr queue[MAX_QUEUE_SIZE];
	if(!ptr) return;
	addq(front, &rear, ptr); //the root is added
	for(; ;){
		ptr = deleteq(&front, rear);
		if(ptr){
			printf("%d", ptr->data);
			if(ptr->left) addq(front, &rear, ptr->left);
			if(ptr->right) addq(front, &rear, ptr->right);
		}
		else break; //if queue is empty
	}
}
```



## 6. Evaluation Tree(평가 트리)

- 노드의 표현 : 평가 트리의 노드에 서브 트리의 계산 값을 저장하기 위한 value 필드가 추가된다. data 필드에는 피연산자 또는 연산자 정보가 저장된다. 단말 노드일 경우는 피연산자가 저장되고, 비단말 노드의 경우에는 연산자가 저장된다.

![image](https://user-images.githubusercontent.com/59410565/118263447-4274c900-b4f1-11eb-8a5f-218ddd99d37d.png)



## 7. Threaded Binary Tree(쓰레드 이진 트리)

![image](https://user-images.githubusercontent.com/59410565/118263804-b4e5a900-b4f1-11eb-8928-077b1c49ebb3.png)

- True : thread link(단말 노드만 존재, 점선)
- False : normal link

![image](https://user-images.githubusercontent.com/59410565/118263648-8536a100-b4f1-11eb-8ae6-e3ad78ceb16e.png)

- left : 이전 / right : 다음
- 예시

![image](https://user-images.githubusercontent.com/59410565/118263880-d34ba480-b4f1-11eb-97ad-88a932ef53a6.png)

-> normal link : 도착한 노드의 left는 child가 없을 때까지 이동

-> thread left가 'F'면 left link / 'T'면 값 출력 후 thread right link

![image](https://user-images.githubusercontent.com/59410565/118264471-b4014700-b4f2-11eb-9c11-54d26d264dae.png)

- Time Complexity

$$
O(n)
$$

```c
//main function
void tbt_inorder(thread_ptr tree){
	thread_ptr temp = tree;
	for(; ;){
		temp = successor(temp);
		if(temp == tree) break;
		printf("%c", temp->data);
	}
}

//find a successor node
thread_ptr successor(thread_ptr tree){
	tread_ptr temp;
	temp = tree->right;
	if(!(tree->right_thread)){
		while(!(temp->left_thread))
			temp = temp->left;
	}
	return temp;
}
```



## 8. Max & Min Heap Tree

- 최대 트리 : 각 노드의 키 값이 그 자식 노드보다 크거나 같은 트리
- 최대 힙 : 최대 트리의 조건을 만족하는 완전 이진 트리(cbt)

![image](https://user-images.githubusercontent.com/59410565/118264510-c24f6300-b4f2-11eb-9888-00ee4e4696d8.png)

```c
// add max heap
void add_heap(int item){
	int i;
	if(total_node >= MAX_SIZE - 1) exit(1); //full heap
	i = ++total_node;
	while((i!=1) && (item>heap[i/2])){
		heap[i] = heap[i/2]; //empty heap
		i /= 2; //move up to parent
	}
	heap[i] = item; //the final position for a new node
}

//delete max heap
int delete_heap(){
	int parent, child;
	int item, temp;
	
	if(total_node == 0) exit(1);
	
	item = heap[1]; //the root
	temp = heap[total_node--];
	parent = 1; //the parent
	child = 2; //the child
	
	while(child <= total_node){
		if(child < total_node && heap[child] < heap[child+1])
			child++; //change with the right
		if(temp >= heap[child]) break;
		
		heap[parent] = heap[child]; //child move up ro parent pos
		parent = child; //update parent & child node
		child *= 2;
	}
	
	heap[parent] = temp; //temp is stored at the final pos
	return item; //return the saved root
}
```



- 최소 트리 : 각 노드의 값이 그 자식 노드보다 작거나 같은 트리
- 최소 힙 : 최소 트리이면서 완전 이진 트리인 경우

![image](https://user-images.githubusercontent.com/59410565/118264698-09d5ef00-b4f3-11eb-9250-90f50c1a89d8.png)