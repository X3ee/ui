#pragma once
#include "../handle_controls.hpp"
#include "../base/base.hpp"

namespace ui {
	class combo : public base_csgo {
	public:
		combo( std::string name, int* value, std::vector<std::string> items );

		void update( );
		void draw( );
		void draw_box( ); // if that are going to p and initialize, not sure yet

		vec2_t pos;
		bool focused;
	private:
		std::string name;
		int* value;

		std::vector<std::string> items;
	};
}