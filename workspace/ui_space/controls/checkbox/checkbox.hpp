#pragma once
#include "../handle_controls.hpp"
#include "../base/base.hpp"

namespace ui {
	class checkbox : public base_csgo {
	public:
		checkbox( std::string m_name, bool* value );

		void draw( );
		void update( );

		vec2_t m_pos;
	private:
		std::string m_name;
		bool* value;
	};
}