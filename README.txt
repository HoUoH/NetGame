﻿//////////////////
// 12.06 홀  //
///////////////////
클라이언트 살아있는 중 접속종료하면 그 캐릭터 안죽고 계속 살아있던 현상 수정

서버- 버벅거림 최소화 wait()함수 지연시간 10주고 클라 - 프레임 제한 둬도 별로 버벅이지 않음 (루프백에서만 테스트 해봄)


//////////////////
// 12.04 익진  //
///////////////////

1) getSendData부분 변수에 저장하도록 바꿈

2) R키로 부활하게 바꿈
- click_join 사용안함 ScnMgr.cpp joinClick참고

3) sendData, recvData 부분 수정 (서버, 클라 공통)
- 이제 속도 사용 안하는거 같아서 뺐음
- pragma pack(1)사용했는데 클라에서 UpdateRecvData부분 구조체 순서랑 다르게 값 받아오길래 순서 맞췄음

★ 최후의 수단으로 pragma pack 없애보는 것도 방법일듯

//////////////////
// 12.04 은상  //
///////////////////

#
1) join 충돌 처리 없에버림 (왜냐하면 이제 무적시간을 넣어서 시작해서 부딪쳐도 안죽음)

2) 무적시간 추가



//////////////////
// 12.02 은상  //
///////////////////

#
1) 캐릭터끼리 충돌을 없엤음
	- 캐릭터끼리의 충돌이 없기 때문에 처음 시작 위치 잡을 때도 캐릭터 충돌 고려하지 않는 걸로 바꿈

2) 벽 vs 캐릭터 충돌만 클라에서 해주고 나머지 충돌은 서버에서 하도록 바꿈

3) 서버에 무적 시간 넣어봤는데 무적 시간을 3초로 줘도 30초 후에 죽거나 하는 버그가 있어서
일단 주석 처리만 해놓음



//////////////////
// 11.30 홀  //
///////////////////

client 
- 충돌처리 최소화
- Wait*() - 구조가 헷갈려서 일단 30햇음
server
- 공끼리 충돌하는거 처음부터 있었던 몇개 붙어서 가만히 있는거는 못고치고 아예 주석 처리 해둠
- Waitformultiobject() - 30으로 해둠 singleobject는 INFINITE (30으로해서 데이터 30milsecond안에 안오면 시작하고 바로 singleobject는 set하기때문)
- Playernum 보다 많은 사람 들어오면 서버 터지던거 막음 (클라에서 접속 시도하면 클라 꺼짐)

//////////////////
// 11.29 홀  //
///////////////////

client
//if(i!=id)  - 서버 데이터 클라가 받아서 자기자신도 업데이트하게함
g_ScnMgr->UpdateRecvData(recvedData[i].isVisible, recvedData[i].posX, recvedData[i].posY,i);

objectcollision		if(i!=PlAYER_NUM)  -> if (i < PlAYER_NUM)   오타 수정

collisionreaction       - 충돌은 이미 체크 됫으니 걍 바로 캐릭터 사망시킴
if (oA_Kind == KIND_HERO)
{
		oA->SetIsVisible(FALSE);
}
else if (oB_Kind == KIND_HERO) {
		oB->SetIsVisible(FALSE);
}
//////////////////
// 11.29 은상  //
///////////////////

#
1) objs[i]->GetVelocity(&(toSendData_InScnMgr[i].velX), &(toSendData_InScnMgr[i].velX)) 
이런식으로 함수 보내지말자 이거 되게 별로인거 같아
무조건 함수안에 변수 생성해서 거기에 값 넘긴다음에 그 다음에 넘기자 이번에 캐릭터 안생기게 한 애 생기는 오류도 여기에서 일어나더라

2) SendDate가 캐릭터 수만큼만 돌길래 MAX Objects 만큼 돌게 바꿈

3) 이제 남은 해결 사항이 가끔 서로 충돌한 다음에 가만히 있다가 사라져 버리는 공을 어떻게 해결 할 것인가?
그 다음으로 이제 캐릭터와 공 부딪쳤을 때 캐릭터 죽게만 하면 끝날듯


//////////////////
// 11.28 은상  //
///////////////////

#
1) 프레임 일단 랜더 씬 부분에서 무조건 30프레임 이하로 출력되게 해버림 (눈에 띄게 버벅거리는거 같음 60프레임하고 차이 많이 남)

2) 동기화 부분 Client 메인 cpp Idle 부분에 넣는 걸로 바꿈 랜더 씬 함수에서 호출되게 하고 싶은데(왜냐하면 그래야 1번 받아올때마다 한번 업데이트가 이상적으로 
될거 같았음) 그러면 버벅거려가지고 보류

3) 캐릭터 겹쳐서 생성되는 현상 없엠 -> 생성 되는거 클라에서 생성되게 해놓음



//////////////////
// 11.28 익진  //
///////////////////

# 공통
!!!! 문제점 발견 !!!!
1) BALL NUMBER 0으로 해두니, 캐릭터 끼리의 움직임은 정상적으로 통신이 됨

BALL_NUM을 1로 하는 순간, 캐릭터의 움직임이 이상하게 처리됨 (좌우상하로 움직이는데 상하로만 움직인다던가)

2) 공끼리만 CollisionReaction일어나게 해야하나?? 캐릭터끼리 하니까 끼임현상때문에 못움직임 --> 일단 해놨음

# 서버
1) 이벤트로 순서제어 해서 좀더 부드럽게 움직이도록 했음
--> 뚝뚝 끊기는 부분은 은상이가 프레임 맞춰서 send, recv 일정 주기 싱크 맞춰주기로 했음

* 고칠점 *
1) ScnMgrThread에서 해당 스레드 번호에 플레이어가 들어가면 그때 오브젝트 생성
지금상태는 0~3 오브젝트 미리 만들어놓기임

2) 그리고 5번째 플레이어 접속시 서버 터져버림

* 생각해볼점 *
1) 만약 클라이언트가 X로 창을 닫고 나가버리면??

# 클라이언트
- 벽 충돌만 되게 하고 객체 끼리의 충돌은 처리 안함 (공끼리 충돌은 됨)


//////////////////
// 11.21 홀  //
///////////////////
------------------------------------------------------
Server, Client 공동
Client의 recvData구조체가 Server의 sendData구조체
         sendData구조체가 Server의 recvData구조체
   
send recv 할때 specialkey  u_char형 -> int형
------------------------------------------------------
Server
Server 데이터
고정 threadnum int크기로 하나 보냄 // 자기 번호 알려줌
고정 toSendData구조체 MAX_OBJECTS 만큼 보냄
고정 recvedData구조체 하나 받음 // 클라이언트의 이동연산된것
------------------------------------------------------
Client
 drawEvent 추가  --DrawMain스레드가 다 불리기전에 objs의 데이터에 접근하게 된다.
 g_ScnMgr이 할당되지 않아서 오류 발생하는 문제있어서 
 할당될때까지 drawEvent로 Clientmain 멈춰둠
 Client 데이터
고정 id int크기로 하나 받음 // 자기 번호 받음 setmyid함수로 전달
고정 recvdata MAX_OBJECTS 만큼 받음 //모든 오브젝트 위치 클라이언트마다 연동
고정 send구조체 하나 보냄 // 클라이언트의 이동연산된것
------------------------------------------------------
기타사항,,,
클라이언트 여러개 키려면 x64폴더->Debug->exe파일들어가야함 
dll, Texture폴더, 그리고 shadow.png(교수님 api특인듯...) 넣어둬야한다... shadow.png는 exe옆에다.

버그
클라이언트에서 키입력이안됨
   - 클라이언트가 recv하고 send하기 전에 RenderScene()이 불려야 하나???
   - RenderScene() 부르려고 하면 프로그램이 죽어버림
   - 해결방법을 찾자
클라이언트 4개정도 켜고 보니 프레임이 많이 떨어짐, 그래도 일단 키입력 문제부터 해결해야 할듯
///////////////////
// 11.20 익진  //
///////////////////

#########
# 변경사항 #
#########
(0) ServerWinAPI.h
- sockAttr구조체 추가됨

(1) ServerMainThread()
- SendRecvThread()에 sockAttr구조체 형식의 sockInfo 전달

(2) SendRecvThread()
- send(), recvn()부분 고정길이로 보내고 받게 함
- WaitForSingleObject(hScnMgrEvent, 10)으로 설정함, 이 부분 주석 참고좀

(3) ServerWinAPI.cpp
- ScnMgrThread() 확인해보기 [[[홀이]]]

(4) ScnMgrThread.h
- ReturnSendData() 추가
- SetRecvedData() 추가

(5)Server_Global.h
- sendData, recvData 구조체 옮김

#####################
# 은상이랑 홀이 읽어볼 사항
#####################
#1_ 스페셜 키

클라이언트에서 서버에 send()해줄 때 specialKey를 넣어줘야함.
서버에서 이것을 recv()해서 recvData에 specialKey가 초기화 되면

홀이는
그럼 각 recvData에서 specialKey 읽어와서
if (objs[i]->isVisible == false && recvData[i].specialKey == F1)
일때 objs[i]->SetisVisible(true);를 해야함


#2_ 홀이 ScnMgrThread의 WaitForMultipleObjects
이건 내가 코드에 주석처리한거 읽어보기

#############
# 고민해 볼 사항 #
#############
플레이어가 클라이언트를 꺼버리고 나가면, 
서버에서는 그 클라이언트 소켓을 closesocket()을 해주는 시점을 어떻게 캐치해낼까?

-------------------------------------------------------------------------------------------

