

# ◈2D 게임 라이브러리◈


  
라이브러리란 간단히 정리한다면 컴파일 과정을 마친 코드입니다. 라이브러리를 제작하는
  
이유는 코드의 독립성과 작업에 대한 분담입니다. 또한 잘 만들어진 라이브러리는 운영체제가
  
새로운 버전으로 바뀌거나 개발 플랫폼이 바뀌어도 어느 정도 독립성을 확보할 수 있어서
  
작업의 시간을 단축할 수 있습니다.
  

  
하지만 이러한 라이브러리도 약점이 있는데 라이브러리에 대한 버전관리는 항상 신중해야 하고
  
안정성 및 신뢰도의 확보는 실행 파일에서 작성된 것 보다 관리하기가 어려운 점 중에 하나입니다.
  
이러한 약점이 있지만 팀 작업, 대규모 작업을 Divide-and-Conquer 입장에서 라이브러리는 아주
  
유용하며 특히 게임 엔진 제작에서 필 수 입니다.
  
반복되는 코드를 우리는 함수로 만들어 갑니다. 이렇게 프로그램의 행위, 구조 등에서 패턴이
  
보이면 이를 라이브러리로 만들면 됩니다. 또한 게임 프로그램에서 자주 사용되는 윈도우 관련
  
코드들이나 DirectX, 사운드, 네트워크 인풋 등 중의 일부는 한 번 만들어지면 거의 수정할 일이
  
없는 코드로 구현이 되는 데 라이브러리로 만들면 됩니다.
  

  
윈도우 프로그램에서 라이브러리는 크게 정적(Static) 라이브러리와 동적(Dynamic) 라이브러리
  
두 종류가 있습니다. 정적 라이브러리는 컴파일러가 실행 파일을 만드는 링킹 과정에서
  
라이브러리가 추가 되는 라이브러리이고, 동적 라이브러리는 프로그램이 실행(Run Time) 도중에
  
프로그램이 요구하면 운영체제가 라이브러리를 결합을 시킬 때 사용되는 라이브러리 입니다.
  
윈도우에서는 이를 DLL(Dynamic Linking Library)라 부릅니다.
  

  
정적 라이브러리를 사용하면 최종으로 만들어지는 프로그램의 크기는 라이브러리를 추가한 만큼
  
커지게 됩니다. 실행속도는 DLL을 사용하는 것보다 빠릅니다.
  
DLL은 멀티 쓰레드 환경과 실행 시의 속도에서 정적 라이브러리 보다 고려해야 할 지점이 많지만
  
실행 파일의 크기가 작고 주 프로그램을 수정하지 않아도 DLL만 수정해서 재 배포 할 수 있다는
  
장점이 있습니다.
  
특히 사용자의 요구사항이 많이 있을 때 이를 플러그인(Plug-in)이라는 형식으로 DLL을 사용할 수
  
있어서 그래픽 프로그램 등에서 많이 이용합니다. 또한 현재 개발하고 있는 언어보다 동일한
  
기능을 가진 다른 언어를 작성한 프로그램을 사용하는 경우, 예를 들어 Visual Basic과 같은
  
Interpreter 언어에서 핵심 모듈 이나 속도를 요구하는 부분을 C언어로 DLL을 만들어서 성능을
  
높일 때 DLL을 이용합니다.
  

  
DLL이 장점이 많지만 게임 제작에서는 주로 정적 라이브러리를 사용합니다. 정적 라이브러리를
  
개발하는 과정에서 이를 시험해봐야 하는데 이렇게 라이브러리를 시험하는 프로그램을
  
테스트 드라이버(Test Driver), 또는 테스트 스텁(Test Stub)라 합니다.
  
드라이버는 시험하려는 프로그램에 입력 값들을 주고, 결과를 출력하는 프로그램입니다.
  
스텁은 드라이버와 반대로 개발하고 있는 프로그램이 호출하는 테스트용 프로그램입니다.
  
라이브러리 개발에서 시스템 구조의 최상층 추상화 구조부터 아래층 모듈로 통합하는 하향식
  
통합 방법을 사용한다면 라이브러리 시험을 드라이버를 사용하는 것이 편리하고, 반대로 최하층
  
모듈부터 상층으로 통합하는 방법을 사용한다면 시험을 스텁을 이용하는 것이 좋습니다.
  

  
그러나 이 강의에서는 드라이버와 스텁을 따로 구분하지 않고, 간단하게 라이브러리 모듈을
  
시험할 수 있는 프로그램으로 대치하겠습니다.
  

  

  

  

  

## 1. 게임 라이브러리 제작


### 1.1 윈도우 생성


  
간단히 윈도우 프로그램을 생각해 본다면 RegisterClass --> CreateWindow --> Callback 함수를
  
통한 메시지 처리 과정으로 정리할 수 있습니다. 이것을 드라이버에서는 다음과 같은 코드로
  
라이브러리를 호출할 수 있습니다.
  


```

	int main()
	{
		printf("Starting SpLib\n\n");

		// Create Window
		SpLib\_CreateWin(100, 100, 800, 600, "Sp Lib App");
		// Run
		SpLib\_Run();
		//Destroy
		SpLib\_DestroyWin();

		return 0;
	}

```

  
드라이버에서 컴파일이 가능하도록 드라이버에서 사용한 게임 라이브러리 함수들을 모아서
  
헤더파일에 다음과 같이 선언하고, cpp파일에서 구현을 합니다.
  

  
-splib.h 파일-

```

	#ifndef \_SpLib\_H\_
	#define \_SpLib\_H\_

	#include < windows.h>


	int	SpLib\_CreateWin(int x,int y,int width,int height,char* sName);	//윈도우 생성
	void	SpLib\_DestroyWin();			// 윈도우와 DirectX소멸
	int	SpLib\_Run();				// 게임 루프

	#endif

```

  
-splib.cpp 파일-

```


	#include < windows.h>
	#include < stdio.h>
	#include "SpLib.h"

	int SpLib\_CreateWin(int x, int y, int width, int height, char* sName)
	{
		printf("Create Window.\n");
		return 0;
	}

	void SpLib\_DestroyWin()
	{
		printf("Destroy Window.\n");
	}

	int	SpLib\_Run()
	{
		printf("Destroy Window.\n");
		return 0;
	}

```

  
[Spl01\_base00.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base00.zip)
  

  
이 라이브러리를 빌드 하고, 드라이버 프로그램을 컴파일 해서 실행하면 화면에는 "CreateWindow",
  
"Run Window", "Destroy Window" 라는 문자열이 차례 대로 출력됩니다. 이렇게 화면에 예상한대로
  
출력이 되었다면 이 함수들을 게임프로그램에서 응용할 수 있도록 코드를 추가하고,
  
다듬어 나가면 게임 라이브러리가 완성이 되는 것입니다.
  

  
다음으로 실제로 윈도우가 만들어 질 수 있도록 위의 함수들을 다듬어 봅시다. 코드를 추가하는
  
동안 외부에서 라이브러리를 사용할 만한 함수들은 특정한 이름으로 시작을 하는 것이 좋습니다.
  
여기서는 이러한 함수들의 이름을 “SpLib\_”로 시작을 하도록 하겠습니다.
  

  
또한 라이브러리의 변수들을 직접 가져가지 못하고 반드시 함수를 통해서 가져가도록 코드를
  
작성하도록 하는 것이 좋습니다. 이 함수들로 마찬가지로 특정한 이름으로 시작하는 것이 좋으며
  
마찬가지로 “SpLib\_”로 시작하도록 하겠습니다.
  

  
[Spl01\_base01.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base01.zip) 파일의 압축을 풀고 SpLib 폴더에 있는 SpLib.h파일과 SpLib.cpp 파일를 보면
  
상당히 많은 코드가 추가되어 있는 것을 볼 수 있는데 먼저 윈도우 생성을 담당하는 함수의 주요
  
코드는 다음과 같습니다.
  


```

	int SpLib\_CreateWin(int x, int y, int width, int height, char* sName)
		strcpy(m\_sCls, sName);
		m\_dScnX = x;
		m\_dScnY = y;
		m\_dScnW = width;
		m\_dScnH = height;

		m\_hInst =(HINSTANCE)GetModuleHandle(NULL);
	…
		RegisterClass(&wc);
		…
		m\_hWnd =CreateWindow(…);
	…

```

  
SpLib\_CreateWin() 함수는 윈도우 생성을 담당하므로 윈도우의 시작 위치, 크기, 그리고 이름을
  
입력 받아 윈도우를 생성하고 그 결과를 반환합니다.
  


```

	Int SpLib\_Run()
	…
		while(WM\_QUIT != msg.message)
		{
			PeekMessage( &msg, NULL, 0U, 0U, PM\_REMOVE ) != 0 );
			…
			TranslateMessage( &msg );
			…
			DispatchMessage( &msg );
			…
		RenderEnv();
		…
		}
	…

```

  
SpLib\_Run() 함수는 실행을 담당하고 프로그램의 메시지 처리와 게임의 장면을 화면에 그리는
  
역할을 수행합니다. 화면 연출을 담당하는 함수는 RenderEnv()함수인데 이 함수는 라이브러리를
  
사용하는 사람은 그 구현과 함수 이름을 알아야 할 필요가 없어서 “SpLib\_”이름으로 시작하지
  
않고 일반적인 함수이름을 적용했습니다.
  

  
또한 라이브러리 사용자가 윈도우의 핸들과 스타일, 너비와 높이, 타이틀 등을 가져오거나
  
수정할 수 있도록 다음 함수들을 추가했습니다.
  


```

	HWND	SpLib\_GetHwnd();	// 윈도우 핸들 얻기
	int	SpLib\_GetScnW();	// 화면의 크기를 가져오기
	int	SpLib\_GetScnH();	// 화면의 너비를 가져오기

	void	SpLib\_SetWindowTitle(const char *format, ...);	// Window Title
	void	SpLib\_SetWindowStyle(DWORD dSty);		// window Style
	DWORD	SpLib\_GetWindowStyle();

```

  
이 코드에 포함되어 있는 Window API 함수들은 2D 프로그래밍 기초 과정에서 충분히 설명
  
했으므로 더 이상 설명은 하지 않겠습니다.
  
라이브러리를 먼저 컴파일하고 드라이버 프로그램을 컴파일 해서 실행 하면 콘솔 창과 윈도우
  
창이 실행되고 있는 것을 확인할 수 있습니다.
  

  
[Spl01\_base01.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base01.zip)
  

  

  

  

  

### 1.2 DirectX


  
윈도우 생성과 소멸을 아무 에러 없이 구현을 했다면 이제 DirectX를 적용할 차례입니다.
  
DirectX의 디바이스(Device: 장치)는 윈도우 핸들이 필요하므로 윈도우가 생성된 다음
  
인스턴스를 생성합니다.
  
윈도우를 생성하는 함수 SpLib\_CreateWin()에 디바이스의 인스턴스를 다음과 같은 구조로
  
코드를 추가합니다.
  


```

	int SpLib\_CreateWin(…)
	…
	m\_hWnd =CreateWindow(…);
	…
	// D3D 생성
	if( NULL == ( m\_pD3D = Direct3DCreate9( D3D\_SDK\_VERSION ) ) )
		return -1;
	…
	// D3D Device 생성
	if( FAILED( m\_pD3D->CreateDevice( D3DADAPTER\_DEFAULT,D3DDEVTYPE\_HAL,m\_hWnd,
			D3DCREATE\_SOFTWARE\_VERTEXPROCESSING,&m\_d3dpp,&m\_pd3dDevice )))
		return -1;
	…

```

  
또한 윈도우 소멸과 관련해서 비어 있던 SpLib\_DestroyWin()에 디바이스와 D3D 객체의
  
소멸 코드를 추가합니다.
  


```

	void SpLib\_DestroyWin()
	…
		m\_pd3dDevice->Release();
	m\_pD3D->Release();
	…

```

  
보통 컴퓨터 모니터는 전체 화면을 그리는 정도가 수백 Hz 미만입니다. 그런데 CPU는 이보다
  
수 백 배 수 천 배 이상 처리를 할 수 있습니다. 따라서 성능이 낮은 컴퓨터에서 게임을
  
진행하면 장치에 장면 그리는 것을 포기하고 데이터만 갱신해야 하는 경우도 발생합니다.
  
그리고 디바이스와 데이터 갱신을 혼용해서 사용하면 최적화에 어려움이 많아 대부분 게임
  
데이터 갱신과 렌더링은 분리해서 코드를 작성합니다.
  

  
이러한 이유로 게임 데이터의 장면을 연출하는 RenderEnv() 함수를 게임데이터 갱신 함수
  
FrameMove2D()와 장치에 장면을 그리는 Render2D() 함수로 나누어서 이를 구현합니다.
  


```

	int RenderEnv()
	…
		FrameMove2D();
		…
		Render2D();
		…
		hr = m\_pd3dDevice->Present( NULL, NULL, NULL, NULL );
		…


	int FrameMove2D()
	{
		return 0;
	}

	int Render2D()
	…
		m\_pd3dDevice->Clear( 0
	, NULL
	, D3DCLEAR\_TARGET|D3DCLEAR\_ZBUFFER
	, m\_dColor, 1.0f, 0 );

		if( FAILED( m\_pd3dDevice->BeginScene() ) )
			return -1;

		…
		m\_pd3dDevice->EndScene();
	…

```

  
다음 예제는 이러한 내용을 포함하고 디바이스를 생성하고 화면을 RGB(0,0, 255) 색상으로
  
표현하는 라이브러리 예제입니다.
  

  
[Spl01\_base02.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base02.zip)
  

  

  

  

  

### 1.3 Sprite


  
DirectX 9.0는 스프라이트라는 2D 텍스처 렌더링을 위한 특별한 객체가 있습니다.
  
이 스프라이트는 디바이스 인스턴스 객체가 있어야 만들어집니다. 윈도우를 생성하는 함수
  
SpLib\_CreateWin() 함수에서 디바이스를 생성하고 ID3DXSprite 객체를 생성합니다.
  


```

	int SpLib\_CreateWin(…)
		…
	m\_pD3D->CreateDevice(…);

	if(FAILED(D3DXCreateSprite(m\_pd3dDevice, &m\_pd3dSprite)))
		{
			m\_pd3dDevice->Release();
			m\_pD3D->Release();
			return -1;
		}
		…

```

  
스프라이트 객체는 디바이스가 해제되기 전에 먼저 해제합니다.
  


```

	void SpLib\_DestroyWin()
		…
		m\_pd3dSprite->Release();
		…
		m\_pd3dDevice->Release();
	…

```

  
게임에서 사용되는 텍스처를 생성할 함수를 다음과 같이 작성합니다.
  


```

	int SpLib\_TextureLoad(char* sFileName
				, LPDIRECT3DTEXTURE9& pTx
				, D3DXIMAGE\_INFO* pImg
				, unsigned long dc)
	{
		if(FAILED(D3DXCreateTextureFromFileEx(
			m\_pd3dDevice
			, sFileName
			, D3DX\_DEFAULT
			, D3DX\_DEFAULT
			, 1
			, 0
			, D3DFMT\_UNKNOWN
			, D3DPOOL\_MANAGED
			, D3DX\_FILTER\_NONE
			, D3DX\_FILTER\_NONE
			, dc
			, pImg
			, NULL
			, &pTx
			)) )
		{
			printf("Create Texture Failed: %s\n", sFileName);
			pTx = NULL;
			return -1;
		}

		return 0;
	}

```

  
또한 텍스처를 해제할 함수도 다음과 같이 작성합니다.
  


```

	void SpLib\_TextureRelease(LPDIRECT3DTEXTURE9& pTx)
	{
		if(pTx)
		{
			pTx->Release();
			pTx = NULL;
		}
	}

```

  
마지막으로 텍스처를 2D 렌더링을 담당하는 ID3DXSprite 객체의 Draw() 함수를
  
외부에서 사용할 수 있도록 다음과 같이 작성합니다.
  


```

	void SpLib\_Draw2D(LPDIRECT3DTEXTURE9 pSrc
			, RECT* pSrcRect
			, D3DXVECTOR2* pTranslation
			, D3DXVECTOR2* pScaling
			, D3DXVECTOR2* pRot
			, FLOAT fAngle
			, unsigned long dC)
	{
		m\_pd3dSprite->Begin(D3DXSPRITE\_ALPHABLEND);
		D3DXVECTOR3	vRot(0, 0, 0);
		D3DXVECTOR3	vTrans(0, 0, 0);

		if(pRot)
			vRot = D3DXVECTOR3(pRot->x, pRot->y, 0);

		if(pTranslation)
			vTrans = D3DXVECTOR3(pTranslation->x, pTranslation->y, 0);
		m\_pd3dSprite->Draw(pSrc, pSrcRect, &vRot, &vTrans, dC);

		m\_pd3dSprite->End();
	}

```

  
이렇게 하면 렌더링의 기초 공사는 어느 정도 완성입니다. 하지만 드라이버 프로그램에서는
  
텍스처를 만들고 화면에 출력하는 함수를 다음과 같이 작성해야 합니다.
  


```

	int Render()
	{
		RECT rc={0,0, m\_Img.Width, m\_Img.Height};
		SpLib\_Draw2D(m\_pTx, &rc,NULL, NULL, NULL, NULL, 0xFFFFFFFF);

		return 0;
	}

```

  
이 함수가 게임 라이브러리와 동작하려면 이 함수 자체를 전달해야 합니다. 이런 경우 함수
  
포인터를 사용해야 하며 렌더링을 함수 포인터를 전달하는 함수를 제공해야 합니다. 게임
  
라이브러리에서 다음과 같이 함수 포인터를 선언합니다.
  


```

	int (*SpLib\_Render2D)();

```

  
또한 이 함수 포인터에 함수를 설정하는 함수를 다음과 같이 작성합니다.
  


```

	void SpLib\_SetRender( int (*\_Render)() )
	{
		SpLib\_Render2D = \_Render;
	}

```

  
함수 포인터는 게임 라이브러리에서 자주 사용되므로 다음 예제를 통해서 함수 포인터
  
사용을 연습하기 바랍니다.
  

  
[Spl02\_FuncPointer.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl02_FuncPointer.zip)
  

  

  

  
렌더링 함수 포인터를 전달하는 함수를 만들었다면 Render2D() 함수에서 이 함수 포인터를
  
호출하게 되면 드라이버에서 작성한 함수가 호출 됩니다. Render2D()함수는 다음과 같이
  
구현합니다.
  


```

	int Render2D()
	…
		m\_pd3dDevice->Clear(…);

		if( FAILED( m\_pd3dDevice->BeginScene() ) )
			return -1;

		if( FAILED( SpLib\_Render2D() ) )
		{
			m\_pd3dDevice->EndScene();
			return 0;
		}

		m\_pd3dDevice->EndScene();
	…

```

  
드라이버에서는 다음과 같이 텍스처 인스턴스를 선언하고 렌더링 담당 함수를 작성합니다.
  


```

	#include < d3d9.h>
	#include < d3dx9.h>

	…

	LPDIRECT3DTEXTURE9	m\_pTx;
	D3DXIMAGE\_INFO		m\_Img;

	int Render()
	{
		RECT rc={0,0, m\_Img.Width, m\_Img.Height};
		SpLib\_Draw2D(m\_pTx, &rc,NULL, NULL, NULL, NULL, 0xFFFFFFFF);
		return 0;
	}

```

  
main() 함수에서는 위에서 작성한 SpLib\_SetRender() 함수에 Render() 함수를 전달합니다.
  
그리고 텍스처를 라이브러리 함수를 통해서 생성하고 실행 합니다.
  


```

	int main()
	{
		SpLib\_CreateWin(100, 100, 800, 600, "SpLib App");
		SpLib\_SetClearColor(0xFF0000FF);
		SpLib\_TextureLoad("texture/lena.png", m\_pTx, &m\_Img);
		SpLib\_SetRender(Render);
		SpLib\_Run();
		SpLib\_TextureRelease(m\_pTx);
		SpLib\_DestroyWin();

		return 0;
	}

```

  
![](https://github.com/3dapi/bs17_2d_lib/raw/master/img_src/spl_base03.png)
  

  
좀 더 자세한 코드의 내용은 다음 예제를 참고 하기 바랍니다.
  

  
[Spl01\_base03.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base03.zip)
  

  

  

  

  

### 1.4 Keyboard, Mouse


  
텍스처의 생성, 해제, 그리기를 위한 코드와 응용 프로그램에서 사용하기 위한 함수들을
  
추가했습니다. 다음으로 키보드와 마우스를 추가할 차례입니다. 키보드와 마우스는
  
Window API 함수인 GetKeyboardState() 함수와 GetCursorPos() 함수를 이용합니다.
  

  
또한 휠 마우스를 지원하기 위해서 윈도우 메시지를 처리하는 함수에서 휠 마우스에 대한
  
이벤트를 처리하도록 작성하게 되면 간단하게 인풋에 대한 내용이 정리가 됩니다.
  

  
먼저 키보드와 마우스 이벤트를 게임 라이브러리의 FrameMove2D() 함수에서 다음과 같이
  
처리합니다.
  


```

	int FrameMove2D()
	{
		POINT mouse;
		memset(m\_KeyCur, 0, sizeof(m\_KeyCur));
		::GetKeyboardState(m\_KeyCur);

		for(int i=0; i< 256; ++i)
			m\_KeyCur[i] = (m\_KeyCur[i] & 0x80);

		::GetCursorPos(&mouse);
		::ScreenToClient(m\_hWnd, &mouse );

		m\_mouseX = mouse.x;
		m\_mouseY = mouse.y;

		SpLib\_Keyboard(m\_KeyCur);
		SpLib\_Mouse(m\_mouseX, m\_mouseY, m\_mouseZ, 0);

		return SpLib\_FrameMove2D();
	}

	LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	…
		WPARAM wHi = HIWORD(wParam);
		…
		switch(uMsg)
		{
			case WM\_MOUSEWHEEL:
			{
				m\_mouseZ += short( wHi);
				return 0;
			}
	…

```

  
라이브러리에서 사용자가 키보드와 마우스에 대한 부분을 함수 포인터를 사용해서 받아와
  
처리할 수 있도록 다음과 같은 함수 포인터 연결 함수를 제공합니다.
  


```

	// 게임엔진에 키보드 이벤트를 가져오는 함수를 연결해주는 함수
	void	SpLib\_SetKeyboard( int (*\_Keyboard)(unsigned char* key) );

	// 게임엔진에 마우스 이벤트를 가져오는 함수를 연결해주는 함수
	void	SpLib\_SetMouse( int (*\_Mouse)(int x, int y, int z, int \_event) );

```

  
이렇게 함수 포인터를 이용하는 방법 뿐만 아니라 직접 인풋에 대한 내용을 받을 수 있도록
  
다음 함수도 제공합니다.
  


```

	unsigned char* SpLib\_GetKeyboard();
	int		SpLib\_GetMouseX();
	int		SpLib\_GetMouseY();
	int		SpLib\_GetMouseZ();
	int		SpLib\_GetMouseEvent(int nMouse);

```

  
드라이버 프로그램에서는 인풋의 처리가 올바르게 진행 되고 있는지 다음과 같은 예제를
  
만들어서 확인 할 수 있습니다.
  


```

	int m\_MouseX = 0;
	int m\_MouseY = 0;
	int m\_MouseZ = 0;

	int Render()
	{
		RECT rc={0,0, m\_Img.Width, m\_Img.Height};
		D3DXVECTOR2	vcPos(m\_MouseX, m\_MouseY);
		SpLib\_Draw2D(m\_pTx, &rc, &vcPos, NULL, NULL, NULL, 0xFFFFFFFF);
		return 0;
	}

	int FrameMove()
	{
		m\_MouseX = SpLib\_GetMouseX();
		m\_MouseY = SpLib\_GetMouseY();
		m\_MouseZ = SpLib\_GetMouseZ();
		…
		return 0;
	}

```

  
라이브러리를 컴파일 하고 드라이버 프로그램을 실행 하면 다음 그림과 같이 마우스 포인터를
  
따라 그림이 이동하고, 키보드를 누를 때 마다 윈도우 타이틀에 키의 이벤트를 표시하고
  
있음을 확인 할 수 있습니다.
  

  
![](https://github.com/3dapi/bs17_2d_lib/raw/master/img_src/spl_base04.png)
  

  
좀 더 자세한 내용은 다음 예제를 참고 하기 바랍니다.
  

  
[Spl01\_base04.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base04.zip)
  

  

  

  

  

### 1.5 Texture Animation


  
2D 게임에서 애니메이션을 하기 위해서는 동작에 대한 영화필름과 같은 연속적인 동작이 들어
  
있는 그림 파일이 먼저 준비 되어야 합니다. 닌텐도 게임 마리오에서 주인공 마리오에 대한
  
애니메이션 이미지를 Texture/mario.png 에 저장되어 있습니다.
  

  
애니메이션을 위해서 다음으로 필요한 것이 시간을 제어하는 것입니다. 시간 제어용 Window API
  
함수는 GetTickCount() 함수와 timeGetTime() 함수가 있는데 여기서는 timeGetTime() 함수를
  
사용합니다.
  
앞서 2D 게임 강좌에서 이야기 했듯이 timeGetTime() 함수는 컴퓨터가 시작하고 나서부터의
  
시간을 DWORD형태로 되돌려 주는데 이를 이용해서 게임 라이브러리를 만들어 봅시다.
  

  
timeGetTime() 함수는 컴퓨터가 시작한 후부터의 시간 값을 반환하는데 라이브러리에서는
  
프로그램이 시작하고 나서부터의 시간을 돌려주는 함수를 제공하는 것이 좋습니다.
  

  
전역 변수에서 다음과 같이 시간 값을 초기화 합니다.
  


```

	DWORD	m\_dTimeBgn=0;
	DWORD	m\_dTimeElapsed=0;

```

  
시간 경과를 사용자가 원하면 다음과 같은 함수를 통해서 경과 시간을 알려주도록 합니다.
  


```

	DWORD	SpLib\_ElapsedTime()
	{
		m\_dTimeElapsed = timeGetTime() - m\_dTimeBgn;
		return m\_dTimeElapsed;
	}

```

  
![](https://github.com/3dapi/bs17_2d_lib/raw/master/img_src/spl_base05.png)
  

  
드라이버 프로그램에서는 위의 함수를 가지고 텍스처 애니메이션을 진행합니다. 텍스처
  
애니메이션 진행에 대한 부분은 2D 게임 프로그래밍 강좌를 참고 하기 바랍니다.
  
전체 코드는 다음 예제를 참고 하기 바랍니다.
  

  
[Spl01\_base05.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base05.zip)
  

  

  

  

  

### 1.6 Texture List


  
게임 라이브러리의 중요한 역할 중 하나가 게임 데이터를 관리하는 것입니다. 대표적인
  
게임 데이터는 텍스처, 사운드, 폰트 등이 있고, 게임 진행을 위해서 사용자가 만든
  
데이터가 있습니다.
  
텍스처와 같은 데이터는 파일 이름이 같다면 중복해서 생성하지 않도록 하는 것이 매우
  
중요한데 이렇게 중복을 제거하기 위해서는 텍스처 파일을 관리해주는 매니저(Manager)
  
클래스 혹은 프로그램을 작성해야 합니다.
  

  
매니저 프로그램은 이름 또는 인덱스를 위한 검색(Search), 생성(Create), 소멸(Destroy) 등을
  
담당해서 검색을 통해서 중복 생성을 막고, 가비지 컬렉션(Garbage Collection)과 같은
  
기능을 추가하기도 합니다.
  

  
매니저 프로그램의 가장 간단한 형태는 연결 리스트를 사용하거나 아니면 STL의 벡터(Vector)
  
또는 맵(Map)을 이용하는 것입니다. (벡터와 맵의 사용법은 STL 강좌를 참고 하기 바랍니다.)
  

  
이상적인 매니저 프로그램을 작성하는 것이 게임 라이브러리를 만드는 모든 사람들이 원하는
  
바이지만 이것은 많은 노력이 필요하기 때문에 여기서는 텍스처를 생성하면 벡터에 저장하는
  
단순한 구조로 진행하도록 하겠습니다.
  

  
먼저 텍스처 개체(Entity)를 저장하기 위한 다음과 같은 자료 구조를 준비합니다.
  


```

	DWORD	m\_nKeyTx=0;		// 텍스처 개체를 위한 키

	struct McTexture
	{
		DWORD			nKey;		// Key
		char			sFile[256];	// 파일 이름
		LPDIRECT3DTEXTURE9	pTx;		// 텍스처 포인터
		D3DXIMAGE\_INFO		pImg;		// 텍스처 정보

		McTexture()
		{
			memset(sFile, 0, sizeof(sFile));
			pTx	= NULL;
			nKey = ++m\_nKeyTx;
		}

		~McTexture()
		{
			if(pTx)	{ pTx->Release(); pTx = NULL; }
		}
	};

	typedef std::vector< McTexture* > 	lsPDTX;
	typedef lsPDTX::iterator		itPDTX;

	lsPDTX		m\_vTx	;	// Texture

```

  
위의 코드의 “nKey = ++m\_nKeyTx” 부분을 보면 텍스처 개체가 생성될 때마다 키 값을
  
자동으로 증가 시킵니다. 이것은 파일 이름이 같더라도 중복 생성할 경우에는 이것을
  
구분하는 방법이 없기 때문에 별도의 키가 필요하기 때문입니다. 만약 같은 파일 이름이라면
  
무조건 중복 생성을 막는 다면 이 부분은 필요가 없고 파일 이름으로도 충분히 검색이
  
가능합니다.
  

  
텍스처를 생성하는 SpLib\_TextureLoad() 함수에서는 텍스처를 생성한 후에 개체(Entity)를
  
생성하고 벡터에 추가 합니다. 그리고 나서 키 값을 리턴합니다.
  


```

	int	SpLib\_TextureLoad(char* sFileName, DWORD dc)
	{
		PDTX	pTx	= NULL;
		DIMG	pImg;

		if(FAILED(D3DXCreateTextureFromFileEx(…)) )
		{
			…
			return -1;
		}

		McTexture*	p = new McTexture;

		p->pTx = pTx;
		strcpy(p->sFile, sFileName);
		memcpy(&p->pImg, &pImg, sizeof(DIMG));

		m\_vTx.push\_back(p);

		// Key를 돌려 준다.
		return p->nKey;
	}

```

  
즉 라이브러리 사용자는 텍스처 포인터를 이용하는 것이 아니라 키 값을 이용하게 됩니다.
  
이렇게 키 값을 이용한다면 텍스처의 소멸, 그리고 너비와 높이와 같은 정보를 알려주는
  
함수가 필요하고 또한 렌더링 함수의 변화가 필요합니다.
  


```

	// 텍스처 해제
	int SpLib\_TextureRelease(int \_nKey)
	…
		for(int i=0; i< iSize; ++i)
			if(m\_vTx[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		lsPDTX::iterator	itTx;

		itTx = m\_vTx.begin() + nIdx;
		SAFE\_DELETE(	m\_vTx[nIdx]	);
		m\_vTx.erase(itTx);
	…

	// 텍스처 폭
	int SpLib\_TextureWidth(int \_nKey)
	…
		for(int i=0; i< iSize; ++i)
			if(m\_vTx[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		return (int)m\_vTx[nIdx]->pImg.Width;
	…

	// 텍스처 높이 값
	int SpLib\_TextureHeight(int \_nKey)
	…
		for(int i=0; i< iSize; ++i)
		{
			if(m\_vTx[i]->nKey == \_nKey)
				nIdx = i;
		}
		…
		return (int)m\_vTx[nIdx]->pImg.Height;
	…

	// 텍스처 그리기
	int SpLib\_Draw2D(int \_nKey, …)
		…
		for(int i=0; i< iSize; ++i)
			if(m\_vTx[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
	…
		PDTX	pTx =  m\_vTx[nIdx]->pTx;
		hr = m\_pd3dSprite->Begin(D3DXSPRITE\_ALPHABLEND);
		hr = m\_pd3dSprite->Draw(pTx, pSrcRect, NULL
					, &D3DXVECTOR3(pTranslation->x, pTranslation->y, 0)
					, dC);
		if(FAILED(hr))
			return hr;

		return m\_pd3dSprite->End();
	…

```

  
텍스처 리스트 전부는 SpLib\_DestroyWin() 함수에서 해제하도록 합니다.
  


```

	void SpLib\_DestroyWin()
	…
		int iSize = m\_vTx.size();

		for(i=0; i< iSize; ++i)
		{
			SAFE\_DELETE(	m\_vTx[i]	);
		}
		m\_vTx.clear();
	…

```

  
이렇게 키를 이용해서 텍스처를 관리하게 되면 필연적으로 따라 붙는 문제가 발생하는데 가장
  
큰 문제는 검색의 효율성 입니다. 위의 코드 에서는 벡터의 시작부터 키 값을 검색하는데 이
  
부분이 그리 좋아 보이지 않습니다. 따라서 좀 더 빠른 검색을 제공하는 Map을 이용하는 것이
  
좋으며 이 것은 과제로 남겨 놓겠습니다.
  
전체 코드는 다음 예제를 살펴 보기 바랍니다.
  

  
[Spl01\_base06.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base06.zip)
  

  

  

  

  

### 1.7 Font List


  
앞서 텍스처를 벡터를 이용해서 관리했듯이 폰트도 벡터를 이용해서 관리 할 수 있도록 다음과
  
같은 벡터 자료 구조를 준비합니다. 폰트는 특별히 키가 필요 없기 때문에 ID3DXFont 객체를
  
가지고 자료구조를 구성합니다.
  


```

	typedef std::vector< ID3DXFont* >	lsDxFont;
	typedef lsDxFont::iterator		itDxFont;

	lsDxFont	m\_vFont	;	// Font

```

  
폰트 생성을 담당하는 SpLib\_FontCreate() 함수는 폰트를 생성하고 벡터에 개체를 추가한 다음
  
벡터의 인덱스를 반환합니다.
  


```

	int SpLib\_FontCreate(char* sName, LONG iH, BYTE iItalic)
	{
		ID3DXFont*	pD3DXFont;
		D3DXFONT\_DESC hFont =
		{
			iH, 0
			, FW\_NORMAL, 1
			, iItalic
			, ANSI\_CHARSET, OUT\_DEFAULT\_PRECIS
			, ANTIALIASED\_QUALITY, FF\_DONTCARE, "Arial"
		};

		strcpy(hFont.FaceName, sName);

	    if( FAILED(D3DXCreateFontIndirect( m\_pd3dDevice, &hFont, &pD3DXFont ) ) )
		return -1;

		m\_vFont.push\_back(	pD3DXFont);

		return (m\_vFont.size()-1);
	}

```

  
문자열을 출력하는 함수는 문자열 포멧을 가지고 출력 문자열을 구성하고 인덱스를 통해서
  
폰트의 개체를 바로 접근해서 화면에 출력을 합니다.
  


```

	Int SpLib\_FontDrawText(int nIdx,…, , const char *format,…)
	…
		va\_list ap;
		char s[1024];
		…
		va\_start(ap, format);
		vsprintf((char *)s, format, ap);
		va\_end(ap);
		…
		ID3DXFont*	pD3DXFont = m\_vFont[nIdx];
		return pD3DXFont->DrawText(NULL, s, -1, &rc, 0, fontColor );
	…

```

  
폰트 해제는 텍스처의 해제와 마찬가지로 SpLib\_DestroyWin() 함수에서 해제하도록 합니다.
  


```

	void SpLib\_DestroyWin()
	…
		int iSize = m\_vFont.size();

		for(int i=0; i< iSize; ++i)
		{
			SAFE\_RELEASE(	m\_vFont[i]	);
		}

		m\_vFont.clear();
	…

```

  
구체적인 코드는 다음 예제를 이용하기 바랍니다.
  

  
[Spl01\_base07.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base07.zip)
  

  

  

  

  

### 1.8 Sound List


  
이제 마지막으로 사운드만 추가시킨다면 2D 게임 라이브러리의 proto type은 어느 정도
  
완성이 됩니다. 사운드는 2D 게임 프로그래밍 강좌에서 사용했던 코드들을 이용할
  
것입니다. 또한 Wave 파일에 대한 사운드의 대한 생성과 해제는 DirectX의 위저드
  
코드 dsutil.cpp, dsutil.h 파일을 수정해서 사용하겠습니다.
  

  
사운드의 개체를 위한 자료구조는 텍스처와 마찬가지로 다음과 같이 구성합니다.
  


```

	DWORD		m\_nKeySnd=0;		// Sound

	struct McSound
	{
		DWORD		nKey;
		char		sFile[256];
		CSound*		pSnd;

		McSound()
		{
			memset(sFile, 0, sizeof(sFile));
			pSnd = NULL;
			nKey = ++m\_nKeySnd;
		}
		~McSound()
		{
			if(pSnd){ delete pSnd; pSnd = NULL; }
		}
	};

	typedef std::vector< McSound* >	lsMcSound;
	typedef	lsMcSound::iterator	itMcSound;

	lsMcSound	m\_vSound	;	// Sound List

```

  
텍스처와 마찬가지로 사운드를 생성하고 나서 사운드 개체를 생성한 후에 자료구조 벡터에
  
추가하고 키 값을 반환 합니다.
  


```

	int SpLib\_SoundLoad(char* sFileName)
	…
		m\_pSndMn->Create( &pSnd, sFileName, 0, GUID\_NULL, 1 )
		…
		McSound* p = new McSound;

		p->pSnd = pSnd;
		strcpy(p->sFile, sFileName);
		m\_vSound.push\_back(p);
		// Key를 돌려 준다.
		return p->nKey;
	…

```

  
사운드 해제 함수도 또한 키 값을 검색해서 사운드를 해제 합니다.
  


```

	int SpLib\_SoundRelease(int \_nKey)
	…
		for(int i=0; i< iSize; ++i)
			if(m\_vSound[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		lsMcSound::iterator	itSound;
		itSound = m\_vSound.begin() + nIdx;
		SAFE\_DELETE( m\_vSound[nIdx]		);
		m\_vSound.erase(itSound);
		iSize = m\_vSound.size();
	…

```

  
사운드의 재생, 멈춤, Reset, 플레이 중 확인 함수들도 키 값을 이용해서 검색을 한 후에
  
사운드 포인터를 얻어서 이를 각각 구현 합니다.
  


```

	// 사운드 재생
	void	SpLib\_SoundPlay(int \_nKey)
		…
		for(int i=0; i< iSize; ++i)
			if(m\_vSound[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		m\_vSound[nIdx]->pSnd->Play();
		…

	// 사운드 멈춤
	void	SpLib\_SoundStop(int \_nKey)
		…
		for(int i=0; i< iSize; ++i)
			if(m\_vSound[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		m\_vSound[nIdx]->pSnd->Stop();
		…

	// 사운드 리셋
	void	SpLib\_SoundReset(int \_nKey)
		…
		for(int i=0; i< iSize; ++i)
			if(m\_vSound[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		m\_vSound[nIdx]->pSnd->Reset();
		…

	// 사운드 재생 중 확인
	BOOL	SpLib\_SoundIsPlaying(int \_nKey)
		…
		for(int i=0; i< iSize; ++i)
			if(m\_vSound[i]->nKey == \_nKey)
			{
				nIdx = i;
				break;
			}
		…
		return m\_vSound[nIdx]->pSnd->IsSoundPlaying();
		…

```

  
전체 코드는 다음 예제를 이용하기 바랍니다.
  

  
[Spl01\_base08.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base08.zip)
  

  

  
[Spl01\_base09.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base09.zip)
  

  

  
[Spl01\_base10.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base10.zip)
  

  

  
[Spl01\_base11.zip](https://github.com/3dapi/bs17_2d_lib/raw/master/Spl01_base11.zip)
  

  
