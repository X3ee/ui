#include "multi-combo.hpp"

namespace ui {
	static auto stored_index = -1;
	static bool state = false;

	static int m_time;
	static int m_alpha[ ui::m_maximum ];
	static int animation[ ui::m_maximum ];
	static int animation_time[ ui::m_maximum ];

	multi_combo::multi_combo( std::string name ) {
		this->name = name;
		this->items = items;
	}

	void multi_combo::update( ) {
		if ( animation_time[ ui::m_control.get_index( ) ] == 100 && state && stored_index == ui::m_control.get_index( ) && ui::m_input.key_pressed( VK_LBUTTON ) && !ui::m_input.mouse_pointer( { pos.x + 31, pos.y + 40 }, { 130, ( float )( items.size( ) * 18 ) } ) ) {
			state = !state;
			stored_index = -1;
		}

		if ( !ui::m_control.m_animate[ 0 ] && animation[ ui::m_control.get_index( ) ] == 0 && !ui::m_control.color_picker_opened && ui::m_control.m_opened[ ui::m_control.multicombo ] == -1 && m_time == -1 && ui::m_input.key_pressed( VK_LBUTTON ) && ui::m_input.mouse_pointer( { pos.x + 27, pos.y + 16 }, { 130, 18 } ) && !ui::m_control.m_animate[ 3 ] ) {
			state = !state;
			stored_index = ui::m_control.get_index( );
		}

		/* we are going to setup the main animation and time */
		if ( m_time >= 12 )
			m_time = 12;
		else if ( m_time <= 0 )
			m_time = 0;

		/* we are going to setup the open state */
		if ( state )
			m_time++;
		else
			m_time--;

		/* other checks for sanity */
		ui::m_control.m_opened[ ui::m_control.multicombo ] = m_time;

		/* check for stored index and setup trought state */
		if ( stored_index == ui::m_control.get_index( ) )
			focused = state;

		/* check the focus and setup it */
		ui::m_control.m_opened_state[ ui::m_control.multicombo ][ ui::m_control.get_index( ) ] = focused;
	}

	void multi_combo::draw( ) {
		auto combo_animation = [ ]( float x, float y, float w, float h, bool turn ) -> void {
			/* setup the lines box */
			render.filled_rect( x + ( w - 18 ), y, 18, h, color_t( 35, 35, 40 ) );
			render.outlined_rect( x + ( w - 18 ), y, 18, h, color_t( 20, 20, 20 ) );

			/* setup the lines */
			render.filled_rect( x + ( w - 15 ), y + 5, 13, 1, color_t( 82, 82, 82 ) );
			render.filled_rect( x + ( w - 15 ), y + ( 4 * 2 ) + 1, 13, 1, color_t( 82, 82, 82 ) );
			render.filled_rect( x + ( w - 15 ), y + ( 4 * 3 ) + 1, 13, 1, color_t( 82, 82, 82 ) );
		};
		auto combo_drawing = [ ]( float x, float y, float w, float h, std::string name ) -> void {
			render.filled_rect( x, y, w, h, color_t( 45, 45, 50 ) );
			render.outlined_rect( x, y, w, h, color_t( 20, 20, 20 ) );
			render.draw_string( { x + 5, y - 15 }, name, color_t( 200, 200, 200 ), fonts::main, font_flags::drop_shadow );
		};

		/* after that we are going to setup open combo so stay close */
		combo_drawing( pos.x, pos.y + 12, 130, 18, name );
		combo_animation( pos.x, pos.y + 12, 130, 18, !focused );

		/* setup focus and animations */
		auto setup_focus = [ ]( bool focused, int animation[ 1000 ], int animation_time[ 1000 ] ) -> void {
			/* focused setup */
			if ( focused ) {
				animation[ ui::m_control.get_index( ) ] += 10;
				animation_time[ ui::m_control.get_index( ) ] += 15;
			}
			else {
				animation[ ui::m_control.get_index( ) ] -= 10;
				animation_time[ ui::m_control.get_index( ) ] -= 15;
			}

			if ( animation_time[ ui::m_control.get_index( ) ] > 255 )
				animation_time[ ui::m_control.get_index( ) ] = 255;

			if ( animation_time[ ui::m_control.get_index( ) ] < 0 )
				animation_time[ ui::m_control.get_index( ) ] = 0;

			if ( animation[ ui::m_control.get_index( ) ] > 100 )
				animation[ ui::m_control.get_index( ) ] = 100;

			if ( animation[ ui::m_control.get_index( ) ] < 0 )
				animation[ ui::m_control.get_index( ) ] = 0;

			std::clamp<int>( animation[ ui::m_control.get_index( ) ], 0, 100 );
			std::clamp<int>( animation_time[ ui::m_control.get_index( ) ], 0, 255 );
		};
		setup_focus( focused, animation, animation_time );
	}
}