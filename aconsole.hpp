#pragma once
#include <Windows.h>
#include <memory>
#include "agui.hpp"
#include "ainput.hpp"



namespace aconsole
{
	// THIS MUST BE CALLED
	void drawConsole();
	void initConsoleSettings();
	void cleanUpConsole();

	void printToConsole(const char* str);
	void printToWatchList(int pos, const char* str);

	// THIS ARE YOUR SETTINGS
	struct settings_console {

		bool wnd_b;
		int wnd_i;

		char console_str[11][150];
		char watch_str[11][150];

	};

	extern std::unique_ptr<settings_console> sConsole;
}