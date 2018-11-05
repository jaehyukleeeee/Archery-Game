//-----------------------------------------------------------------------------
// File: MyGameEngine.cpp
//
// Desc: This is the first tutorial for using Direct3D. In this tutorial, all
//       we are doing is creating a Direct3D device and using it to clear the
//       window.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device
LPD3DXSPRITE g_pSprite;
LPD3DXFONT g_pFont;
struct Font
{
	RECT FontRect;
	BOOL FontVisible;
	TCHAR string[128];
};
struct Image
{
	LPDIRECT3DTEXTURE9 texture;
	RECT rect;
	D3DXVECTOR3 center;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 velocity;
	BOOL visible;
  D3DXCOLOR color;
};

struct good
{
	Image image;

};

struct Score {
  INT myscore[5];
  INT wr;
  INT or;
};

enum GAME_STATE {
  INIT, NAME, SEL, RUNHELP, ARCHELP, SHOOTHELP, RUN, ARC, SHOOT, END, RANK
};

GAME_STATE g_GameState = INIT;

Font g_pText;

good Start;

good Name;

good Sel;

good Archelp;

good arc_icon;
good ath_icon;
good sht_icon;


good bg;
good target;
good arrow;
good player;
good targetCollbox;
good arrowCollbox;
good record;

Score athletics;
Score archery;
Score clayShooting;
INT totalScore;

INT s;

INT a = 0;

INT stopdelay;

INT hap;

FLOAT distance;

INT myscore = 0;

INT delay = 0;

INT selected = 0;

INT arrowdelay = 0;

VOID Init();
VOID Update();

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

BOOL coll(good a, good b) {

	if (a.image.pos.x <= b.image.pos.x + b.image.rect.right &&
		a.image.pos.x + a.image.rect.right >= b.image.pos.x &&
		a.image.pos.y <= b.image.rect.bottom + b.image.pos.y &&
		a.image.pos.y + a.image.rect.bottom >= b.image.pos.y) {
		return TRUE;
	}

	return FALSE;
}

VOID Init() {

	D3DXCreateSprite(g_pd3dDevice, &g_pSprite);

	D3DXCreateFont(g_pd3dDevice, 30, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"굴림체", &g_pFont);
	g_pText.FontVisible = TRUE;
	g_pText.FontRect = { 1350, 350, 0, 0 };
  
	
  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"record.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &record.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"bg.png", 
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &bg.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"target.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &target.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"arrow.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &arrow.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"player.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &player.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"collbox.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &targetCollbox.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"collbox.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &arrowCollbox.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"main.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &Start.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"name.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &Name.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"sel.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &Sel.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"archelp.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &Archelp.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"archer_icon.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &arc_icon.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"athlete-icon.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &ath_icon.image.texture);

  D3DXCreateTextureFromFileEx(g_pd3dDevice, L"shooting_icon.png",
    D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
    D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &sht_icon.image.texture);

  Start.image.rect = { 0,0,1600,900 };
  Name.image.rect = { 0,0,1600,900 };
  Sel.image.rect = { 0,0,1600,900 };
  Archelp.image.rect = { 0,0,1600,900 };


  ath_icon.image.rect = { 0,0,400,400 };
  ath_icon.image.pos = { 0,200,0 };
  ath_icon.image.color = { 255,255,255,255 };

  arc_icon.image.rect = { 0,0,400,400 };
  arc_icon.image.pos = { 400,200,0 };
  arc_icon.image.color = { 255,255,255,255 };

  sht_icon.image.rect = { 0,0,400,400 };
  sht_icon.image.pos = { 800,200,0 };
  sht_icon.image.color = { 255,255,255,255 };


  record.image.rect = { 0,0,1600,900 };
  record.image.pos = { 0,0,0 };
  record.image.visible = FALSE;


	bg.image.rect = { 0,0,1600,900 };

	target.image.rect = { 0,0,300,300 };
	target.image.pos = { 1285,0,0 };
	target.image.velocity.x = 3;


	arrow.image.rect = { 0,0,35,128 };
	arrow.image.pos = { 715,550,0 };
	arrow.image.velocity.y = 20;
	arrow.image.visible = TRUE;

	player.image.rect = { 0,0,145,148 };
	player.image.pos = { 650,600,0 };

	targetCollbox.image.rect = { 0,0,300,5 };

	arrowCollbox.image.rect = { 0,0,5,5 };

  archery.wr = 48; //48
  archery.or = 45; //45

	
	s = 0;
}

VOID Update() {
  delay++;
	distance = fabs((targetCollbox.image.pos.x + targetCollbox.image.rect.right / 2) - (arrow.image.pos.x + arrow.image.rect.right / 2));
  hap = archery.myscore[0] + archery.myscore[1] + archery.myscore[2] + archery.myscore[3] + archery.myscore[4];


  switch (g_GameState)
  {
    delay++;
  case INIT:
    if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
      g_GameState = NAME;
      delay = 0;
    }
    break;
  case NAME:
    if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
      g_GameState = SEL;
      delay = 0;
    }
    break;
  case SEL:
    if (GetKeyState(VK_LEFT) & 0x80000000 && delay >= 10)
    {
      delay = 0;
      selected--;
    }
    if (GetKeyState(VK_RIGHT) & 0x80000000 && delay >= 10)
    {
      delay = 0;
      selected++;
    }
    if (selected == 0)
    {
      ath_icon.image.color = { 255, 0, 0, 255 };
      arc_icon.image.color = { 255,255,255,255 };
      sht_icon.image.color = { 255,255,255,255 };
      if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
        g_GameState = RUNHELP;
        delay = 0;
      }
    }
    if (selected == 1)
    {
      ath_icon.image.color = { 255, 255, 255, 255 };
      arc_icon.image.color = { 255,0,0,255 };
      sht_icon.image.color = { 255,255,255,255 };
      if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
        g_GameState = ARCHELP;
        delay = 0;
      }
    }
    if (selected == 2)
    {
      ath_icon.image.color = { 255, 255, 255, 255 };
      arc_icon.image.color = { 255,255,255,255 };
      sht_icon.image.color = { 255,0,0,255 };
      if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
        g_GameState = SHOOTHELP;
        delay = 0;
      }
    }
    break;
  case ARCHELP:
    if (GetKeyState(VK_SPACE) & 0x80000000 && delay >= 10) {
      g_GameState = ARC;
      delay = 0;
    }
    break;
  case ARC:
    swprintf(g_pText.string, TEXT("WR : %d\n\nOR : %d\n\n\n1회 점수 : %d\n\n2회 점수 : %d\n\n3회 점수 : %d\n\n4회 점수 : %d\n\n5회 점수 : %d\n\n합산 : %d\n게임점수 : %d\n거리 : %f\na : %d"), archery.wr, archery. or , archery.myscore[0], archery.myscore[1], archery.myscore[2], archery.myscore[3], archery.myscore[4], hap, totalScore, distance, a);

    if (a > 5) {
      delay++;
      if (delay > 5000) {
        delay = 0;
        record.image.visible = TRUE;
      }

    }

    if (hap > archery.wr&&a == 7) {
      archery.wr = hap;
      totalScore = totalScore + 10000;
      a++;
    }

    if (hap > archery. or &&a == 6) {
      archery. or = hap;
      totalScore += totalScore + 3000;
      a++;
    }
    if (hap > 35 && a == 5) {
      totalScore = totalScore + (hap - 35) * 200;
      a++;
    }

    target.image.pos.x -= target.image.velocity.x;

    targetCollbox.image.pos.x = target.image.pos.x;
    targetCollbox.image.pos.y = target.image.pos.y + 145;

    arrowCollbox.image.pos.x = arrow.image.pos.x + 14;
    arrowCollbox.image.pos.y = arrow.image.pos.y;


    if (target.image.pos.x < 0) {
      target.image.pos = { 1285,0,0 };
    }

    if (GetKeyState(0x41) & 0x80000000) {
      s = 1;
    }



    if (s == 1) {
      arrow.image.pos.y -= arrow.image.velocity.y;
    }

    if (arrow.image.pos.y < -128 || coll(targetCollbox, arrowCollbox) == TRUE) {
      s = 0;

      arrow.image.pos = { 715,550,0 };
    }





    if (coll(targetCollbox, arrowCollbox)) {
        
        target.image.pos = { 1285,0,0 };
        if (arrow.image.visible == TRUE) {


          archery.myscore[a] = 10;
          for (int i = 15; i <= 150; i += 15)  // 20 220 20
          {
            if (i >= distance)
            {
              //arrow.image.visible = FALSE;
              if (a < 5) {
                a++;
                target.image.velocity.x += 1;
              }
              break;
            }
            archery.myscore[a]--;
          }
          if (archery.myscore[a] <= 0)
          {
            archery.myscore[a] = 0;
          }


        }
      
    }



    if (target.image.velocity.x == 0) {
      target.image.velocity.x = 3;

    }
    if (a == 5) {
      target.image.velocity.x = 0;
      arrow.image.velocity.y = 0;
    }
    break;
  }
	
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    if( NULL == g_pd3dDevice )
        return;

    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Rendering of scene objects can happen here
		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    switch (g_GameState)
    {
    case INIT:
      g_pSprite->Draw(Start.image.texture, &Start.image.rect, &Start.image.center, &Start.image.pos, 0xffffffff);
      break;
    case NAME:
      g_pSprite->Draw(Name.image.texture, &Name.image.rect, &Name.image.center, &Name.image.pos, 0xffffffff);
      break;
    case SEL:
      g_pSprite->Draw(Sel.image.texture, &Sel.image.rect, &Sel.image.center, &Sel.image.pos, 0xffffffff);
      g_pSprite->Draw(arc_icon.image.texture, &arc_icon.image.rect, &arc_icon.image.center, &arc_icon.image.pos, arc_icon.image.color);
      g_pSprite->Draw(ath_icon.image.texture, &ath_icon.image.rect, &ath_icon.image.center, &ath_icon.image.pos, ath_icon.image.color);
      g_pSprite->Draw(sht_icon.image.texture, &sht_icon.image.rect, &sht_icon.image.center, &sht_icon.image.pos, sht_icon.image.color);
      
      break;
    case ARCHELP:
      g_pSprite->Draw(Archelp.image.texture, &Archelp.image.rect, &Archelp.image.center, &Archelp.image.pos, 0xffffffff);
      break;
    case ARC:


      g_pSprite->Draw(bg.image.texture, &bg.image.rect, &bg.image.center, &bg.image.pos, 0xffffffff);
      g_pSprite->Draw(target.image.texture, &target.image.rect, &target.image.center, &target.image.pos, 0xffffffff);
      g_pSprite->Draw(targetCollbox.image.texture, &targetCollbox.image.rect, &targetCollbox.image.center, &targetCollbox.image.pos, 0xffffffff);

      g_pSprite->Draw(player.image.texture, &player.image.rect, &player.image.center, &player.image.pos, 0xffffffff);
      g_pSprite->Draw(arrow.image.texture, &arrow.image.rect, &arrow.image.center, &arrow.image.pos, 0xffffffff);
      g_pSprite->Draw(arrowCollbox.image.texture, &arrowCollbox.image.rect, &arrowCollbox.image.center, &arrowCollbox.image.pos, 0xffffffff);
      
      g_pFont->DrawText(g_pSprite, g_pText.string, -1, &g_pText.FontRect, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

      if (record.image.visible == TRUE) {
        g_pSprite->Draw(record.image.texture, &record.image.rect, &record.image.center, &record.image.pos, 0xffffffff);
      }

      break;
    }
		g_pSprite->End();
        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 01: MyGameEngine",
                              WS_OVERLAPPEDWINDOW, 100, 100, 1600, 900,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
		Init();
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg;
		ZeroMemory(&msg, sizeof(msg));
        while( msg.message != WM_QUIT )
        {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				Update();
				Render();

			}

        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}



