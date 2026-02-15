
#include <windows.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// 게임라이브러리에서 사용되는 함수 포인터

// 함수 포인터 변수
int		(*g_pFncInit)(int, char);
void	(*g_pFncDestroy)();
int		(*g_pFncRender)();

// 함수 포인터 설정 함수
void	SetEngineInit(int (*)(int, char)  );
void	SetEngineDestroy(void (*)()  );
void	SetEngineRender(int (*)()  );

int		EngineInit();
int		EngineDestroy();
int		EngineRender();


////////////////////////////////////////////////////////////////////////////////
// 사용자 프로그램

int	UserInit(int d)
{
	printf("User Data Init: %d\n", d);
	return 0;
}

void UserDestroy()
{
	printf("User Data Destroy\n");
}


int UserRender()
{
	static int c=0;

	if(++c>10)
		return -1;

	printf("User Data Render:%3d\n", c);

	return c;
}

void main()
{

	SetEngineInit		( (int (*)(int,char))UserInit	);
	SetEngineDestroy	( UserDestroy	);
	SetEngineRender		( UserRender	);

	if(FAILED(EngineInit()))
	{
		printf("Game EngineInit Failed\n");
		return;
	}

	while(1)
	{
		Sleep(500);
		
		if(FAILED(EngineRender()))
			break;
	}

	EngineDestroy();
}


////////////////////////////////////////////////////////////////////////////////
// 게임 라이브러리에서의 구현 코드

int EngineInit()
{
	printf("Game Engine EngineInit\n");

	if(g_pFncInit)
	{
		if(FAILED(g_pFncInit(20, 40) ))
			return -1;
	}

	return 0;
}

int EngineDestroy()
{
	if(g_pFncDestroy)
		g_pFncDestroy();

	printf("Game Engine EngineDestroy\n");
	return 0;
}


int EngineRender()
{
	printf("Game Engine Redering preparation\n");

	if(g_pFncRender)
		return g_pFncRender();

	return -1;
}


void SetEngineInit(int (*pFnc)(int, char)  )
{
	g_pFncInit = pFnc;
}

void SetEngineDestroy(void (*pFnc)()  )
{
	g_pFncDestroy = pFnc;
}

void SetEngineRender(int (*pFnc)()  )
{
	g_pFncRender = pFnc;
}

