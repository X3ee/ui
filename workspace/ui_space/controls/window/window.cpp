#include "window.hpp"

namespace ui {	
	window::window( std::string name, vec2_t* m_pos, vec2_t m_size, std::vector<std::string> m_tabs, int* m_selected ) {
		this->m_win_pos = m_pos;
		this->size = m_size;
		this->name = name;
		this->m_selected = m_selected;
		this->m_tabs = m_tabs;

		this->install( );
	}

	void window::install( ) {
		draw( );
		handle_movement( );
		initialize_tabs( );
	}

	void window::initialize_tabs( ) {
		/* if we are not drawing any tabs dont draw */
		if ( this->m_tabs.empty( ) )
			return;

		bool is_possible = !m_control.color_picker_opened && !m_control.m_animate[ 0 ] &&
			!m_control.m_animate[ 1 ] && !m_control.m_animate[ 2 ] && !m_control.m_animate[ 3 ] && m_control.is_possible( );

		/* main int's */
		static int m_alpha[ 6 ];
		static int m_switched{ 0 };
		static int m_next_alpha{ 50 };
		static int m_next_alpha2{ 80 };
		static int m_next_alpha3{ 220 };
		static int m_changed;

		if ( m_switched != *this->m_selected ) {
			m_next_alpha = -1;
			m_next_alpha2 = -1;
			m_next_alpha3 = -1;
			m_switched = *this->m_selected;
		}

		/* alpha sys */ {
			if ( m_next_alpha != 50 ) {
				m_next_alpha += 2;
			}

			if ( m_next_alpha > 50 ) {
				m_next_alpha = 50;
			}

			if ( m_next_alpha < 0 ) {
				m_next_alpha = 0;
			}

			/* clamp alpha */
			std::clamp<int>( m_next_alpha, 0, 50 );
		}
		/* alpha sys 2 */ {
			if ( m_next_alpha2 != 80 ) {
				m_next_alpha2 += 2;
			}

			if ( m_next_alpha2 > 80 ) {
				m_next_alpha2 = 80;
			}

			if ( m_next_alpha2 < 0 ) {
				m_next_alpha2 = 0;
			}

			/* clamp alpha */
			std::clamp<int>( m_next_alpha2, 0, 80 );
		}
		/* alpha sys 3 */ {
			if ( m_next_alpha3 != 220 ) {
				m_next_alpha3 += 2;
			}

			if ( m_next_alpha3 > 220 ) {
				m_next_alpha3 = 220;
			}

			if ( m_next_alpha3 < 0 ) {
				m_next_alpha3 = 0;
			}

			/* clamp alpha */
			std::clamp<int>( m_next_alpha3, 0, 220 );
		}

		/* run tabs */
		for ( int i = 0; i < this->m_tabs.size( ); i++ ) {
			if ( is_possible && m_input.mouse_pointer( { this->m_win_pos->x + 330 - m_changed + ( i * 55 ), this->m_win_pos->y + 12 }, { 70, 20 } ) ) {
				if ( m_input.key_pressed( VK_LBUTTON ) ) {
					*this->m_selected = i;
				}
			}

			auto tab_size = render.get_text_size( this->m_tabs[ i ].c_str( ), fonts::main );

			if ( i == 0 )
				m_changed = 7;
			else if ( i == 3 )
				m_changed = 13;
			else
				m_changed = 0;

			/* tab render */ {
				render.gradient_vertical( this->m_win_pos->x + 330 - m_changed + ( i * 55 ), this->m_win_pos->y + 16, tab_size.x, 15, color_t( 0, 0, 0, 0 ), i == *this->m_selected ? color_t( 255, 0, 0, m_next_alpha ) : color_t( 0, 0, 0, 0 ) );
				render.draw_string( { this->m_win_pos->x + 330 - m_changed + (i * 55), this->m_win_pos->y + 15 }, this->m_tabs[ i ].c_str( ), i == *this->m_selected ? color_t( 255, 255, 255, m_next_alpha3 ) : color_t( 255, 255, 255, 100 ), fonts::main, font_flags::centered_y );
			
				render.gradient_vertical( this->m_win_pos->x + 330 - m_changed + ( i * 55 ), this->m_win_pos->y + 15, 2, 16, color_t( 0, 0, 0, 0 ), i == *this->m_selected ? color_t( 255, 0, 0, m_next_alpha2 ) : color_t( 0, 0, 0, 0 ) );
				render.gradient_vertical( ( this->m_win_pos->x + 330 - m_changed + ( i * 55 ) ) + tab_size.x, this->m_win_pos->y + 15, 2, 16, color_t( 0, 0, 0, 0 ), i == *this->m_selected ? color_t( 255, 0, 0, m_next_alpha2 ) : color_t( 0, 0, 0, 0 ) );

			}
		}
	}

	void window::handle_movement( ) {
		/* drag & offset */
		static bool m_drag{ false };
		static vec2_t m_offset{ };

		/* delta calculation */
		vec2_t delta = m_input.get_mouse_pos( ) - m_offset;

		/* drag system */ {
			/* dont drag */
			if ( m_drag && !GetAsyncKeyState( VK_LBUTTON ) ) {
				m_drag = false;
			}

			/* drag delta */
			if ( m_drag && GetAsyncKeyState( VK_LBUTTON ) ) {
				*this->m_win_pos = delta;
			}

			/* get mouse pointer */
			if ( m_input.mouse_pointer( vec2_t( this->m_win_pos->x, this->m_win_pos->y ), vec2_t( 210, 21 ) ) ) {
				m_drag = true;
				m_offset = m_input.get_mouse_pos( ) - *this->m_win_pos;
			}
		}

		/* iritate window */
		if ( this->m_win_pos->x < 0 ) {
			this->m_win_pos->x = 0;
		}

		if ( this->m_win_pos->y < 0 ) {
			this->m_win_pos->y = 0;
		}
	}

	void window::draw( ) {
		/* backround */ {
			render.filled_rect( this->m_win_pos->x, this->m_win_pos->y, this->size.x, this->size.y, color_t( 31, 31, 37 ) );

			/* backround design */ {
				for ( int dots = 0; dots < this->size.x / 2; dots++ )
					render.outlined_rect( this->m_win_pos->x + ( dots * 2 ), this->m_win_pos->y, 2, this->size.y, color_t( 23, 23, 27 ) );
			}
		}

		render.outlined_rect( this->m_win_pos->x, this->m_win_pos->y, this->size.x, this->size.y, color_t( 44, 44, 50 ) );
		render.outlined_rect( this->m_win_pos->x + 1, this->m_win_pos->y - 1, this->size.x - 2, this->size.y, color_t( 44, 44, 50 ) );
		render.outlined_rect( this->m_win_pos->x - 1, this->m_win_pos->y - 2, this->size.x + 2, this->size.y + 3, color_t( 23, 23, 28 ) );

		/* outline title bar */ {
			render.outlined_rect( this->m_win_pos->x + 18, this->m_win_pos->y + 29, this->size.x - 36, this->size.y - 46, color_t( 44, 44, 50 ) );
		}

		/* title bar */ {
			render.filled_rect( this->m_win_pos->x + 2, this->m_win_pos->y + 1, this->size.x - 3, 30, color_t( 31, 31, 37 ) );	

			/* menu window shits */ {
				render.filled_rect( this->m_win_pos->x + 2, this->m_win_pos->y + 30, 15, this->size.y - 32, color_t( 31, 31, 37 ) );
				render.filled_rect( this->m_win_pos->x + this->size.x - 16, this->m_win_pos->y + 30, 15, this->size.y - 32, color_t( 31, 31, 37 ) );
				render.filled_rect( this->m_win_pos->x + 2, this->m_win_pos->y + this->size.y - 16, this->size.x - 3, 15, color_t( 31, 31, 37 ) );
			}

			/* menu bar */ {
				render.filled_rect( this->m_win_pos->x + 2, this->m_win_pos->y + 31, this->size.x - 3, 1, color_t( 161, 8, 8 ) );
				render.gradient_vertical( this->m_win_pos->x + 2, this->m_win_pos->y + 31, this->size.x - 3, 10, color_t( 255, 0, 0, 50 ), color_t( 0, 0, 0, 0 ) );
			}

			/* cheat tab rect */ {
				render.filled_rect( this->m_win_pos->x + 302, this->m_win_pos->y + 1, 230, 30, color_t( 27, 27, 34 ) );
				render.filled_rect( this->m_win_pos->x + 302, this->m_win_pos->y + 1, 1, 30, color_t( 44, 44, 50 ) );
				render.filled_rect( this->m_win_pos->x + 532, this->m_win_pos->y + 1, 1, 30, color_t( 44, 44, 50 ) );
			}

			/* cheat tab rect gradient in out */ {
				render.gradient_horizontal( this->m_win_pos->x + 302, this->m_win_pos->y + 1, 12, 30, color_t( 21, 21, 27, 55 ), color_t( 0, 0, 0, 0 ) );
				render.gradient_horizontal( this->m_win_pos->x + 520, this->m_win_pos->y + 1, 12, 30, color_t( 0, 0, 0, 0 ), color_t( 21, 21, 27, 55 ) );
			}

			/* cheat name */ {
				render.draw_string( { this->m_win_pos->x + 7, this->m_win_pos->y + 12 }, "csgo_cheat", color_t( 255, 255, 255, 200 ), fonts::icon, font_flags::centered_y );
			}
		}

	}

	void window::add_group( group_box* group_box ) {
		group_box->set_visible( group_box->m_tab == *this->m_selected );
		if ( group_box->get_visible( ) ) {
			group_box->set_pos( *this->m_win_pos );
			group_box->install( );
		}
	}
}