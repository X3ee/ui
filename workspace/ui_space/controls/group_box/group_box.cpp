#include "group_box.h"

namespace ui {
	// animation;
	static int m_slide[ m_maximum ];
	static bool m_slide_move[ m_maximum ];
	static float m_slide_offset[ m_maximum ];

	group_box::group_box( std::string name, int m_group_num, float pos_x, float pos_y, float width, float height, int m_current_tab, uint32_t m_flags ) {
		this->m_name = name;
		this->m_group_num = m_group_num;
		this->m_pos = { pos_x, pos_y };
		this->m_size = { width, height };
		this->m_tab = m_current_tab;
	}

	/* visible */
	bool group_box::get_visible( ) {
		return m_visible;
	}

	void group_box::set_visible( bool m_vis ) {
		m_visible = m_vis;
	}

	/* position */
	vec2_t group_box::get_pos( ) {
		return m_pos;
	}

	void group_box::set_pos( vec2_t m_position ) {
		m_pos += m_position;
	}
	
	void group_box::install( ) {
		m_elements.offset_x = this->m_pos.x + 10;
		m_elements.offset_y[ this->m_group_num ] = this->m_pos.y + 15 + m_slide[ this->m_group_num ];
		m_slide_offset[ m_group_num ] = 0;

		/* render */
		draw( );
	}

	void group_box::draw( ) {
		static int m_alpha[ ui::m_maximum ];
		bool is_hovered = ui::m_input.mouse_pointer(
			{ m_pos.x, m_pos.y }, { m_size.x, m_size.y + 20 }
		);

		/* is hovered shit */ {
			if ( is_hovered && !ui::m_control.color_picker_opened && !ui::m_control.m_animate[ 0 ] && !ui::m_control.m_animate[ 1 ] && !ui::m_control.m_animate[ 2 ] && !ui::m_control.m_animate[ 3 ] && ui::m_control.is_possible( ) )
				m_alpha[ m_group_num ] += 3;
			else
				m_alpha[ m_group_num ] -= 3;

			if ( m_alpha[ m_group_num ] > 220 )
				m_alpha[ m_group_num ] = 220;
			
			if ( m_alpha[ m_group_num ] < 70 )
				m_alpha[ m_group_num ] = 70;

			std::clamp<int>( m_alpha[ m_group_num ], 70, 220 );
		}

		/* main groupbox */ {
			render.filled_rect( m_pos.x, m_pos.y, this->m_size.x, this->m_size.y, color_t( 31, 31, 37 ) );
			render.outlined_rect( m_pos.x, m_pos.y, this->m_size.x, this->m_size.y, color_t( 44, 44, 50, m_alpha[ m_group_num ] ) );

			/* groupbox name */ {
				render.draw_string( { m_pos.x + 10, m_pos.y - 2 }, this->m_name, is_hovered ? color_t( 255, 255, 255, m_alpha[ m_group_num ] ) : color_t( 255, 255, 255, 60 ), fonts::main, font_flags::centered_y );
			}
		}
	}

	void group_box::set_limit( ) {
		render.set_viewport( {
			(DWORD)m_pos.x, (DWORD)m_pos.y + 10,
			(DWORD)m_size.x, (DWORD)m_size.y - 15, 0.f, 1.f
		} );
	}

	bool group_box::is_hovered( ) {
		return ui::m_input.mouse_pointer( {
				m_pos.x, m_pos.y + 20
		}, {
				m_size.x, m_size.y - 15
		} );
	}

	void group_box::add_element( checkbox* check ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = render.get_viewport( );
			set_limit( );

			check->m_pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			if ( is_hovered( ) )
				check->update( );

			check->draw( );

			m_elements.offset_y[ m_group_num ] += 18;
			m_slide_offset[ m_group_num ] += 18;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			render.set_viewport( m_old );
		}
		delete check;
	}

	void group_box::add_element( slider* slider ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = render.get_viewport( );
			set_limit( );

			slider->pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			if ( is_hovered( ) ) {
				slider->update( );
				slider->m_allow_draw = true;
			}
			else {
				slider->m_allow_draw = false;
			}

			slider->draw( );

			m_elements.offset_y[ m_group_num ] += 30;
			m_slide_offset[ m_group_num ] += 30;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			render.set_viewport( m_old );
		}
		delete slider;
	}

	void group_box::add_element( combo* combo ) {
		if ( get_visible( ) ) {
			D3DVIEWPORT9 m_old = render.get_viewport( );

			/* initialize combo pos in menu elements */
			combo->pos = { m_elements.offset_x, m_elements.offset_y[ m_group_num ] };

			/* do some sanity checks */
			if ( !ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] ) {
				if ( is_hovered( ) ) {
					combo->update( );
				}
			}
			else if ( ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] ) {
				combo->update( );
			}

			/* this is going to be initialized after we do some checks */
			set_limit( );
			render.set_viewport( m_old );

			/* do some sanity checks after setting viewport */
			if ( !ui::m_control.m_opened_state[ ui::m_control.combo ][ ui::m_control.get_index( ) ] )
				set_limit( ); /* set the limit of drawing even when the open state is not true */

			/* initialize the drawing */
			combo->draw( );

			/* finish the initialization */
			m_elements.offset_y[ m_group_num ] += 35;
			m_slide_offset[ m_group_num ] += 35;
			ui::m_control.set_index( ui::m_control.get_index( ) + 1 );
			render.set_viewport( m_old );
		} delete combo;
	}
}
