#include "aconsole.hpp"

namespace aconsole
{
	using namespace agui;

	std::unique_ptr<settings_console> sConsole = std::make_unique<settings_console>();

	bool initOneTimeSettingsConsole = false;
	bool readyForDrawConsole = false;

	constexpr int max_elements = 11;
	char* arr[max_elements];
	int index = max_elements - 1;
	

	void initConsoleSettings()
	{
		if (initOneTimeSettingsConsole)
			return;

		initOneTimeSettingsConsole = true;
		readyForDrawConsole = true;

		// MEMSET CRASH ALL
		//memset(&sConsole, 0, sizeof(sConsole));

		sConsole->wnd_b = true;
		sConsole->wnd_i = VK_INSERT;

		for (int i = 0; i < max_elements; i++)
		{
			arr[i] = sConsole->console_str[i];
		}
	}

	void cleanUpConsole()
	{
		readyForDrawConsole = false;
	}


	void printToConsole(const char* str)
	{
		if (readyForDrawConsole == false)
			return;

		// resort
		char* last = arr[0];
		for (int i = 0; i < max_elements - 1; i++)
		{
			arr[i] = arr[i + 1];
		}
		arr[max_elements - 1] = last;

		sprintf_s(&arr[index][0], 149, str);
		index++;
		if (index == max_elements)
			index = max_elements - 1;
	}

	void printToWatchList(int pos, const char* str)
	{
		if (readyForDrawConsole == false)
			return;

		if (pos > 7)
			pos = 7;

		//sprintf();
		//memset(&sConsole->watch_str[pos][0], '\0', 150);
		sprintf_s(&sConsole->watch_str[pos][0], 149, str);
	}

	// THIS IS YOUR MENU
	void drawConsole() 
	{
		initConsoleSettings();

		if (readyForDrawConsole == false)
			return;

		static auto gui = new guiFunc;
		setGuiFunc(gui);
		gui->init();

		static auto wnd = window("WindowName", &sConsole->wnd_b, &sConsole->wnd_i, 100, 100, 490, 290);
		static auto tit = titlebar("Console - Press [INSERT]");
		static auto rib = ribbon("ribbonbar");
		static auto tab1 = tab("Console");
		static auto fm1 = frame("");
		static auto cl0 = consoleLabel("", &arr[0]);
		static auto cl1 = consoleLabel("", &arr[1]);
		static auto cl2 = consoleLabel("", &arr[2]);
		static auto cl3 = consoleLabel("", &arr[3]);
		static auto cl4 = consoleLabel("", &arr[4]);
		static auto cl5 = consoleLabel("", &arr[5]);
		static auto cl6 = consoleLabel("", &arr[6]);
		static auto cl7 = consoleLabel("", &arr[7]);
		static auto cl8 = consoleLabel("", &arr[8]);
		static auto cl9 = consoleLabel("", &arr[9]);
		static auto cl10 = consoleLabel("", &arr[10]);


		static auto tab2 = tab("Watch");
		static auto fm2 = frame("");

		//static auto lblName = label("[ Position ] [ File, Line ] [ Function ] >> Output");
		static auto lbl00 = label(sConsole->watch_str[0]);
		static auto lbl01 = label(sConsole->watch_str[1]);
		static auto lbl02 = label(sConsole->watch_str[2]);
		static auto lbl03 = label(sConsole->watch_str[3]);
		static auto lbl04 = label(sConsole->watch_str[4]);
		static auto lbl05 = label(sConsole->watch_str[5]);
		static auto lbl06 = label(sConsole->watch_str[6]);
		static auto lbl07 = label(sConsole->watch_str[7]);


		gui->drawAll();
	}
}