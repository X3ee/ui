#include "main.hpp"
#include "resources/main_includes.hpp"
#include "render/d3d9 render/render_d3d9.hpp"
#include "ui_space/gui.hpp"
/* modulate the code functions */
void reset_device( );
void cleanup_d3d9_device( );
BOOL create_d3d9_device( HWND );
void setup_render_states( std::function< void( ) > );
ATOM register_my_class( HINSTANCE, LPCTSTR );
BOOL initialize_window( HINSTANCE, LPCTSTR, LPCTSTR );
LRESULT CALLBACK wnd_processing( HWND, UINT, WPARAM, LPARAM );

/* main function where we call all things */
int main( HINSTANCE h_instance, HINSTANCE h_prev_instance, LPWSTR lp_cmd_line, int n_cmd_show ) {
	/* hide console window so we dont have cmd when testing */
	ShowWindow( GetConsoleWindow( ), SW_HIDE );

	/* register main class */
	LPCTSTR lpz_class = L"workspace";
	if ( !register_my_class( h_instance, lpz_class ) ) {
		return 0;
	}

	/* initialize our main window */
	if ( !initialize_window( h_instance, lpz_class, L"workspace" ) ) {
		return 0;
	}

	/* create our d3d9 device so we can render the main window */
	if ( !create_d3d9_device( g_window ) ) {
		cleanup_d3d9_device( );
		UnregisterClass( lpz_class, h_instance );
		return 0;
	}

	/* Show window */
	ShowWindow( g_window, SW_SHOWDEFAULT );
	UpdateWindow( g_window );

	/* setup next strings */
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	/* view memory */
	while ( msg.message != WM_QUIT ) {
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
			/* translate strings */
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		/* fixing rendering bugs */
		g_device->SetRenderState( D3DRS_ZENABLE, false );
		g_device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		g_device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );
		//g_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 0, 0, 0, 0 ), 0.0f, 0 );
		g_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 80, 80, 80, 0 ), 0.0f, 0 );
		/* beging rendering */
		if ( g_device->BeginScene( ) >= 0 ) {
			setup_render_states( [ = ]( ) {
				/* initialize rendering */
				render.init( g_device );
				render.setup_render_states( );

				ui::m_details.install( );
			});

			/* unload external window */
			if ( GetAsyncKeyState( VK_END ) ) {
				ExitProcess( -1 );
			}

			/* end rendering */
			g_device->EndScene( );
		}

		/* handle packet loss */
		HRESULT result = g_device->Present( NULL, NULL, NULL, NULL );
		if ( result == D3DERR_DEVICELOST && g_device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			reset_device( );
	}

	/* release device */
	cleanup_d3d9_device( );

	/* unregister class */
	DestroyWindow( g_window );
	UnregisterClass( lpz_class, h_instance );

	return 0;
}

/* virtualize voids */
void reset_device( ) {
	HRESULT hr = g_device->Reset( &g_d3d9pp );
	if ( hr != D3DERR_INVALIDCALL ) { }
}
void cleanup_d3d9_device( ) {
	if ( g_device ) {
		g_device->Release( );
		g_device = NULL;
	}
	if ( g_pd3d ) {
		g_pd3d->Release( );
		g_pd3d = NULL;
	}
}
BOOL create_d3d9_device( HWND hWnd ) {
	if ( ( g_pd3d = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
		return FALSE;

	/* Create the D3DDevice */
	ZeroMemory( &g_d3d9pp, sizeof( g_d3d9pp ) );
	g_d3d9pp.Windowed = TRUE;
	g_d3d9pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3d9pp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3d9pp.EnableAutoDepthStencil = TRUE;
	g_d3d9pp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3d9pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; /* Present with vsync */
	if ( g_pd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3d9pp, &g_device ) < 0 )
		return FALSE;

	return TRUE;
}
void setup_render_states( std::function< void( ) > func ) {

	/* Backup the DX9 state */
	IDirect3DStateBlock9* d3d9_state_block = NULL;
	if ( g_device->CreateStateBlock( D3DSBT_ALL, &d3d9_state_block ) < 0 )
		return;

	/* Get Client DisplaySize */
	RECT rect;
	GetClientRect( g_window, &rect );
	vec2_t DisplaySize = vec2_t( ( float )( rect.right - rect.left ), ( float )( rect.bottom - rect.top ) );

	g_device->SetPixelShader( NULL );
	g_device->SetVertexShader( NULL );
	g_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_device->SetRenderState( D3DRS_LIGHTING, false );
	g_device->SetRenderState( D3DRS_ZENABLE, false );
	g_device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_device->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	g_device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	g_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_device->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	g_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	g_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	g_device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	/* Setup viewport */
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = ( DWORD )DisplaySize.x;
	vp.Height = ( DWORD )DisplaySize.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	g_device->SetViewport( &vp );

	/* Setup orthographic projection matrix */ {
		const float L = 0.5f, R = DisplaySize.x + 0.5f, T = 0.5f, B = DisplaySize.y + 0.5f;
		D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } };
		D3DMATRIX mat_projection =
		{
			2.0f / ( R - L ),   0.0f,         0.0f,  0.0f,
			0.0f,         2.0f / ( T - B ),   0.0f,  0.0f,
			0.0f,         0.0f,         0.5f,  0.0f,
			( L + R ) / ( L - R ),  ( T + B ) / ( B - T ),  0.5f,  1.0f,
		};
		g_device->SetTransform( D3DTS_WORLD, &mat_identity );
		g_device->SetTransform( D3DTS_VIEW, &mat_identity );
		g_device->SetTransform( D3DTS_PROJECTION, &mat_projection );
	}

	func( );

	/* Restore the DX9 state */
	d3d9_state_block->Apply( );
	d3d9_state_block->Release( );
}
ATOM register_my_class( HINSTANCE hInstance, LPCTSTR ClassName ) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wnd_processing;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_GUITEST ) );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCE( IDC_GUITEST );
	wcex.lpszClassName = ClassName;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &wcex );
}
BOOL initialize_window( HINSTANCE hInstance, LPCTSTR ClassName, LPCTSTR Title ) {
	/* get position on screen */
	RECT screen_rect;
	GetWindowRect( GetDesktopWindow( ), &screen_rect );

	/* on the whole screen */
	g_window = CreateWindowEx( WS_EX_APPWINDOW, ClassName, Title, WS_POPUP, screen_rect.left, screen_rect.top, screen_rect.right /* width */, screen_rect.bottom /* height */, NULL, NULL, hInstance, NULL );

	if ( !g_window ) {
		return FALSE;
	}

	return TRUE;
}
LRESULT CALLBACK wnd_processing( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == WM_MOUSEMOVE )
		ui::m_input.set_mouse_pos( vec2_t( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ) );

	if ( ui::m_details.get_menu_state( ) && message == WM_MOUSEWHEEL )
		ui::m_input.set_mouse_wheel( GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA );

	return DefWindowProcA( hWnd, message, wParam, lParam );
}

