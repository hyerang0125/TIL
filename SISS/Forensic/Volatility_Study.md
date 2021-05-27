# 메모리 덤프 파일 분석 도구 조사

**[참고 자료]**

- https://m.blog.naver.com/PostView.naver?blogId=chogar&logNo=80211957733&proxyReferer=https:%2F%2Fwww.google.com%2F
- https://www.ahnlab.com/kr/site/securityinfo/secunews/secuNewsView.do?seq=22109

- https://i-am-seongni.tistory.com/52
- https://wave1994.tistory.com/31

## 1. Volatility 설치하기

- 오픈 소스 기반으로 CLI 인터페이스를 제공하는 메모리 분석 도구이다. 비슷한 기능을 제공하는 도구로는 구글에서 제작한 ReKall, 멘디언트에서 제작한 Redline 등이 있다.

  ```
  **Redline**
  - 파이어파이(FireEye) 회사의 Mandiant에서 제작된 공개 도구이다. 
  - 시스템 물리적 메모리에서 동작하고 있는 프로세스 정보, 파일 시스템 메타데이터, 레지스터리, 이벤트, 네트워크, 서비스, 웹 히스토리 정보 등을 확인할 수 있다.
  - GUI 환경으로 제작되었으며, 사용자들이 매우 편리하고 쉽게 사용할 수 있다.
  - 다른 공개용 메모리 분석 소프트웨어에 비해 비교적 빠르게 메모리 분석을 진행 할 수 있다. (그러나 테스트 과정에서는 일부 악성코드로 판단되는 항목을 표기하지 못하는 것 발견)
  - 메모리 덤프 파일에 존재하는 여러 데이터를 조합하여 시간 순서로 정렬한 타임라인 기능으로 인해 악성코드로 의심되는 항목이 최초 컴퓨터 시스템에 실행되었던 시간을 역으로 추적 할 수 있게 해준다.
  ```

- 볼라틸리티는 파이썬을 기반으로 한다. 이때 파이썬 버전은 2.x 버전만 가능하다.

- [Volatility 설치 링크](https://www.volatilityfoundation.org/releases)로 이동하여 2.6 버전을 다운 받아 주었다.
- 나중에 프로그램을 실행할 때, 분석한 파일을 해당 경로에 넣어줘야 한다. (압축파일 풀어둔 곳 기억하고 있기)

> cd [volatility exe파일과 분석 파일이 함께 있는 폴더 경로]
>
> -> 해당 폴더 진입 후 명령어 사용하기
>
> vol.py -f <image path> 명령어

- Redline과 다르게 종료되었던 프로세스 정보들도 나오기 때문에 전날 프로세스들도 확인 가능하다.
- 콘솔 환경이 아니고 비주얼한 결과를 보고 싶다면 dot 파일 형식으로 변환하여 그래픽 도구로 볼 수 있다. (부모 자식 프로세스들의 연관성 확인)
- 침투(pentest)를 목적으로 개발된 우분투 커널 기반 운영체제인 백트랙 및 칼리 리눅스에도 포함되어 있다.



## 2. Volatility 사용법(명령어)

- **이미지 프로파일 확인 명령어** 

  - imageinfo

  > 메모리 덤프 파일의 프로파일 정보를 출력한다.

- **프로파일 지정하여 메모리 덤프 분석**

  - --info

  > 플러그인 정보 확인이 가능하다.

  ![image](https://user-images.githubusercontent.com/59410565/119803578-8bc60f00-bf1a-11eb-8255-ab265045ca90.png)

  - pstree

    > 프로세스 트리 출력

  - pslist

    > 프로세스 리스트 출력, 리스트 워킹(가상 주소)

  - psscan

    > 프로세스 리스트 출력, 패턴 매칭(물리적 주소)

  - psxview

    > 프로세스 은닉을 탐지하기 위해 다양한 방식으로 프로세스 등을 조회

  - netscan

    > tcp, udp, raw 소켓 등을 출력한다. (윈도우 서버 2008버전 이상에서 사용 가능)
    >
    > 즉, 네트워크 연결 등 확인

  - iehistory(인터넷 히스토리)

  - procdump

    > 프로세스 실행파일 추출

  - dumpfiles -Q 메모리주소 -D ./ 

    > 메모리 상에 있는 파일 복구

  - memdump

    > 윈도우 가상 메모리를 파일로 추출한다. -p옵션을 사용하면 특정 프로세스만 대상으로 가상 메모리 주소를 추출한다. 
    >
    > > 특정 프로세스만 추출하여 strings.exe 도구로 활용 가능
    > >
    > > strings -o "dump file path" > 파일명.txt

  - mftparser

    > 메모리에 로드 된 디코딩 된 자료들 분석. pc 부팅 시 pc가 on/off 되는데 변경되는 데이터들의 기록이 남는다.

  - filescan

    > 메모리상에 로드 된 모든 파일. grep, findstr 사용 가능

  - hivelist

    > 메모리상의 파일 오브젝트 전체검색(패턴 매칭)

  - cmdscan

    > cmd에서 실행한 명령어 확인

  - cmdline

    > cmd에서 실행한 명령어 이력 확인

  - malfind

    > 시그니처 패턴 정보

이 외에도 많은 명령어들이 존재한다.

