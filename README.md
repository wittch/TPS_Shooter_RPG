###Asset의 용량이 큰 관계로 Source만 따로 추가했습니다.

# TPS_Shooter

## Unreal PvE Multiplay 3인칭 Shooter Game

12/07 (월) ~ 01/02 (월), 총 27일

### 기능

#### Character

캐릭터 회전에 따라서 자연스러운 Animation 재생

무기 종류에 따른 Gun Grap 방식과 Aim Animation 재생(재장전 시 Weapon종류에 따라서 Clip의 위치가 변화하기 때문에 정상적으로 재생할 수 있도록 IK 변경)

캐릭터 Turn,Lookup Rate에 맞춘 Bone 회전

총기 반동 Animation

적에게 피격 시, 피격 Animation 재생과 일정 확률로 Stun 상태

적에게 사격 시, Damage에 따라서 Widget 출력

FootStep에 따라서 사운드 재생 및 재질에 따라서 사운드 변화

Widget으로 Aim을 띄우고, 무기 종류에 따라서 Aim 모양이 변경

무기 종류에 따라서 소모되는 Bullet의 종류가 다름




#### Item

Item 습득시 Curve를 통한 Item 위치 조정으로 습득

Overlap시 아이템 정보를 Widget으로 띄움

Item 습득

1. Gun -> Widget이 띄워진 상태에서 E key 상호작용 시 습득
2. Bullet -> overlap시 습득


#### Weapon

희귀도에 따라서 바닥에 떨어져 있는 무기의 및나는 색상이 달라짐

Bullet이 발사되는 궤적표시

탄퍼짐 구현

연속 발사 시 Aim의 벌어짐 구현


#### Enemy

Behavior Tree로 AI 구현

Head Bone 피격 시, 증폭된 Damage를 받음

공격 모션 재생 시 쥐고 있는 Weapon에 캐릭터가 피격 판정을 받음



#### Session

Advanced Session Plugin으로 같은 네트워크 안에서 게임 참여

Host가 방을 생성 후, 다른 클라이언트가 방을 검색하여 참여하는 방식

방에 참가한 참가자들은 Lobby로 이동 후 게임 시작시 Game Play Level로 변화됨


#### Replicate 디버깅 내용

Server의 캐릭터와 Client의 캐릭터의 Aim이 겹치는 문제가 발생하여 수정

Crouch를 Unreal에서 제공하는 기능을 사용하지 않고 구현하였더니 Client의 캐릭터가 가라앉는 문제가 발생하여 수정

각종 캐릭터와 Enmey의 위치 및 Animation이 재생되지 않는 문제 수정

Server와 Client의 Explosive Object와 Enmey의 피격 판정 문제 수정

Item 습득 시 사라지지 않고 남아있는 문제 수정

Client와 Server에서 각 캐릭터의 체력이 다른 문제 수정



#### 기타

폭발물 사격 시 폭발하며, 이펙트 재생

적에겐 200, 캐릭터에겐 20의 피해를 입힘


## 시연영상

https://youtu.be/hQ4JKXvowsw
