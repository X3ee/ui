#pragma once

/* c++ includes */
#include "../../resources/main_includes.hpp"
#include "../utils/vec2.hpp"
#include "../utils/color.hpp"
#include "../fonts/font.hpp"

/* vertices */
struct vertice_t {
	float x, y, z, rhw;
	int c;
};

/* fonts */
namespace fonts {
	extern std::shared_ptr<c_font> main;
	extern std::shared_ptr<c_font> icon;
	extern std::shared_ptr<c_font> tabs;
}

/* main class */
class d3d9_render {
public:
	/* class constructor */
	d3d9_render( ) = default;
	~d3d9_render( ) = default;
private:
	/* prototypes */
	static void string_prototype( vec2_t pos, std::wstring string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	static vec2_t text_size_protype( std::wstring string, std::shared_ptr<c_font>& font );
	void gradient_horizontal_prototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b );
	void gradient_vertical_prototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b );
	void filled_prototype( vec2_t a, vec2_t b, color_t c );
	void rect_prototype( vec2_t a, vec2_t b, color_t c );
	void line_prototype( vec2_t a, vec2_t b, color_t c );
public:
	/* main voids */
	void setup_render_states( );
	void init( IDirect3DDevice9* dev );

	void init_device( IDirect3DDevice9* dev );
	void invalidate_device( );

	D3DVIEWPORT9 get_viewport( );
	void set_viewport( D3DVIEWPORT9 vp );

	void draw_string( vec2_t pos, std::string string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags = 0 ) const;
	vec2_t get_text_size( std::string string, std::shared_ptr<c_font>& font ) const;
	void gradient_horizontal( float x, float y, float w, float h, color_t c_a, color_t c_b );
	void gradient_vertical( float x, float y, float w, float h, color_t c_a, color_t c_b );
	void filled_rect( float x, float y, float w, float h, color_t c );
	void outlined_rect( float x, float y, float w, float h, color_t c );
	void line( float x, float y, float x1, float x2, color_t c );
private:
	IDirect3DDevice9* dev;
	bool b_ready = false;
	std::vector<std::shared_ptr<c_font>> m_fonts;
};
extern d3d9_render render;