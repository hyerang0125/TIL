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

IDA로 '/bin/sh' 주소는 쉽게 알 수 있었다. (**0x084a038**)

![image](https://user-images.githubusercontent.com/59410565/117292554-51cd9400-aeab-11eb-9968-df02ab65988e.png)

system의 주소 또한 IDA를 통해 알 수 있었다. (**0x0804a01c**)

