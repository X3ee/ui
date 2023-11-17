#include "render_d3d9.hpp"

/* main class insc */
d3d9_render render;

/* fonts */
namespace fonts {
	std::shared_ptr<c_font> main = std::make_shared<c_font>( "Verdana", 8, 100 );
	std::shared_ptr<c_font> icon = std::make_shared<c_font>( "Verdana", 10, 100 );
	std::shared_ptr<c_font> tabs = std::make_shared<c_font>( "menu.ttf", 8, 100 ); //Calligraphr
}

/* initialize fonts and renderer */
void d3d9_render::init( IDirect3DDevice9* dev ) {
	if ( b_ready )
		return;

	m_fonts.push_back( fonts::main );
	m_fonts.push_back( fonts::icon );

	init_device( dev );
	b_ready = true;
}

/* initialize device */
void d3d9_render::init_device( IDirect3DDevice9* dev ) {
	this->dev = dev;
	for ( auto& font : m_fonts ) {
		font->init_device_objects( dev );
		font->restore_device_objects( );
	}
}

/* invalidate device so we dont get memory leaks */
void d3d9_render::invalidate_device( ) {
	dev = nullptr;
	for ( auto& font : m_fonts )
		font->invalidate_device_objects( );
}

/* setup render states */
void d3d9_render::setup_render_states( ) {
	dev->SetVertexShader( nullptr );
	dev->SetPixelShader( nullptr );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	dev->SetRenderState( D3DRS_STENCILENABLE, false );

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );

	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	dev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

/* viewports */
D3DVIEWPORT9 d3d9_render::get_viewport( ) {
	D3DVIEWPORT9 vp;
	dev->GetViewport( &vp );
	return vp;
}
void d3d9_render::set_viewport( D3DVIEWPORT9 vp ) {
	dev->SetViewport( &vp );
}

/* prototypes */
void d3d9_render::string_prototype( vec2_t pos, std::wstring string, color_t c, std::shared_ptr<c_font>& font, uint8_t flags ) {
	font->draw_string( std::roundf( pos.x ), std::roundf( pos.y ), D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ), string.c_str( ), flags );
}
vec2_t d3d9_render::text_size_protype( std::wstring string, std::shared_ptr<c_font>& font ) {
	SIZE size;
	font->get_text_extent( string.c_str( ), &size );
	return vec2_t( static_cast< float >( size.cx ), static_cast< float >( size.cy ) );
}
void d3d9_render::gradient_horizontal_prototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b ) {
	b += a;

	/* run vertices */
	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void d3d9_render::gradient_vertical_prototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b )
{
	b += a;

	/* run vertices */
	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void d3d9_render::rect_prototype( vec2_t a, vec2_t b, color_t c ) {
	b += a;
	b.x -= 1;
	b.y -= 1;

	vertice_t verts[ 5 ] = {
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( b.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( b.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( a.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &verts, 20 );
}
void d3d9_render::filled_prototype( vec2_t a, vec2_t b, color_t c ) {
	b += a;

	vertice_t verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}
void d3d9_render::line_prototype( vec2_t a, vec2_t b, color_t c ) {
	vertice_t verts[ 2 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINELIST, 1, &verts, 20 );
}

/* renderings */
void d3d9_render::draw_string( vec2_t pos, std::string string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags ) const {
	string_prototype( pos, std::wstring( string.begin( ), string.end( ) ), color, font, flags );
}
vec2_t d3d9_render::get_text_size( std::string string, std::shared_ptr<c_font>& font ) const {
	return text_size_protype( std::wstring( string.begin( ), string.end( ) ), font );
}
void d3d9_render::gradient_horizontal( float x, float y, float w, float h, color_t c_a, color_t c_b ) {
	gradient_horizontal_prototype( { x, y }, { w, h }, c_a, c_b );
}
void d3d9_render::gradient_vertical( float x, float y, float w, float h, color_t c_a, color_t c_b ) {
	gradient_vertical_prototype( { x, y }, { w, h }, c_a, c_b );
}
void d3d9_render::outlined_rect( float x, float y, float w, float h, color_t c ) {
	rect_prototype( { x, y }, { w + 1, h + 1 }, c );
}
void d3d9_render::filled_rect( float x, float y, float w, float h, color_t c ) {
	filled_prototype( { x, y }, { w, h }, c );
}
void d3d9_render::line( float x, float y, float x1, float x2, color_t c ) {
	line_prototype( { x, y }, { x1, x2 }, c );
}