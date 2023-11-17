#pragma once
#include "../handle_controls.hpp"
#include "../base/base.hpp"

namespace ui {
	class slider : public base_csgo {
	public:
		slider( std::string name, float* value, int min = 0, int max = 100, std::string suffix = "%" ); // we are going to use suffix % but we will look for prefix later

		/* we are going to do the draw /update function down */
		void draw( );
		void update( );

		vec2_t pos;
		bool m_allow_draw;
	private:
		std::string name;
		std::string suffix;

		float* value;
		int min;
		int max;
	};
}