#include "checkbox.hpp"

namespace ui {
	static int m_alpha[ ui::m_maximum ];
	static int m_progress[ ui::m_maximum ];

	checkbox::checkbox( std::string m_name, bool* value ) {
		this->m_name = m_name;
		this->value = value;
	}

	void checkbox::update( ) {
		auto m_Size = render.get_text_size( m_name, fonts::main );

		if ( ui::m_control.is_possible( ) && !ui::m_control.color_picker_opened && !ui::m_control.m_animate[ 0 ] ) {
			if ( ui::m_input.mouse_pointer( { m_pos.x, m_pos.y }, { 60, 15 } ) && ui::m_input.key_pressed( VK_LBUTTON ) )
				*this->value = !*this->value;
		}
	}

	void checkbox::draw( ) {
		bool is_hovered = ui::m_input.mouse_pointer(
			{ m_pos.x, m_pos.y },
			{ 60, 15 }
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

		render.filled_rect( m_pos.x, m_pos.y, 10, 10, color_t( 45, 45, 50 ) );
		render.draw_string( { m_pos.x + 15, m_pos.y + 3 }, m_name, is_hovered ? color_t( 255, 255, 255, 0 + m_alpha[ ui::m_control.get_index( ) ] ) : color_t( 255, 255, 255, 30 ), fonts::main, font_flags::centered_y | font_flags::drop_shadow);
	
		if ( *value )
			m_progress[ ui::m_control.get_index( ) ] += 3;
		else
			m_progress[ ui::m_control.get_index( ) ] -= 3;

		if ( m_progress[ ui::m_control.get_index( ) ] > 170 )
			m_progress[ ui::m_control.get_index( ) ] = 170;

		if ( m_progress[ ui::m_control.get_index( ) ] < 0 )
			m_progress[ ui::m_control.get_index( ) ] = 0;

		std::clamp<int>( m_progress[ ui::m_control.get_index( ) ], 0, 170 );
		
		render.draw_string( { m_pos.x + 15, m_pos.y + 3 }, m_name, *value ? color_t( 255, 255, 255, 0 + m_progress[ ui::m_control.get_index( ) ] ) : color_t( 255, 255, 255, 30 ), fonts::main, font_flags::centered_y | font_flags::drop_shadow );
		render.gradient_vertical( m_pos.x, m_pos.y, 
			10,
			5 * ( m_progress[ ui::m_control.get_index( ) ] / 100.f ) + 1, 
		color_t( 255, 0, 0 ), color_t( 156, 12, 2 ) );
	
		render.outlined_rect( m_pos.x, m_pos.y, 10, 10, is_hovered ? color_t(
			35, 35, 35, 0 + m_alpha[ ui::m_control.get_index( ) ] ) : color_t( 20,20,20 ) );
	}	
}