#include "stdafx.h"
#include "Join.h"


//죽었는지 살았는지 체크
BOOL death_check(BOOL heroVisible) {
	if (heroVisible == false)
		return true;
	
	return false;
}

//죽었을때 다시 시작(F1)키를 눌렀을 때 체크
BOOL click_join(int key, BOOL heroVisible) {
	if (key == 'r' && heroVisible == false)
		return true;

	return false;
}