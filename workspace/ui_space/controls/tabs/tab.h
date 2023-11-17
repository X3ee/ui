#pragma once
#include "../../gui.hpp"

namespace ui {
	/* we are going to do that namespace cuz we are using it main window.cpp */
	namespace tab_system {
		void initialize_tabs( );
		bool m_set_switched;
		std::vector<std::string> m_tabs;
		int* m_selected;
	}
}