# [HackerRank_C] Sortinf: Find the Median

> 2021년도 1학기 7주차 과제(자료구조 복습용 sorting)



## 문제

- 링크 : https://www.hackerrank.com/challenges/find-the-median/problem
- 원소의 개수가 홀수인 배열이 주어질 때, 이 배열의 중간값을 찾아라.

![image](https://user-images.githubusercontent.com/59410565/119861375-49b8bf80-bf52-11eb-9984-18c9960497ac.png)



## 풀이

```c
int findMedian(int arr_count, int* arr) {
    int i, j, gap, temp;
    for(gap = arr_count/2; gap>0; gap/=2){
        for(i = gap; i<arr_count; i+=gap){
            temp = arr[i];
            for(j=i; j>=gap; j-=gap){
                if(temp < arr[j-gap])
                    arr[j] = arr[j-gap];
                else break;
            }
            arr[j] = temp;
        }
    }
    for(i=0; i<arr_count; i++)
        printf("%d ",arr[i]);
    
    return arr[arr_count/2];
}
```

**[쉘 정렬 사용]**

1. 정렬해야 할 리스트의 각 k(gap)번째 요소를 추출해서 부분 리스트를 만든다.
-> gap 초기값 : 정렬할 수 / 2, 생성된 부분 리스트의 개수는 gap과 동일
2. 반복문 실행 마다 k를 절반으로 줄인다. 즉, 반복문이 실행될 때마다 하나의 부분 리스트에 속한 값들의 개수는 증가한다.
-> gap은 홀수가 좋음, 짝수일 경우 1을 더해 홀수로 만든다.
3. k가 1이 될 때까지 반복한다.



## 결과

![image](https://user-images.githubusercontent.com/59410565/119861499-6d7c0580-bf52-11eb-82b0-5d573982fc7c.png)