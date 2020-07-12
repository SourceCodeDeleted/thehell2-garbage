
#include "resource.h"
#include "PaletteShader.h"
#include "LineShader.h"
#include "storm.h"

LPDIRECT3D9/*EX*/ g_pD3D = 0; // Used to create the D3DDevice
bool DDrawEmulation = 0;
HMODULE HModule = 0;
extern LPDIRECT3DDEVICE9 SecDevice;
extern LPDIRECT3DSURFACE9 SecBuffer;
extern LPDIRECT3DSURFACE9 SecVideoSurface;
extern LPDIRECT3DSURFACE9 SecD3dSurface;
//HANDLE SharedHandle = 0;
extern LPDIRECT3DTEXTURE9 SecTexture;
extern LPDIRECT3DTEXTURE9 SecPalette;

void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp);

//----- (00417D36) --------------------------------------------------------
int __fastcall SetupScreenResolution(HWND ahWnd)
{
	SetFocus(ahWnd);
	//ShowWindow(ahWnd, SW_SHOWNORMAL);
	int guid = 0;
	if( DDrawEmulation ){
		guid = DDCREATE_EMULATIONONLY;
	}
	HRESULT hr = CallDirectDrawCreate((GUID*)guid, &DirectDrawPtr, 0);
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	}

	#ifdef GUARD
	void ProtectAllGuards(); ProtectAllGuards();
	#endif

	#if tryhackddraw
	// Применение ключа совместимости ddraw palette mode 
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\DirectDraw\Compatibility\ 
	((DDRAWI_DIRECTDRAW_INT*)DirectDrawPtr)->lpLcl->dwAppHackFlags |= 0x0800;
	#endif
	UseReservedSystemPalette = 1;

	hr = DirectDrawPtr->SetCooperativeLevel( ahWnd, DDSCL_NORMAL /*GetCooperativeLevel()*/ );
	
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	}
	#ifndef USE_32BITCOLOR
	#ifdef _DEBUG
	hr = DirectDrawPtr->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8);
	#else
	hr = DirectDrawPtr->SetDisplayMode(ScreenWidth, ScreenHeight, 8);
	#endif
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
		// ! тут в оригинале был возврат режима десктопа
	}
	#endif
	// если эти функции сверемые, то при синхронизации потоков 
	// primary surface TH 1 становится 32 битной через раз и вылетает ошибка DDERR_INVALIDPIXELFORMAT
	CreateScreenSurface();
	SetScreenSurfacePalette();
	
	GdiSetBatchLimit( 1 );
	CreateWorkingSurface();
	return SDrawManualInitialize(ahWnd, DirectDrawPtr, ScreenSurface, ConvertPaletteSurfaceToTrueColor, 0, 0/*DDSrcSurface*/, DirectDrawPalettePtr, 0);
// label1: 1AB50  JMP 00 01 19 7D
// FF FE E6 74
// 		 mov	 ConvertPaletteSurfaceToTrueColor,eax	//[ 15 01 EE 2C h], eax	    // *(int*)0x1501EE2C = eax;
// 		 mov     StartX,0								//[ 15 01 EE 44 h], 0
}

//----- (00417E4C) --------------------------------------------------------
void CreateWorkingSurface()
{
	//DWORD v0; // eax@1  HRESULT hr
	//DWORD v1; // eax@4  HRESULT hr
	//DWORD v3; // eax@8  HRESULT hr
	DDSCAPS ddCaps;              // [sp+78h] [bp-4h]@1
	DDSURFACEDESC DDSurfaceDesc; // [sp+Ch] [bp-70h]@4
	HRESULT hr = ScreenSurface->GetCaps(&ddCaps);
	if( FAILED(hr) )
		ShowDirectDrawError(hr, 59, "dx.cpp");
	if( ! UsingDDrawWorkingSurface ){ // always =0
		DDSurfaceDesc.dwSize = sizeof DDSurfaceDesc;
		hr = ScreenSurface->Lock(0, &DDSurfaceDesc, DDLOCK_WRITEONLY | DDLOCK_WAIT, 0);
		if( hr == DD_OK ){
			ScreenSurface->Unlock(0);
			SoftSrcSurfacePtr = (uchar*)AllocMem( (WorkingHeight + 256) * WorkingWidth); // хак отрисовки города
			return;
		}
		if( hr != DDERR_CANTLOCKSURFACE ) //0x887601B3 ){
			ErrorIn_CPP_File(ID_DIALOG1, hr, "dx.cpp", __LINE__);
	}else{
		// эта часть вроде не используется
		memset( &DDSurfaceDesc, 0, sizeof DDSurfaceDesc);
		DDSurfaceDesc.dwSize = sizeof DDSurfaceDesc; // 108
		DDSurfaceDesc.lPitch = WorkingWidth; // 768
		DDSurfaceDesc.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PITCH|DDSD_PIXELFORMAT;
		DDSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		DDSurfaceDesc.dwHeight = WorkingHeight; // 656
		DDSurfaceDesc.dwWidth = WorkingWidth; // 768
		DDSurfaceDesc.ddpfPixelFormat.dwSize = sizeof DDSurfaceDesc.ddpfPixelFormat;
		hr = ScreenSurface->GetPixelFormat( &DDSurfaceDesc.ddpfPixelFormat);
		if( FAILED(hr) )
		  ErrorIn_CPP_File(ID_DIALOG1, hr, "dx.cpp", __LINE__);
	
	    hr = DirectDrawPtr->CreateSurface( &DDSurfaceDesc, &DDSrcSurface, 0);
		if( hr != DD_OK )
			ErrorIn_CPP_File(ID_DIALOG1, hr, "dx.cpp", __LINE__);
	}	
}

#define PS_PALETTE
//----- (00417F59) --------------------------------------------------------
HRESULT CreateScreenSurface()
{
	_EXE( DirectDrawPtr ) EXE( ScreenHeight ) EXE( ScreenWidth ) EXE( ScreenSurface ) EXE( BackBuffer ) EXE( PaletteShader ) EXE ( DirectShader)
	 EXE( ScreenSurface32 ) EXE( D3dDevice ) EXE( D3dSurface ) EXE( ErrorIn_CPP_File ) EXE(D3dTexture) EXE(D3dPalette) EXE(VideoSurface);
	DDSURFACEDESC ScreenDesc;
	DDSURFACEDESC2 ScreenDesc32;
	HRESULT hr;
    memset(&ScreenDesc, 0, sizeof(ScreenDesc));
    ScreenDesc.dwSize = sizeof(ScreenDesc);
	ScreenDesc.dwFlags = DDSD_CAPS;
	#ifdef USE_32BITCOLOR
	//POINT p{ 0, 0 };
	//ClientToScreen( global_hWnd, &p );
	//int desktopHeight = GetSystemMetrics( SM_CYSCREEN );
	//int desktopWidth = GetSystemMetrics( SM_CXSCREEN );
	ScreenDesc.lPitch = ScreenWidth;
	ScreenDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
	ScreenDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	// в главном меню есть разные области, текст, рамки, почти в самом низу номер версии TH
	// один из элементов( вместе с прямоугольником своего фона ) расположен нижней границей на два пикселя ниже края экрана( возможно это как раз номер версии )
	// пока оно рисуется в TH в полноэкранную видеопамять, все нормально, там есть запас неиспользуемой памяти
	// как только я попробовал переделать TH в окно и переместил буфер отрисовки в системную память,
	// тут два лишних пикселя по вертикали создали танцы с бубном на граблях и многомесячную отладку
	// по хорошему функция отрисовки в storm должна проверять выход за границы экрана, но она этого не делает, это есть баг
	// два пикселя для выхода нижнего элемента меню за границу экрана + запасной буфер для проверки возможных других выходов
	ScreenDesc.dwHeight = ScreenHeight + 2 + 128;
	ScreenDesc.dwWidth = ScreenWidth;
    // добавден pixel format в TH 1 через вызов этого ScreenSurface
	ScreenDesc.ddpfPixelFormat.dwSize = sizeof(ScreenDesc.ddpfPixelFormat);
	ScreenDesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	ScreenDesc.ddpfPixelFormat.dwRGBBitCount = 8;

    if( false /*render == ddraw*/ ){
		if( !(IsExeValidating && isEXE) ){
			memset( &ScreenDesc32, 0, sizeof( ScreenDesc32 ) );
			ScreenDesc32.dwSize = sizeof(ScreenDesc32);
			ScreenDesc32.dwFlags = DDSD_CAPS;
			ScreenDesc32.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE /*| DDSCAPS_VISIBLE*/;
			hr = DirectDrawPtr7->CreateSurface(&ScreenDesc32, &ScreenSurface32, nullptr);
			if( FAILED(hr) ){
				//__debugbreak();
				ErrorIn_CPP_File(ID_DIALOG1, hr, "dx_utility.cpp::DDCreateSurface", __LINE__);
			}
		}

	}else{ /*render == d3d*/ 
		if( D3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D back buffer" );
		if( IsSecondWin	&& SecDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &SecBuffer ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second window back buffer" );
		if( D3dDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &VideoSurface, 0/*&SharedHandle*/ ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D render target" );
		if( IsSecondWin && SecDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &SecVideoSurface, 0/*&SharedHandle*/ ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second render target" );
		if( D3dDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &D3dSurface, 0 ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D offscreen surface" );
		if( IsSecondWin && SecDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &SecD3dSurface, 0 ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second offscreen surface" );
		// сэмплинг текстуры с палитровым шейдером не работает, текстурируем в промежуточный буфер(VideoSurface)
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		DrawMode = D3dRef ? 0 : 1 ; // форсируем режим текстуры для увеличения количества цветов
		#if !defined(TH1) && defined(MORE_COLOR)
		constexpr D3DFORMAT fmt = D3DFMT_L16;
		constexpr int colors = 512;
		#else
		constexpr D3DFORMAT fmt = D3DFMT_L8;
		constexpr int colors = 256;
		#endif
		if( DrawMode == 1 ){ // режим текстуры
			if( D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, fmt, D3DPOOL_MANAGED, &D3dTexture, 0 ) == D3D_OK
			 && D3dDevice->CreatePixelShader( (DWORD*) g_ps20_main_palette, &PaletteShader ) == D3D_OK
			 && D3dDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK ){
				D3dDevice->SetTexture( 0, D3dTexture );
				D3dDevice->SetRenderState(D3DRS_LIGHTING, false);
				D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				D3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				D3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				// пиксельный шейдер для режима палитровой текстуры
				D3dDevice->CreateTexture( colors, 1, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &D3dPalette, 0 );
				D3dDevice->SetTexture( 1, D3dPalette );
				D3dDevice->SetPixelShader( PaletteShader );
				// для проверки созданной текстуры
				//D3DSURFACE_DESC d{};
				//D3dPalette->GetLevelDesc(0, &d);
				//__debugbreak();
			}else{
				#if !defined(TH1) && defined(MORE_COLOR)
				TerminateWithError( "Unable to create Direct 3D texture" );
				#else
				DrawMode = 0;
				#endif
			}
		}
		if( IsSecondWin && DrawMode == 1 ){ // режим текстуры
			if( SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, fmt, D3DPOOL_MANAGED, &SecTexture, 0 ) == D3D_OK
				&& SecDevice->CreatePixelShader((DWORD*)g_ps20_main_palette, &PaletteShader ) == D3D_OK
				&& SecDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK ){
				SecDevice->SetTexture( 0, SecTexture );
				SecDevice->SetRenderState( D3DRS_LIGHTING, false );
				SecDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				SecDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				SecDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				// пиксельный шейдер для режима палитровой текстуры
				SecDevice->CreateTexture( colors, 1, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &SecPalette, 0 );
				SecDevice->SetTexture( 1, SecPalette );
				SecDevice->SetPixelShader( PaletteShader );
				// для проверки созданной текстуры
				//D3DSURFACE_DESC d{};
				//D3dPalette->GetLevelDesc(0, &d);
				//__debugbreak();
			}else{
				TerminateWithError( "Unable to create Direct 3D second texture" );
			}
		}
	}
	#else
	ScreenDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	#endif
    hr = DirectDrawPtr->CreateSurface(&ScreenDesc, &ScreenSurface, nullptr);
	if( FAILED(hr) ){
		//__debugbreak();
		ErrorIn_CPP_File(ID_DIALOG1, hr, "dx_utility.cpp::DDCreateSurface", __LINE__);
	}
	ScreenSurface->Lock(0, &ScreenDesc, DDLOCK_READONLY | DDLOCK_NOSYSLOCK, 0);
	extern int ScreenSurfacePitch; ScreenSurfacePitch = ScreenDesc.lPitch;
	ScreenSurface->Unlock(0);
	return hr;
}

// th2 ---------------------------------------------------
bool D3dReset()
{
	_EXE(ScreenWidth) EXE(ScreenHeight) EXE(D3dDevice) EXE(D3dSurface) EXE(BackBuffer) EXE(D3dTexture) EXE(D3dPalette) EXE(VideoSurface) EXE(PaletteShader) EXE(DirectShader);
	#ifdef TH1
	if( IsStartTh1 && !Exe::IsMainWindowActive ) return true;
	#endif // TH1
	if( D3dDevice ){
		//if( D3dSurface ){ D3dSurface->Release(); D3dSurface = 0; }
		if( VideoSurface ){ VideoSurface->Release(); VideoSurface = 0; }
		if( BackBuffer ){ BackBuffer->Release(); BackBuffer = 0; }
		D3DPRESENT_PARAMETERS d3dpp;
		SetD3dParam( d3dpp );
		if( D3D_OK != D3dDevice->Reset( &d3dpp ) ){
			return false;
		}
		D3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer );
		D3dDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &VideoSurface, 0 );
		//D3dDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &D3dSurface, 0 );
		//сэмплинг текстуры с палитровым шейдером не работает, текстурируем в промежуточный буфер(VideoSurface)
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		if( DrawMode == 1 ){ // режим текстуры
			D3dDevice->SetRenderState( D3DRS_LIGHTING, false );
			D3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			D3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			D3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			D3dDevice->SetTexture( 0, D3dTexture );
			// пиксельный шейдер для режима палитровой текстуры
			D3dDevice->SetTexture( 1, D3dPalette );
			D3dDevice->SetPixelShader( PaletteShader );
		}
	}
	return true;
}

// th2 ---------------------------------------------------
bool SecondReset()
{
	if( SecDevice ){
		if( SecBuffer ){ BackBuffer->Release(); BackBuffer = 0; }
		if( SecVideoSurface ){ SecVideoSurface->Release(); SecVideoSurface = 0; }
		D3DPRESENT_PARAMETERS d3dpp;
		SetD3dParam( d3dpp );
		if( D3D_OK != SecDevice->Reset( &d3dpp ) ){
			return false;
		}
		SecDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &SecBuffer );
		SecDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &SecVideoSurface, 0 );
		if( DrawMode == 1 ){ // режим текстуры
			SecDevice->SetRenderState( D3DRS_LIGHTING, false );
			SecDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			SecDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SecDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			SecDevice->SetTexture( 0, SecTexture );
			// пиксельный шейдер для режима палитровой текстуры
			SecDevice->SetTexture( 1, SecPalette );
			SecDevice->SetPixelShader( PaletteShader );
		}
	}
	return true;
}

// th2 ---------------------------------------------------
void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp)
{
	_EXE( IsWorldProcess ) EXE(ScreenWidth) EXE(ScreenHeight);
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = BigWin || ! IsFullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = ( ! BigWin && IsFullScreen ) ? ( D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8 ) : D3DFMT_UNKNOWN; // UNKNOWN - цветность десктопа
	//d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE; // do not work
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = (IsFullScreen && !BigWin) ? ScreenWidth : 0; // WinWidth;
	d3dpp.BackBufferHeight = (IsFullScreen && !BigWin) ? ScreenHeight : 0; // WinHeight;
	d3dpp.PresentationInterval = ( Vsync && (LoadOnStart || IsWorldProcess) && !BigWin && IsFullScreen) ? D3DPRESENT_INTERVAL_ONE :  // v-sync in fullscreen in game
	                                                                            D3DPRESENT_INTERVAL_IMMEDIATE;
}

//----- (00417FB1) --------------------------------------------------------
HRESULT __fastcall CallDirectDrawCreate ( GUID *guid, LPDIRECTDRAW* ddraw, IUnknown* iid )
{
	_EXE( HModule ) EXE( global_hWnd ) EXE( D3dDevice ) 
	if( ! HModule ){
		HModule = LoadLibraryA("ddraw.dll");
	}
	// согласно MSDN требуется запрашивать адрес DirectDrawCreate через GetProcAddress
	// без этого в некоторых конфигурациях может не работать, на досуге поразбираюсь и переделаю, если надо будет
	if( true /*render == d3d*/ ){
		D3DPRESENT_PARAMETERS d3dpp;
		bool TryedBigWin = false;
		buggy_res:
		if( ! IsExeValidating ){
			if( g_pD3D == 0 ){
				g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
				// для использования shared resources требуется минимум Windows Vista
				//Direct3DCreate9Ex( D3D_SDK_VERSION, &g_pD3D );
			}
		}
		SetD3dParam(d3dpp);
		if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, global_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice ) != D3D_OK ){
			if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, global_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice ) != D3D_OK){
				if( !TryedBigWin && IsFullScreen && !BigWin ){
					goto tryBigWin;
				}else{
					D3dRef = true;
					SetD3dParam(d3dpp);
					HRESULT hr = g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, global_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice );
					if( hr != D3D_OK ){
						//__debugbreak();
						TerminateWithError("Unable to create Direct3D Device");
					}
				}
			}
		}
		if( IsSecondWin ){
			SetD3dParam( d3dpp );
			if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, SecWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &SecDevice ) != D3D_OK ){
				if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, SecWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &SecDevice ) != D3D_OK ){
					TerminateWithError( "Unable to create second window Direct3D Device" );
				}
			}
		}
		extern int SystemWidth, SystemHeight;
		extern HWND DlgHwnd;
		if( !D3dDevice && IsFullScreen && !BigWin && !TryedBigWin ){
			tryBigWin:
			// обработка кривых разрешений и fullscreen в remote desktop
			BigWin = true;
			TryedBigWin = true;
			WinWidth = SystemWidth;
			WinHeight = SystemHeight;
			SetWindowLongPtr( global_hWnd, GWL_STYLE, WS_POPUP );
			SetWindowPos( global_hWnd, HWND_NOTOPMOST, 0, 0, SystemWidth, SystemHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
			goto buggy_res;
		}
	}else{ /*render == ddraw*/
		if( ! IsExeValidating ){
			DirectDrawCreateEx( 0, (void**)&DirectDrawPtr7, IID_IDirectDraw7, 0 );
		}
		DirectDrawPtr7->SetCooperativeLevel( global_hWnd , IsFullScreen ? DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN : DDSCL_NORMAL );
		if( IsFullScreen ){
			DirectDrawPtr7->SetDisplayMode( ScreenWidth, ScreenHeight, 32, 0, 0 );
		}
	}
	// direct draw пока нельзя отключить полностью, так как storm туда рисует меню и видео
	return DirectDrawCreate( guid, ddraw, iid );
}

//----- (0041802C) --------------------------------------------------------
void LockSrcSurface()
{
	DDSURFACEDESC ddSurfaceDesc; // [sp+0h] [bp-6Ch]@7
	EnterCriticalSection(&DDSurfaceCritSec);
	if( SoftSrcSurfacePtr ){
		WorkingSurface = SoftSrcSurfacePtr;
	}else{
		if( !DDSrcSurface ){
			Sleep(20000);
			TerminateWithError("lock_buf_priv");
		}
		if( !SurfaceLockCount ){
			ddSurfaceDesc.dwSize = sizeof ddSurfaceDesc;
			HRESULT hr = DDSrcSurface->Lock(0, &ddSurfaceDesc, 1u, 0);
			if( FAILED(hr) )
				ShowDirectDrawError(hr, 235, "dx_utility.cpp");
			WorkingSurfaceEnd += (int)(ddSurfaceDesc.lpSurface);
			WorkingSurface = (uchar*)(ddSurfaceDesc.lpSurface);
		}
	}
	++SurfaceLockCount;
}

//----- (004180BE) --------------------------------------------------------
void UnlockSrcSurface()
{
	//DWORD v0; // eax@7  HRESULT hr
	char v1;           // zf@5
	uchar* v2;          // eax@6
	if( !SurfaceLockCount ) //trying to unlock a surface that is not locked
		TerminateWithError("draw main unlock error");
	if( !WorkingSurface )
		TerminateWithError("draw consistency error");
	v1 = SurfaceLockCount-- == 1;
	if( v1 ){
		v2 = WorkingSurface;
		WorkingSurface = 0;
		WorkingSurfaceEnd -= (uint)v2;
		if( !SoftSrcSurfacePtr ){
			HRESULT hr = DDSrcSurface->Unlock(0);
			if( FAILED(hr) )
				ShowDirectDrawError(hr, 273, "dx.cpp");
		}
	}
	LeaveCriticalSection(&DDSurfaceCritSec);
}

// это функция вызывается при выходе, вылете при ошибке,
// при потере и восстановлении сурфейса после альт-таба
// ее можно было бы пометить как несверяемую, но поскольку тут были вылеты в TH 2, пока сверяем
//----- (0041813E) --------------------------------------------------------
void ReleaseDDSurface()
{
	if( global_hWnd ){
		ShowWindow(global_hWnd, SW_HIDE);
	}
	SDrawDestroy();
	EnterCriticalSection(&DDSurfaceCritSec);
	if( SoftSrcSurfacePtr ){
		FreeMem(SoftSrcSurfacePtr);
		SoftSrcSurfacePtr = 0;
	}else if( DDSrcSurface ){
		DDSrcSurface->Release();
		DDSrcSurface = 0;
	}
	SurfaceLockCount = 0;
	WorkingSurface = 0;
	if( ScreenSurface32 ){
		ScreenSurface32->Release();
		ScreenSurface32 = 0;
	}
	if( ScreenSurface ){
		// одно из мест падежа при выходе из игры в 640 на 480
		ScreenSurface->Release(); // куча была повреждена
		ScreenSurface = 0;
	}
	if( DirectDrawPalettePtr ){
		DirectDrawPalettePtr->Release();
		DirectDrawPalettePtr = 0;
	}
	if( DirectDrawPtr ){
		DirectDrawPtr->Release();
		DirectDrawPtr = 0;
	}
	if( DirectDrawPtr7 ){
		DirectDrawPtr7->Release();
		DirectDrawPtr7 = 0;
	}
	if( D3dSurface ){
		D3dSurface->Release();
		D3dSurface = 0;
	}
	if( VideoSurface ){
		VideoSurface->Release();
		VideoSurface = 0;
	}
	if( BackBuffer ){
		BackBuffer->Release();
		BackBuffer = 0;
	}
	if( D3dDevice != NULL ){
		D3dDevice->Release();
		D3dDevice = 0;
	}
	if( IsSecondWin ){
		if( SecD3dSurface ){
			SecD3dSurface->Release();
			SecD3dSurface = 0;
		}
		if( SecVideoSurface ){
			SecVideoSurface->Release();
			SecVideoSurface = 0;
		}
		if( SecBuffer ){
			SecBuffer->Release();
			SecBuffer = 0;
		}
		if( SecDevice != NULL ){
			SecDevice->Release();
			SecDevice = 0;
		}
	}
	if( g_pD3D != NULL ){
		g_pD3D->Release();
		g_pD3D = 0;
	}

	LeaveCriticalSection(&DDSurfaceCritSec);
}

//in-game stuff to mess with the resolution
//----- (004181E6) --------------------------------------------------------
void ReturnToDDSurface()
{
	int v0; // esi@1
	int v1; // esi@3
	int v2; // esi@4
	int v3; // eax@3
	EnterCriticalSection(&DDSurfaceCritSec);
	ZeroCursorImageWidth();
	v0 = SurfaceLockCount;
	while( SurfaceLockCount ){
		UnlockSrcSurface();
	}
	ReleaseDDSurface();
	NeedRedrawAll = 255;
	SetupScreenResolution(global_hWnd);
	v3 = v0;
	v1 = v0 - 1;
	if( v3 ){
		v2 = v1 + 1;
		do{
			LockSrcSurface();
			--v2;
		}while( v2 );
	}
	LeaveCriticalSection(&DDSurfaceCritSec);
}
