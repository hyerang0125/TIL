> Security Fact x SISS 연합스터디 4주차
>
> Wireshark 툴 사용법 - 네트워크 패킷 분석



## L1

- Q : ARP_Spoofing에 의해서 나의 아이디와 패스워드가 유출됬다!
- key is AttackerMacaddress_VictimPassword

가장 먼저 arp 필터링을 걸어보니 충돌이 일어난 부분을 확인할 수 있다. 즉, 문제에 나타나듯이 ARP_Spoofing을 통해 아이디와 패스워드를 탈취한 것 같다.

![image-20210506013432635](https://user-images.githubusercontent.com/59410565/117235868-823d1000-ae62-11eb-91c0-912ee0da6264.png)

공격자의 MAC은 **00:0c:29:f3:21:ad** 라는 것을 알 수 있다. 공격자의 MAC 주소를 필터링 해보자.

![image-20210506013741587](https://user-images.githubusercontent.com/59410565/117235915-98e36700-ae62-11eb-9e30-5b112e7b1bc2.png)

POST 메소드를 통해 로그인 한 흔적이 있다. **tcp.stream eq 8** 필터를 이용해서 [Follow HTTP Stream]으로 Stream을 보면 아래와 같다.

![image-20210506013910737](https://user-images.githubusercontent.com/59410565/117235943-a4cf2900-ae62-11eb-918e-65c462b67814.png)

Facebook에 email은 **HI_GAL@gmail.com**으로 password는 **YONG_GAL**로 로그인 하는 것을 ARS_Spoofing 공격을 통해 유출된 것이다.

- A : 00:0c:29:f3:21:ad_YONG_GAL



## L2

- Q : 남자들이 뼛속까지 좋아하는 여자는 누구? DNA 연ㄱ누 결과가 발표되었다. 바코드를 찾아라!

파일 내의 모든 object들을 추출하는데, 이미지(바코드)를 찾아야하기 때문에 "HTML"을 선택하였다.

![image-20210506014653000](https://user-images.githubusercontent.com/59410565/117235965-b284ae80-ae62-11eb-947e-3fe59df7c6ba.png)

엄청난 파일 더미 속에서 DNA_MAP이라는 바코드 이미지를 발견하였다.

![image-20210506015526744](https://user-images.githubusercontent.com/59410565/117235984-bd3f4380-ae62-11eb-8999-eb1e189cce53.png)

DNA_MAP.jpg는 아래 이미지이다.

![DNA_Map](https://user-images.githubusercontent.com/59410565/117236009-ca5c3280-ae62-11eb-842a-29af2b39463f.jpg)

바코드 스캔 사이트를 이용하여 비밀번호를 알아내었다.

![image-20210506015922907](https://user-images.githubusercontent.com/59410565/117236032-d9db7b80-ae62-11eb-88e3-3ccd7325f4a8.png)

- A : IU GOOD



## L3

- Q : 화창한 봄만 G마켓에 코드가 삽입됐다.
- hint : VB Script, URI contains/gmk/

의심가는 파일을 발견했다.

![image-20210506020347536](https://user-images.githubusercontent.com/59410565/117236051-e52ea700-ae62-11eb-88b0-29fc5ae46a3f.png)

코드를 살펴보니 가장 위에 있는 "getflashPlayerHTML"이라는 함수의 구성 중

![image-20210506020527538](https://user-images.githubusercontent.com/59410565/117236061-ed86e200-ae62-11eb-8cce-15d72057c87c.png)

```javascript
sHtml = sHtml + "   <PARAM NAME+'base64 hash : #@~^dQ4AAA==-mD~kY9{/DDbUo,'~[[%fILa%lI[[F!ZI'aW%p[:,0ILa{Fp'aRvI'[%cI'[%qiL:RcI .....
```

위와 같이 구성된 코드가 가장 수상했다. 힌트에 따라 VB Script를 디코딩하려 했는데 디코딩 해주는 사이트를 찾지 못했다... 시간 있을 때 다시 해봐야지



## L4

- 우탱아, 가을인데 단풍놀이 가야지~ 어디로 갈까?

추출된 object 중에 Where_is_it.jpg 파일이 있었다. 

![Where_is_it](https://user-images.githubusercontent.com/59410565/117236087-fa0b3a80-ae62-11eb-9793-a7f0f857d740.jpg)

photoME를 활용하여 파일의 Exif 정보를 보았다.

![image-20210506023338705](https://user-images.githubusercontent.com/59410565/117236109-068f9300-ae63-11eb-884b-633840a97ea5.png)

JPG 파일의 코멘트에 인증키가 있었다.

- A : hallasan



## L5

- Q : 악성 다운로더

추출한 파일에서 exe를 검색했더니 **noexe.exe**라는 파일을 발견했다.

![image-20210506024057698](https://user-images.githubusercontent.com/59410565/117236124-1018fb00-ae63-11eb-97a1-1a6b950e576c.png)

이 파일을 pcap 파일에서 찾아 follow-TCP로 확인해보았다.

![image-20210506024043197](https://user-images.githubusercontent.com/59410565/117236133-18713600-ae63-11eb-9386-815ac5cec7ac.png)

답을 찾았다.

![image-20210506024235848](https://user-images.githubusercontent.com/59410565/117236144-2030da80-ae63-11eb-9ada-15ae22102b35.png)

- A : HTP_Forever

