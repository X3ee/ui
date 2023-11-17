#pragma once
#include "../../gui.hpp"
#include "../group_box/group_box.h"

// vcc includes
#include <vector>
#include <string>
#include "../base/base.hpp"

namespace ui {
	class group_box;
	class window : public base_csgo {
	public:
		//constructor.
		window( std::string name, vec2_t* m_pos, vec2_t m_size, std::vector<std::string> m_tabs = { }, int* m_selected = 0 );	
		
		void add_group( group_box* group_box );
		vec2_t* m_win_pos;
		std::vector<std::string> m_tabs;
		int* m_selected;
	private:
		void draw( );
		void install( );
		void handle_movement( );
		void initialize_tabs( );

		//aditions.
		std::string name;
		vec2_t size;
	};
}