#include "slider.hpp"

namespace ui {
	static int m_alpha[ ui::m_maximum ];
	static int m_alpha_text[ ui::m_maximum ];
	static int m_alpha_text2[ ui::m_maximum ];

	slider::slider( std::string name, float* value, int min, int max, std::string suffix ) {
		this->name = name;
		this->value = value;
		this->min = min;
		this->max = max;
		this->suffix = suffix;
	}

	void slider::update( ) {
		static auto stored_index = -1;
		auto delta = max - min;

		/* is possible */
		if ( ui::m_control.is_possible( ) && !ui::m_control.color_picker_opened ) {
			if ( ui::m_input.key_pressed( VK_LBUTTON ) & 1 && ui::m_input.mouse_pointer( vec2_t( pos.x, pos.y + 15 ), vec2_t( 185, 8 ) ) )
				stored_index = ui::m_control.get_index( );
		}

		/* updating controls */
		if ( GetAsyncKeyState( VK_LBUTTON ) && stored_index == ui::m_control.get_index( ) ) {
			*value = min + delta * ( ui::m_input.get_mouse_pos( ).x - pos.x ) / 185;

			if ( *value < min )
				*value = min;

			if ( *value > max )
				*value = max;
		}

		/* update index */
		if ( !GetAsyncKeyState( VK_LBUTTON ) )
			stored_index = -1;
	}

	/* draw slider */
	void slider::draw( ) {
		/* is hovered */
		bool is_hovered = ui::m_input.mouse_pointer(
			{ pos.x, pos.y },
			{ 185, 8 }
		);

		if ( is_hovered && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 2 ] && !ui::m_control.m_animate[ 3 ] )
			m_alpha[ ui::m_control.get_index( ) ] += 1;
		else
			m_alpha[ ui::m_control.get_index( ) ] -= 1;

		if ( m_alpha[ ui::m_control.get_index( ) ] > 50 )
			m_alpha[ ui::m_control.get_index( ) ] = 50;

		if ( m_alpha[ ui::m_control.get_index( ) ] < 0 )
			m_alpha[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_alpha[ ui::m_control.get_index( ) ], 0, 50 );

		render.draw_string( { pos.x + 5, pos.y }, name, color_t( 200, 200, 200 ), fonts::main, font_flags::drop_shadow );
		render.outlined_rect( pos.x - 1, pos.y + 14, 186, 8, color_t( 20, 20, 20 ) );
		render.filled_rect( pos.x, pos.y + 15, 185, 7, color_t( 45, 45, 50 ) );

		/* now sliding */
		auto min_delta = *value - min;
		auto delta = max - min;
		auto total = ( min_delta / delta ) * 185;

		if ( total && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 2 ] && !ui::m_control.m_animate[ 3 ] )
			m_alpha_text[ ui::m_control.get_index( ) ] += 1;
		else
			m_alpha_text[ ui::m_control.get_index( ) ] -= 1;

		if ( m_alpha_text[ ui::m_control.get_index( ) ] > 170 )
			m_alpha_text[ ui::m_control.get_index( ) ] = 170;

		if ( m_alpha_text[ ui::m_control.get_index( ) ] < 0 )
			m_alpha_text[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_alpha_text[ ui::m_control.get_index( ) ], 0, 170 );

		render.filled_rect( pos.x, pos.y + 15, total, 7, color_t( 255, 0, 0, 0 + m_alpha_text[ ui::m_control.get_index( ) ] ) );
		
		if ( total && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 2 ] && !ui::m_control.m_animate[ 3 ] )
			m_alpha_text2[ ui::m_control.get_index( ) ] += 1;
		else
			m_alpha_text2[ ui::m_control.get_index( ) ] -= 1;

		if ( m_alpha_text2[ ui::m_control.get_index( ) ] > 150 )
			m_alpha_text2[ ui::m_control.get_index( ) ] = 150;

		if ( m_alpha_text2[ ui::m_control.get_index( ) ] < 0 )
			m_alpha_text2[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_alpha_text2[ ui::m_control.get_index( ) ], 0, 150 );

		if ( *value < max > min ) {
			render.gradient_horizontal( pos.x + total, pos.y + 15, 8, 7, color_t( 255, 0, 0, 0 + m_alpha_text2[ ui::m_control.get_index( ) ] ), color_t( 255, 0, 0, 0 ) );
		}		

		/* is hovered */
		bool is_hovered_value = ui::m_input.mouse_pointer(
			{ pos.x, pos.y + 13 },
			{ 185, 8 }
		);

		std::string buf = std::to_string( ( int )*value );
		auto m_Size = render.get_text_size( buf, fonts::main );

		if ( is_hovered_value ) {
			if ( *value > 99 ) {
				render.filled_rect( pos.x + ( 185 / 2 ) - 20, pos.y + 12, 26, 12, color_t( 35, 35, 40, 170 ) );
				render.outlined_rect( pos.x + ( 185 / 2 ) - 20, pos.y + 12, 26, 12, color_t( 45, 45, 50, 170 ) );

			}
			else {
				render.filled_rect( pos.x + ( 185 / 2 ) - 20, pos.y + 12, 20, 12, color_t( 35, 35, 40, 170 ) );
				render.outlined_rect( pos.x + ( 185 / 2 ) - 20, pos.y + 12, 20, 12, color_t( 45, 45, 50, 170 ) );
			}
			
			if ( *value < 10 ) {
				render.draw_string( { pos.x + ( 185 / 2 ) - 13, pos.y + 12 }, buf, color_t( 255, 255, 255, 170 ), fonts::main, font_flags::drop_shadow );
			}
			else if ( *value > 9 ) {
				render.draw_string( { pos.x + ( 185 / 2 ) - 16, pos.y + 12 }, buf, color_t( 255, 255, 255, 170 ), fonts::main, font_flags::drop_shadow );
			}
			else if ( *value > 99 ) {
				render.draw_string( { pos.x + ( 185 / 2 ) - 24, pos.y + 12 }, buf, color_t( 255, 255, 255, 170 ), fonts::main, font_flags::drop_shadow );
			}
		}
	}
}