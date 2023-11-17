#include "base.hpp"
using namespace ui;

base_csgo m_base;
void base_csgo::destroy( base_csgo* m_element ) {
	delete m_element;
}