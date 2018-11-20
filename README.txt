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

