# Sorting Algorithm

- 정렬(sort)은 주어진 레코드를 원하는 키 필드 값의 순서대로 나열하는 작업을 말한다.
- 정렬은 레코드의 리스트를 주메모리에서 처리하는 내부 정렬(internal sort)과 하드 디스크와 같은 별도의 보조 기억장치를 사용하여 처리하는 외부 정렬(external sort)로 구분한다.
- 대표적인 내부 정렬 알고리즘의 종류를 알아보자.



## 1. 선택 정렬(Selection Sort)

- 알고리즘

```c
for (n개의 원소 각각에 대해 반복){
	기준 원소를 포함하여 최소/최대값을 탐색한다.
	기준 원소와 최소/최대값 원소를 교환한다.
	정렬된 원소를 제외한 나머지 원소에 대해 위 과정을 반복한다.
}
```

- Time Complexity : n개의 원소를 n번 비교하기 때문
  $$
  O(n^2)
  $$
  
- Space Complexity : 단 하나의 배열에서만 정렬을 진행하기 때문
  $$
  O(n)
  $$
  
- 예시

```c
void selectSort(int list[], int n){
	int i, j, min, temp;
	for(i=0; i<n-1; i++){
		min = i;
		for(j=i+1; j<n; j++){
			if(list[j] < list[min])
				min = j;
		}
		temp = list[i];
		list[i] = list[min];
		list[min] = temp;
	}
}
```



## 2. 버블 정렬(Bubble Sort)

- 알고리즘

  ```
  1. 가장 왼쪽의 인접한 두 수를 비교하여 왼쪽 수가 더 크면 오른쪽 수와 위치를 교환한다.
  2. 오른쪽으로 1칸을 이동하면서 같은 방식으로 두 수의 크기를 비교하여 교환하고 가장 오른쪽에 도달할 때까지 이 과정을 반복한다. (최대값이 오른쪽 끝에 도착)
  3. 오른쪽 끝에 최대값이 도달하였음으로 이 원소를 제외하고 나머지 원소에 대해서 비교 대상 원소가 1개가 남을 때까지 1번 과정부터 반복한다.
  ```

- Time Complexity : n개의 원소를 n번 비교하기 때문
  $$
  O(n^2)
  $$
  
- 중복해서 비교가 일어나기 때문에 입력의 크기(n)가 클수록 성능이 떨어진다.

- 예시

  ```c
  void bubbleSort(int list[], int n){
      int i, j, flag = 1;
      for(i=n-1; flag>0; i--){
          flag = 0;
          for(j=0; j<i; j++){
              if(list[j] > list[j+1]){
                  temp = list[j];
                  list[j] = list[j+1];
                  list[j+1] = temp;
                  flag = 1; //swap
              }
          }
      }
  }
  ```

  

## 3. 삽입 정렬(Insertion Sort)

- 알고리즘

  ```
  1. 리스트의 인덱스 0번 위치의 항목 한 개는 이미 정렬이 완료된 리스트로 취급한다.
  2. 정렬 리스트의 오른쪽에 있는 정렬되지 않은 원소(1번 위치의 원소)는 자신의 앞에 있는 원소와 크기를 비교하여 앞쪽 원소가 더 크면 자리를 바꾼다.
  3. 동일하게 2번 위치에 있는 원소도 왼쪽으로 전진하면서 자기 자리를 찾을 때까지 왼쪽으로 전진한다.
  4. 이 과정을 더 이상 비교 대상 원소가 없을 때까지 반복한다.
  ```

- Time Complexity 

  : 최상의 경우, 이미 정렬이 완료 되어 있을 때
  $$
  O(n)
  $$
  

  : 최악의 경우, 원소들이 역순으로 되어 있을 때
  $$
  O(n^2)
  $$
  
- 예시

  ```c
  void insertionSort(int list[], int n){
      int i, j, next;
      for(i=1; i<n; i++){
          next = list[i];
          for(j=i-1; j>=0 && next<list[j]; j--)
              list[j+1] = list[j];
          list[j+1] = next;
      }
  }
  ```



## 4. 쉘 정렬(Shell Sort)

- 삽입 정렬의 원소들이 삽입될 때 이웃한 위치로만 이동하는 단점을 보안한 알고리즘이다.

- 삽입 정렬과 다르게 쉘 정렬은 전체의 리스트를 한 번에 정렬하지 않는다.

- 알고리즘

  ```
  1. 정렬해야 할 리스트의 각 k(gap)번째 요소를 추출해서 부분 리스트를 만든다.
  -> gap 초기값 : 정렬할 수 / 2, 생성된 부분 리스트의 개수는 gap과 동일
  2. 반복문 실행 마다 k를 절반으로 줄인다. 즉, 반복문이 실행될 때마다 하나의 부분 리스트에 속한 값들의 개수는 증가한다.
  -> gap은 홀수가 좋음, 짝수일 경우 1을 더해 홀수로 만든다.
  3. k가 1이 될 때까지 반복한다.
  ```

- Time Complexticy : 평균적으로 소요되는 시간이고, 최악의 경우는 삽입 정렬과 동일하다.

$$
O (n^{3/2})
$$

- 장점

1. 연속적이지 않은 부분 리스트에서 자료의 교환이 일어나면 더 큰 거리를 이동한다. (교환되는 원소들이 삽입 정렬보다 최종 위치에 가까워질 가능성이 높다.)
2. 부분 리스트는 어느 정도 정렬이 된 상태이기 때문에 부분 리스트이 개수가 1이 되면 쉘 정렬은 기본적으로 삽입 정렬보다 더욱 빠르게 수행된다.
3. 알고리즘이 간단하다.

- 예시

  ```c
  void shellSort(int list[], int n){
      int i, j, gap, temp;
      for(gap = n/2; gap>0; gap/=2){
          if(gap % 2 == 0) gap++;
          for(i = gap; i<n; i++){
              temp = list[i];
              for(j=i; j>=gap; j-=gap){
                  if(temp < list[j-gap])
                      list[j] = list[j-gap];
                  else break;
              }
              list[j] = temp;
          }
      }
  }
  ```



## 5. 퀵 정렬(Quick Sort)

- 알고리즘

  ```
  1. 기준 수로 가장 왼쪽 수를 선택하고 기준 수보다 값이 작은 그룹과 큰 그룹으로 나눈다.
  2. 분할이 완료되면 기준 수를 작은 그룹과 큰 그룹 사이로 옮긴다.
  3. 위의 과정을 작은 그룹과 큰 그룹 각가에 대해서 재귀적으로 적용하여 원소가 1개가 되어 더 이상 분할이 불가능해질 때까지 반복한다.
  ```

- 배열 내부에서 분할

  ```
  추가 배열 없이 원본 배열 상에서 분할이 이루어진다. i와 j를 각각 오른쪽 끝과 왼쪽 끝 위치로 초기화한다.
  1. 기준 수로 첫 번째 항목을 선택하고 기준 수보다 큰 값을 만날 때까지 i값을 1씩 증가시키고, 기준 수보다 작은 값을 만날 때까지 j값을 1씩 감소시킨다.
  2. 1번 과정에서 찾은 i와 j 위치의 원소를 교환하고 i<j 일 때까지 이 과정을 반복한다.
  3. i >= j 이면 분할이 완료된 것이므로 list[j]와 기준 수를 교환한다.
  ```

- Time Complexity : 평균적으로 pivot을 가지고 n개의 수와 비교하는 시간과 pivot하는 수
  $$
  O(nlogn)
  $$
  +) 최악의 경우(분할 결과가 편중된 경우)
  $$
  O(n^2)
  $$

- 예시

  ```c
  void quickSort(int list[], int left, int right){
      int pivot, i, j, temp;
      if(left < right){
          i = left, j = right+1;
          pivot = list[left];
          do{
              do{i++} while(list[i] < pivot && i <= right);
              do{j--} while(list[j] > pivot && j >= left);
              if(i < j){
                  temp = list[i];
                  list[i] = list[j];
                  list[j] = temp;
              }
          }while(i < j);
          temp = list[left];
          list[left] = list[j];
          list[j] = temp;
          
          quickSort(list, left, j-1);
          quickSort(list, j+1, right);
      }
  }



## 6. 합병 정렬(Merge Sort)

- 알고리즘

  ```
  [분할]
  1. 현재 배열을 반으로 쪼갠다. 배열의 시작 위치와 종료 위치를 입력받아 둘을 더한 후 2로 나워 그 위치를 기준으로 나눔.
  2. 쪼갠 배열의 크기가 0이거나 1(더이상 쪼갤 수 x)일때 까지 반복한다.
  [합병]
  1. 두 배열의 크기를 비교한다.
  2. i에는 A배열의 시작 인덱스를, j에는 B배열의 시작 주소를 저장한다.
  3. A[i]와 B[j]를 비교하고 이중에 작은 값을 새 배열에 저장한다. 만약 A[i]의 값을 저장 시 i의 값을 1 증가.
  4. i 또는 j가 각 배열의 끝에 도달할 때까지 반복한다.
  5. 남은 배열의 값을 새 배열 뒤에 저장한다.
  6. 새 배열의 값을 원래 배열에 저장한다.(복사)
  ```

- Time Complexity

  : 합병 과정은 두 배열을 정렬하기 때문에 두 배열의 크기의 합과 같다. 이때 두 배열은 전체 배열의 길이가 N이라고 할 경우 N = N1 + N2가 된다.
  $$
  O(n)
  $$
  : 분할 과정은 크기가 N인 배열을 분할하면, 한 번 분할할 때마다 1/2씩 줄어든다.
  $$
  O(log n)
  $$
   : 따라서 합병 과정과 분할 과정을 합하면 시간 복잡도가 된다.
  $$
  O(nlogn)
  $$

- Space Complexity : 기존의 배열 1개와 정렬을 위한 배열을 하나 더 생성한다.
  $$
  2n
  $$

- 예시

  ```c
  void mergeSort(int list[], int left, int right){
      int mid;
      if(right > left){
          mid = (right + legt) / 2;
          mergeSort(list, left, mid);
          mergeSort(list, mid+1, right);
          merge(list, left, mid+1, right);
      }
  }
  
  void merge(int list[], int left, int mid, int right){
      int i, left_end, list_items, temp_pos;
      int temp[100];
      //1st = [left..mid-1], 2st = [mid..right]
      left_end = mid-1;
      temp_pos = left;
      list_items = right-left+1;
      
      while((left <= left_end) && (mid <= right)){
          if(list[left] <= list[mid]){
              temp[temp_pos] = list[left];
              temp_pos++, left++;
          }
          else{
              temp[temp_pos] = list[mid];
              temp+pos++, mid++;
          }
      }
      
      //첫 번째 세그먼트에 남아 있는 항목 추가
      while(left <= left_end){
          temp[temp_pos] = list[left];
          temp_pos++, left++;
      }
      
      //두 번째 세그먼트에 남아 있는 항목 추가
      while(mid <= right){
          temp[temp_pos] = list[mid];
          temp_pos++, mid++;
      }
      
      //temp 배열을 num 배열에 복사
      for(i = 0; i<list_items; i++){
          list[right] = temp[right];
          right--;
      }
  }
  ```

  

## 7. 기수 정렬(Radix Sort)

- 알고리즘

  ```
  1. 0~9 까지의 버켓을 준비한다.
  2. 모든 데이터에 대하여 가장 낮은 자리수에 해당하는 버켓에 차례대로 데이터를 저장한다.
  3. 0부터 차례대로 버켓에서 데이터를 다시 가져온다.
  4. 가장 높은 자리수를 기준으로 하여 자리수를 높여가며 과정을 반복한다.
  ```

- Time Complexity : K자리수 이하의 n(원소)
  $$
  O(k*n)
  $$

- 기수 정렬은 각 수를 비교하지 않고 정렬할 수 있다는 특징이 있다.



## Performance Comparison

| Sorting Algorithm |       Average Time       | Worst Case Time |
| :---------------: | :----------------------: | :-------------: |
|  Selection sort   |          O(n^2)          |     O(n^2)      |
|  Insertion sort   |          O(n^2)          |     O(n^2)      |
|    Quick sort     |         O(nlogn)         |     O(n^2)      |
|     Heap sort     |         O(nlogn)         |    O(nlogn)     |
|    Merge sort     |         O(nlogn)         |    O(nlogn)     |
|    Radix sort     | O(k*n), k is # of digits |     O(k*n)      |

