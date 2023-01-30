# TPS_Shooter

## Unreal PvE Multiplay 3인칭 Shooter Game

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




#### Enemy

Behavior Tree로 AI 구현

Head Bone 피격 시, 증폭된 Damage를 받음

공격 모션 재생 시 쥐고 있는 Weapon에 캐릭터가 피격 판정을 받음



#### Session

Advanced Session Plugin으로 같은 네트워크 안에서 게임 참여

Host가 방을 생성 후, 다른 클라이언트가 방을 검색하여 참여하는 방식

방에 참가한 참가자들은 Lobby로 이동 후 게임 시작시 Game Play Level로 변화됨



#### 기타

폭발물 사격 시 폭발하며, 이펙트 재생

적에겐 200, 캐릭터에겐 20의 피해를 입힘
