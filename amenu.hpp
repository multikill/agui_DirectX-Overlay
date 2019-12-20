#pragma once
#include <Windows.h>
#include <memory>
#include "agui.hpp"
#include "ainput.hpp"


namespace amenu
{
	// THIS MUST BE CALLED
	void drawMenu();
	void initMenuSettings();
	void cleanUpMenu();

	// THIS ARE YOUR SETTINGS
	struct settings_menu {

		bool wnd_b;
		int wnd_key;
		bool btn1_b;
		bool btn2_b;
		bool chb1_b;
		bool chb2_b;
		int sli1_i;
		int sli2_i;
		char txt1_str[50];
		char txt2_str[50];
		int hky1_i;
		int hky2_i;
		char tb1_sz[20];
		char tb2_sz[50];
		int cmb1_i;
		//char* cmb1_sz[10];
		int cmb2_i = 0;
		//char cmb2_sz[10];
	};

	extern std::unique_ptr<settings_menu> sMenu;
}