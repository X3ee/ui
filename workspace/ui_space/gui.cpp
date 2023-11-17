#include "gui.hpp"
#include "controls/handle_controls.hpp"

ui::details ui::m_details;
ui::input ui::m_input;
ui::control ui::m_control;
ui::external_rendering ui::m_ext_render;

namespace ui {
	bool details::get_menu_state( ) {
		return menu_show;
	}

	void details::set_menu_starte( const bool m_state ) {
		menu_show = m_state;
	}

	vec2_t input::get_mouse_pos( ) {
		return m_mouse_pos;
	}

	void input::set_mouse_pos( const vec2_t m_pos ) {
		m_mouse_pos = vec2_t{ m_pos.x, m_pos.y };
	}

	void input::pool_input( ) {
		for ( size_t i{ 0 }; i < 256; i++ ) {
			helpers_input.m_prev_key_state[ i ] = helpers_input.m_key_state[ i ];
			helpers_input.m_key_state[ i ] = GetAsyncKeyState( i );
		}
	}

	bool input::key_pressed( const uintptr_t m_key ) {
		return helpers_input.m_key_state[ m_key ] && !helpers_input.m_prev_key_state[ m_key ];
	}

	bool input::mouse_pointer( const vec2_t m_pos, const vec2_t m_size ) {
		std::pair<bool, bool> m_should_return {
			get_mouse_pos( ).x > m_pos.x && get_mouse_pos( ).y > m_pos.y, 
			get_mouse_pos( ).x < m_pos.x + m_size.x && get_mouse_pos( ).y < m_pos.y + m_size.y
		};

		return m_should_return.first && m_should_return.second;
	}

	void input::set_mouse_wheel( const float m_mouse ) {
		next_menu_wheel = m_mouse;
	}

	float input::get_mouse_wheel( ) {
		return next_menu_wheel;
	}

	int control::get_index( ) {
		return m_index;
	}

	void control::set_index( const uintptr_t m_last ) {
		m_index = m_last;
	}

	bool control::is_possible( ) {
		return m_control.m_opened[ control_type::combo ] == -1 && 
			   m_control.m_opened[ control_type::multicombo ] == -1 &&
			   m_control.m_opened[ control_type::bind ] == -1;
	}

	void external_rendering::install( ) {
		for ( size_t i{ 0 }; i < rect_draw.size( ); i++ )
			render.filled_rect( rect_draw[ i ].pos_x, rect_draw[ i ].pos_y, rect_draw[ i ].m_width, rect_draw[ i ].m_height, rect_draw[ i ].color );

		for ( size_t i{ 0 }; i < gradient_h_draw.size( ); i++ )
			render.gradient_horizontal( gradient_h_draw[ i ].pos_x, gradient_h_draw[ i ].pos_y, gradient_h_draw[ i ].m_width, gradient_h_draw[ i ].m_height, gradient_h_draw[ i ].color, gradient_h_draw[ i ].color2 );

		for ( size_t i{ 0 }; i < gradient_v_draw.size( ); i++ )
			render.gradient_vertical( gradient_v_draw[ i ].pos_x, gradient_v_draw[ i ].pos_y, gradient_v_draw[ i ].m_width, gradient_v_draw[ i ].m_height, gradient_v_draw[ i ].color, gradient_v_draw[ i ].color2 );
	
		for ( size_t i{ 0 }; i < outline_draw.size( ); i++ )
			render.outlined_rect( outline_draw[ i ].pos_x, outline_draw[ i ].pos_y, outline_draw[ i ].m_width, outline_draw[ i ].m_height, outline_draw[ i ].color );

		for ( size_t i{ 0 }; i < text_draw.size( ); i++ )
			render.draw_string( { text_draw[ i ].pos_x, text_draw[ i ].pos_y }, text_draw[ i ].m_text, text_draw[ i ].color, fonts::main, font_flags::drop_shadow );
	
	
		rect_draw.clear( );
		outline_draw.clear( );
		gradient_h_draw.clear( );
		gradient_v_draw.clear( );
		text_draw.clear( );
	}

	static bool test;
	static bool test2;
	static float test_slider = 0;
	static int test_combo = 0;

	vec2_t window_position = vec2_t( 200, 200 );
	std::vector<std::string> tabs = { "ragebot", "visuals", "skins", "misc" };
	std::vector<std::string> test_combo2 = { "1", "2", "3", "4" };
	void details::install( ) {
		m_input.pool_input( );
		m_control.set_index( 0 );
		
		//opened states
		m_control.m_opened[ m_control.control_type::combo ] = -1;
		m_control.m_opened[ m_control.control_type::multicombo ] = -1;
		m_control.m_opened[ m_control.control_type::bind ] = -1;

		//draw
		auto ui_window = new window( "_testing", &window_position, { 550, 420 }, tabs, &m_index ); {
			auto test_main = new group_box( "main", 0, 30, 45, 237, 165, 0 ); {
				ui_window->add_group( test_main );
				test_main->add_element( new checkbox( "enable", &test ) );
				test_main->add_element( new slider( "hitchance", &test_slider, 0, 100, "" ) );
				test_main->add_element( new checkbox( "hello", &test2 ) );
				test_main->add_element( new combo( "accuracy mode", &test_combo, test_combo2 ) );
			} delete test_main;
			auto test_main2 = new group_box( "accuracy", 1, 30, 225, 237, 165, 0 ); {
				ui_window->add_group( test_main2 );
			} delete test_main2;
			auto test_main3 = new group_box( "other", 2, 283, 45, 237, 345, 0 ); {
				ui_window->add_group( test_main3 );
			} delete test_main3;
		} delete ui_window;

		// install externals
		m_ext_render.install( );
	}
}