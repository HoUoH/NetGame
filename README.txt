///////////////////
// 11.20 ����  //
///////////////////

#########
# ������� #
#########
(0) ServerWinAPI.h
- sockAttr����ü �߰���

(1) ServerMainThread()
- SendRecvThread()�� sockAttr����ü ������ sockInfo ����

(2) SendRecvThread()
- send(), recvn()�κ� �������̷� ������ �ް� ��
- WaitForSingleObject(hScnMgrEvent, 10)���� ������, �� �κ� �ּ� ������

(3) ServerWinAPI.cpp
- ScnMgrThread() Ȯ���غ��� [[[Ȧ��]]]

(4) ScnMgrThread.h
- ReturnSendData() �߰�
- SetRecvedData() �߰�

(5)Server_Global.h
- sendData, recvData ����ü �ű�

#####################
# �����̶� Ȧ�� �о ����
#####################
#1_ ����� Ű

Ŭ���̾�Ʈ���� ������ send()���� �� specialKey�� �־������.
�������� �̰��� recv()�ؼ� recvData�� specialKey�� �ʱ�ȭ �Ǹ�

Ȧ�̴�
�׷� �� recvData���� specialKey �о�ͼ�
if (objs[i]->isVisible == false && recvData[i].specialKey == F1)
�϶� objs[i]->SetisVisible(true);�� �ؾ���


#2_ Ȧ�� ScnMgrThread�� WaitForMultipleObjects
�̰� ���� �ڵ忡 �ּ�ó���Ѱ� �о��

#############
# ����� �� ���� #
#############
�÷��̾ Ŭ���̾�Ʈ�� �������� ������, 
���������� �� Ŭ���̾�Ʈ ������ closesocket()�� ���ִ� ������ ��� ĳġ�س���?

-------------------------------------------------------------------------------------------

