# FTK IMAGER 사용

### 1. FTK IMAGER 사용법

![image](https://user-images.githubusercontent.com/59410565/118929037-f4434800-b97e-11eb-923f-d3cca0198422.png)

FTK IMAGER를 실행시킨 화면이다. 

![image](https://user-images.githubusercontent.com/59410565/118932164-b0ead880-b982-11eb-8af2-60b326cb64b9.png)

- **Add Evidence Item** : 증거 자료를 Evidence Tree에 추가하는 과정이다. 아래 실습에서 자세히 볼 수 있다.
- **Create Disk Image** : 이미지 파일의 경로를 주고 마운트 시키면 포렌식 이미지는 읽기 전용 보기 옵션이 있는 드라이브나 물리 장치로 마운트된다. 드라이브처럼 마운트 된 이미지를 오픈하고 윈도우나 다른 애플리케이션의 내용을 검색할 수 있다. 
- **Capture Memory** : 아래 옵션을 선택해서 메모리를 캡쳐한 결과를 저장할 수 있다. 

![image](https://user-images.githubusercontent.com/59410565/118939072-49d12200-b98a-11eb-9833-538c86af1208.png)

- **Detect EFS Encryption** : 증거를 스캔하여, 암호화된 파일이 있는지 검사해준다.

------



### 1.1. FTK IMAGER로 이미지 파일 복구해보기(사용법 익히기)

![image](https://user-images.githubusercontent.com/59410565/118929208-3076a880-b97f-11eb-8fd7-c7ba492c5816.png)

File을 클릭하고, Add Evidence item을 클릭하면 소스 유형 선택창이 나온다.

**[소스의 유형]**

- Physical Drive : 물리적 드라이브 (HDD 전체를 지정)
- Logical Drive  : 논리적 드라이브 (HDD를 C, D 등으로 분할하여 사용할 경우 이 옵셕 선택)
- Image File : 이미징 파일
- Contents of a Folder : 특정 폴더의 콘텐츠

![image](https://user-images.githubusercontent.com/59410565/118929340-59973900-b97f-11eb-8382-26a85ff6dd6a.png)

소스의 유형을 선택한 뒤 다음으로 넘어가 사용할 드라이브를 선택하면 된다.

![image](https://user-images.githubusercontent.com/59410565/118929452-83506000-b97f-11eb-8a00-628a37ac9d62.png)

USB로 할 예정이기 떄문에 USB 파일을 선택한 후 완료해 주었다. 일단 비어있는 USB이기 때문에 아무 사진 파일이나 저장한 뒤 실습해보자.

![image](https://user-images.githubusercontent.com/59410565/118929783-ecd06e80-b97f-11eb-837a-ae439398656d.png)

인터넷에서 귀여운 푸 사진을 3개 다운받아 주었다. 

![image](https://user-images.githubusercontent.com/59410565/118929915-138ea500-b980-11eb-9e2d-b8e105588350.png)

아쉽지만 실습을 위해 3개 항목을 삭제해 주었다.

![image](https://user-images.githubusercontent.com/59410565/118930162-5c465e00-b980-11eb-8369-8331f457280e.png)

앞서 설정한 USB로 이동하여 Partition 1 >> [root]로 이동해보았다. 

![image](https://user-images.githubusercontent.com/59410565/118930852-38374c80-b981-11eb-8b98-028205323cd2.png)

예전에 삭제했던 파일들은 물론 방금 삭제했던 파일들이 모두 있었다. 복구할 파일들을 선택하고 우클릭을 하면 다음과 같은 화면이 나온다.

![image](https://user-images.githubusercontent.com/59410565/118931033-759bda00-b981-11eb-91b9-701e5268b4df.png)

Export Files를 클릭하면 파일이 복구될 공간을 선택할 수 있고 완료를 누르면 파일들이 복구된다!

![image](https://user-images.githubusercontent.com/59410565/118931173-a2e88800-b981-11eb-969b-c149c1030b63.png)

![image](https://user-images.githubusercontent.com/59410565/118931210-ac71f000-b981-11eb-8ff9-a5a0551e19f8.png)

------



### 1.2 디스크 이미지 분석 및 복원

실습을 위해 열어 둔 USB 드아이브에 있는 파일과 폴더들, 그리고 삭제했던 파일들을 확인할 수 있다.

![image-20210520164529312](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210520164529312.png)**

그 중 File Slack이라는 부분이 있는데, 이 공간은 물리적으로는 파일에 할당된 공간이고 논리적으로는 사용할 수 없는 공간이다. slack 공간은 정보를 은닉할 수 있고, 파일의 복구 및 삭제된 파일의 파편조사 시 유용하게 사용할 수 있으므로 포렌식 분석 시 중요하게 고려하는 사항이다.

크게 Ram Slack, Drive Slack(File Slack), File System Slack, Volume Slack으로 나뉘어지고 현재 보고있는 slack은 Drive Slack에 해당한다.

- **Drive Slack** : 클러스터의 사용으로 인해 낭비되는 공간 중 램 슬랙을 제외한 부분으로 File Slack이라고도 불린다. 파일 슬랙을 이용하면 특정 파일이 해당 저장 매체에 존재하였는지 규명할 수 있다. 존재 여부를 알아야 할 파일을 클러스터 단위로 나눈 후, 클러스터의 마지막 부분과 파일 슬랙 중 일치하는 부분이 있는지 확인하는 것이다.

![image](https://user-images.githubusercontent.com/59410565/118940356-979a5a00-b98b-11eb-908b-7572dce7d80b.png)



### 2. FTK IMAGER 내 디스크 파일 덤프 떠보기

File >> Create Disk Image

![image](https://user-images.githubusercontent.com/59410565/118932164-b0ead880-b982-11eb-8af2-60b326cb64b9.png)

Physical Drive 선택 >> 해당하는 디스크 선택 후 Finish(1번에서 했던 것 참고)

![image](https://user-images.githubusercontent.com/59410565/118932432-01facc80-b983-11eb-8725-21d1c2504d7c.png)

ADD >> Image type에서 E01 선택(E01은 Encase의 디스크 이미지 파일로, 가장 대중적으로 이용되고 있다.)

![image](https://user-images.githubusercontent.com/59410565/118932588-2a82c680-b983-11eb-974c-01b49126f6fd.png)

자신의 상황에 맞게 작성하기

![image](https://user-images.githubusercontent.com/59410565/118932792-661d9080-b983-11eb-9609-ae592da79107.png)

이미지를 저장할 폴더와 이미지 파일 이름(확장자 제외) 작성하기. 단, 이미지를 저장할 폴더는 덤프할 디스크와 같은 곳에 존재하면 안된다.

![image](https://user-images.githubusercontent.com/59410565/118933256-f4921200-b983-11eb-88f8-2936417fdee7.png)

- **Image Fragment Size**: Image 파일을 분할하여 저장하는 경우, 어느 크기로 분할할지 지정하는 것이다. (0으로 설정하면 한 파일에 디스크 이미지를 생성)

- **Compression**: 압축 정도로 9에 가까울수록 용량은 적어지지만 이미지 파일을 만드는 시간은 오래 걸린다.

Start를 누르면 이미지 덤프가 시작된다. 컴퓨터와 디스크 용량에 따라 오래걸릴 수 있다.

![image](https://user-images.githubusercontent.com/59410565/118933842-a4677f80-b984-11eb-8283-55bbb14143d3.png)

하는중(너무 오래걸려서 중간에 USB 덤프로 바꿈)

![image](https://user-images.githubusercontent.com/59410565/118937023-0f668580-b988-11eb-917d-9d762931c09d.png)

작업이 끝나면 Drive/Image Verify Result가 나오고, 이는 증거의 무결성에 사용되는 해시값들을 보여준다.

![image](https://user-images.githubusercontent.com/59410565/118938053-28bc0180-b989-11eb-9ac5-b310fb07d333.png)

저장한 폴더에 가면 지정한 대로 1500MB 씩 끊어서 이미지가 저장된 것을 확인할 수 있다.

![image](https://user-images.githubusercontent.com/59410565/118938123-44270c80-b989-11eb-935e-7760a988ea21.png)

------



### 3. 파티션 삭제하고 내용 복구해보기

컴퓨터 관리 >> 디스크 관리 선택

![image](https://user-images.githubusercontent.com/59410565/118941123-5f474b80-b98c-11eb-9794-b6fa660db550.png)

볼륨 축소 및 확장이 가능한 NTFS 파일 시스템 변경에 앞서, USB 드라이브를 포맷해 주었다.

![image](https://user-images.githubusercontent.com/59410565/118941680-ef859080-b98c-11eb-95d7-8b4e2e70f36b.png)

새 볼륨에서 마우스 오른쪽 버튼을 눌러서 **볼륨 축소(H)** 선택

![image](https://user-images.githubusercontent.com/59410565/118941778-09bf6e80-b98d-11eb-94a2-3e911f0d5577.png)

원하는 분할 공간만큼 용량을 입력하고 **[축소]** 버튼 클릭

![image](https://user-images.githubusercontent.com/59410565/118942580-d4675080-b98d-11eb-8525-9107eef1af87.png)

새로운 파티션이 생성되었다!

![image](https://user-images.githubusercontent.com/59410565/118942737-f5c83c80-b98d-11eb-9db3-b41015756115.png)

할당되지 않은 파티션에서 마우스 오른쪽 버튼 클릭 후 **[새 단순 볼륨(l)]** 선택

![image](https://user-images.githubusercontent.com/59410565/118943039-3aec6e80-b98e-11eb-9b4d-371ff2c49b1a.png)

볼륨 용량을 설정하고

![image](https://user-images.githubusercontent.com/59410565/118943398-8a329f00-b98e-11eb-877f-cf7d6de6f2ea.png)

드라이브 문자를 선택하고

![image](https://user-images.githubusercontent.com/59410565/118943814-f57c7100-b98e-11eb-85d4-5993335118ef.png)

볼륨의 파일 시스템을 선택한 뒤 생성 완료하면 된다.

![image](https://user-images.githubusercontent.com/59410565/118944531-9cf9a380-b98f-11eb-9107-857f0b2690fe.png)

잘 만들어진 것을 ㅎ확인할 수 있다. 우리는 "F :" 에 하나의 파일을 생성한 뒤 다시 해당 파치션을 삭제할 것이다. **"hiden.txt"** 파일을 생성했고 내용은 **"Look for me!"**로 하였다.

![image](https://user-images.githubusercontent.com/59410565/118945168-2ad58e80-b990-11eb-98bf-9beaaa3e1746.png)

다시 컴퓨터 관리자로 돌아가 Partition2 드라이브를 삭제하였다. (볼륨 축소 선택)

![image](https://user-images.githubusercontent.com/59410565/118945295-49d42080-b990-11eb-8a99-6922851ed88b.png)

"F :" 드라이브가 사라진 것을 확인할 수 있다.

![image](https://user-images.githubusercontent.com/59410565/118945470-712aed80-b990-11eb-8051-baddb8d601db.png)

FTK IMAGER로 USB를 열어보았다. 

![image](https://user-images.githubusercontent.com/59410565/118955082-57da6f00-b999-11eb-9110-0b4678179070.png)

**"Invalid partition table Error loading operating system Missing operat V2.0system"** 메시지를 보아 파티션 테이블이 손상된 것을 볼 수 있다.

![image](https://user-images.githubusercontent.com/59410565/118971831-d5f34180-b9aa-11eb-8afa-0874d8700019.png)

**[첫 번째 파티션 분석]**

> 00 51 12 00 07 F9 AC CD 00 14 00 00 00 EC AF 00

- 00 : 부팅 불가능
- 51 12 00 : CHS 주소지정방식의 시작 위치 정보
- 07 : NTFS 파일 시스템
- F9 AC CD : CHS 주소지정방식이 끝나는 위치 정보
- 00 14 00 00 : LBA 주소지정방식에 대한 파티션 시작 주소 -> 00 00 14 00 -> 5,120 섹터
- 00 EC AF 00 : 해당 파티션 총 섹터 개수 정보 -> 00 AF EC 00 -> 11,529,216 섹터



**1) 첫 번째 파티션 시작 위치(5,120 섹터)에 데이터가 없는 것 확인 **

![image](https://user-images.githubusercontent.com/59410565/118989458-05ab4500-b9bd-11eb-9004-e71b609c2315.png)

**-> 해당 위치부터 NTFS VBR 시그니처인 'EB 52 90 4E 54 46 53'을 검색**

![image](https://user-images.githubusercontent.com/59410565/118990868-38a20880-b9be-11eb-87e8-60cb2b1f371f.png)

해당 5,123 섹터에 NTFS VBR 정보가 있음. 

**2) 첫 번째 파티션의 VBR 영역 분석**

-> 5,123 섹터로 이동 및 VBR 확인, 분석

![image](https://user-images.githubusercontent.com/59410565/118972563-a6910480-b9ab-11eb-96a8-1eba53b529b3.png)

- 00 02 : 섹터 당 바이트(Byte per sector)  -> 02 00 -> 512 Bytes
- 08 : 클러스터 당 섹터(Sector per Cluster) -> 08 -> 8 bytes
- 00 00 : 예약된 영역(Reserved Area) -> 1 Cluster의 크기 : 섹터 당 바이트 * 클러스터 당 섹터 수 = 512 * 8 = 4,096 bytes

![image](https://user-images.githubusercontent.com/59410565/118972918-0edfe600-b9ac-11eb-9391-e523a296d867.png)

- FF EB AF 00 00 00 00 00 : 파일시스템 전체 섹터 수(Total Sector) -> AF EB FF -> 11,529,215 섹터 -> 파일시스템 전체 용량 : 전체 섹터 수 * 섹터 당 바이트 = 5,902,958,080 bytes(5902MB)

![image](https://user-images.githubusercontent.com/59410565/118973520-b9f09f80-b9ac-11eb-8a8a-2c301a0e451b.png)

- 00 00 04 00 00 00 00 00 : $MFT 클러스터 번호 -> 04 00 00 -> 262,144번 클러스터 -> MFT 섹터 번호 : VBR 시작 위치 + 클러스터 번호 * 클러스터 당 섹터 수 = 5124 + 262,144 * 8 = 2,102,276 섹터

![image](https://user-images.githubusercontent.com/59410565/118974312-a3971380-b9ad-11eb-97ad-69ee1d930fc2.png)

13,538,567 섹터에 NTFS VBR 정보가 있는 것을 확인

![image](https://user-images.githubusercontent.com/59410565/118998530-7b66df00-b9c4-11eb-9d47-e2a2dfe2b81b.png)

위 파티션이 2번째 파티션에 대한 내용이다.



**2) VBR 내용을 토대로 MBR의 2번째 파티션을 완성하기**

- 부팅정보 : 80
- CHS 시작주소 : 00 00 00 (00으로 채워도 무방)
- 파티션 타입 : 07 (NTFS)
- CHS 마지막 주소 : 00 00 00
- VBR 시작 주소(파티션 시작 주소) :  13,538,567 섹터 -> 00 CE 95 07(빅엔디안) -> 07 95 CE 00(리틀 엔디안)
- 파티션 총 섹터 수 : NFS VBR의 Total Sector + 1(VBR 백업본 섹터까지 포함되야 하므로 1을 더한다)

![image-20210520234027121](C:\Users\hyera\AppData\Roaming\Typora\typora-user-images\image-20210520234027121.png)

-> FF F7 3F 00 00 00 00 00 + 1 -> 00 00 00 00 00 3F F7 FF + 1 -> 00 3F F8 00(빅엔디안) -> 00 F8 3F 00(리틀 엔디안)



> 80 00 00 00 07 00 00 00 07 95 CE 00 00 F8 3F 00

을 MBR 두 번째 파티션에 덮어 씌우고 저장(HxD 사용)

### 변경 후 오류남..,, 고치기,,,



