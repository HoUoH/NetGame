###################
구현한 것은 #대신 O표시 해주기
###################

# 사이즈와 무게는 비례하기로 하자

# 처음에 WaitForMultipleObject를 이용해서 8명의 클라이언트가 send()한 것을 
서버에서 모두 recv()할 때 까지 기다리게 하려고 했다.
그런데 8명의 클라이언트 중 한명이라도 연결을 끊어버리는 경우, 
WaitForMultipleObject함수가 -1을 반환할 것을 감안해 BOOL값을 이용해서 이 문제를 해결하기로 생각중


CreateThread함수는 최대 8개가 있어야하는데


recv 8번 해야한느거아닌가
# NetworkThread 
	recv()
	wait*() Collision처리
	send()

# CollisionThread 

# userNum 

# recvNum 

