#include "tab.h"

namespace ui {
	/* we are going to do that namespace cuz we are using it main window.cpp */
	namespace tab_system {
		/* initializing tabs there then call it in window.cpp run */
		void initialize_tabs( ) {
			/* if we are not drawing any tabs dont draw */
			if ( tab_system::m_tabs.empty( ) )
				return;

			bool is_possible = !m_control.color_picker_opened && !m_control.m_animate[ 0 ] &&
				!m_control.m_animate[ 1 ] && !m_control.m_animate[ 2 ] && !m_control.m_animate[ 3 ] && m_control.is_possible( );
				
			/* main int's */
			static int m_alpha[ 6 ];
			static int m_switched{ 0 };
			static int m_next_alpha{ 255 };
			static int m_changed;

			if ( m_switched != *m_selected ) {
				m_next_alpha = -1;
				m_switched = *m_selected;
			}

			/* alpha sys */ {
				if ( m_next_alpha != 255 ) {
					m_next_alpha += 5;
				}

				if ( m_next_alpha > 255 ) {
					m_next_alpha = 255;
				}

				if ( m_next_alpha < 0 ) {
					m_next_alpha = 0;
				}

				/* clamp alpha */
				std::clamp<int>( m_next_alpha, 0, 255 );
			}
			
			/* run tabs */
			for ( int i = 0; i < tab_system::m_tabs.size( ); i++ ) {
				/* tab render */ {
					
				}
			}
		}
	}
}