#pragma once
#include "../../gui.hpp"
#include "../base/base.hpp"
#include "../handle_controls.hpp"
#include "../checkbox/checkbox.hpp"
#include "../slider/slider.hpp"
#include "../combo/combo.hpp"

namespace ui {
	class checkbox;
	class slider;
	class combo;
	class group_box : public base_csgo {
	public:
		enum group_flags {
			group_no_draw = 1 << 0,
			group_no_draw_text = 1 << 1
		};

		/* constructor. */
		group_box( std::string name, int m_group_num, float pos_x, float pos_y, float width, float height, int m_current_tab = 0, uint32_t m_flags = 0 );
		void install( );
		void draw( );

		void set_pos( vec2_t m_position );
		vec2_t get_pos( );

		vec2_t m_size;
		vec2_t m_pos;

		bool get_visible( );
		void set_visible( bool m_vis );
		bool m_visible;

		int m_tab;
		int m_group_num; // used for animation

		void set_limit( );
		bool is_hovered( );

		// oofsets
		struct {
			float offset_x;
			float offset_y[ m_maximum ];
		} m_elements;
	public:
		/* add elements */
		void add_element( checkbox* check );
		void add_element( slider* slider );
		void add_element( combo* combo );
	private:
		std::string m_name;
	};
}