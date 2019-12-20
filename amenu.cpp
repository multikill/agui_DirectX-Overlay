#include "amenu.hpp"

namespace amenu
{
	using namespace agui;

	std::unique_ptr<settings_menu> sMenu = std::make_unique<settings_menu>();

	bool initOneTimeSettingsMenu = false;
	bool readyForDrawingMenu = false;

	char* cmb1_sz[] = { "test", "test2", "test3" };
	char* cmb2_sz[]{ "Value 1", "Value 2", "Value 3", "Value 4" };

	void initMenuSettings()
	{
		if (initOneTimeSettingsMenu)
			return;

		initOneTimeSettingsMenu = true;
		readyForDrawingMenu = true;
		//memset(&sMenu, 0, sizeof(sMenu));

		sMenu->wnd_b = true;
		sMenu->wnd_key = VK_HOME;
		sMenu->btn1_b = false;
		sMenu->btn2_b = false;
		sMenu->chb1_b = false;
		sMenu->chb2_b = true;
		sMenu->sli1_i = 20;
		sMenu->sli2_i = 120;
		sprintf(sMenu->txt1_str, "text 1");
		sprintf(sMenu->txt2_str, "");
		//char txt1_str[50];
		//char txt2_str[50];
		sMenu->hky1_i = VK_SPACE;
		sMenu->hky2_i = VK_LEFT;
		sprintf(sMenu->tb1_sz, "Textbox 1");
		sprintf(sMenu->tb2_sz, "Textbox 2");
		sMenu->cmb1_i = 1;
		sMenu->cmb2_i = 0;
	
	}

	void cleanUpMenu()
	{
		readyForDrawingMenu = false;
	}

	// THIS IS YOUR MENU
	void drawMenu()
	{
		initMenuSettings();

		if (readyForDrawingMenu == false)
			return;

		if (sMenu->btn1_b) {
			sprintf(sMenu->txt1_str, "button 1 is clicked");
			sMenu->btn1_b = false;
		}
		if (sMenu->btn2_b) {
			sprintf(sMenu->txt1_str, "button 2 is clicked");
			sMenu->btn2_b = false;
		}

		POINT mouse = ainput::getMousePos();
		sprintf(sMenu->txt2_str, "x: %i, y: %i", mouse.x, mouse.y);

		static auto guiMenu = new guiFunc;
		setGuiFunc(guiMenu);

		static auto wnd = window("WindowName", &sMenu->wnd_b, &sMenu->wnd_key, 700, 100, 490, 290);
		static auto tit = titlebar("Titlebar - Press [HOME]");
		static auto rib = ribbon("ribbonbar");
		static auto tab1 = tab("Tab 1");
		static auto fm1 = frame("frame 1");
		static auto btn1 = button("button 1", &sMenu->btn1_b);
		static auto btn2 = button("button 2", &sMenu->btn2_b);
		static auto txt1 = label(sMenu->txt1_str);
		static auto txt2 = label(sMenu->txt2_str);
		static auto chb1 = checkbox("checkbox 1", &sMenu->chb1_b);
		static auto chb2 = checkbox("checkbox 2", &sMenu->chb2_b);
		static auto sli1 = slider("slider 1", &sMenu->sli1_i, 50);
		static auto sli2 = slider("slider 2", &sMenu->sli2_i, 255);
		static auto hky1 = hotkey("hotkey 1", &sMenu->hky1_i);
		static auto hty2 = hotkey("hotkey 2", &sMenu->hky2_i);

		static auto tb1 = textbox(sMenu->tb1_sz);
		static auto tb2 = textbox(sMenu->tb2_sz);
		static auto tab2 = tab("Tab 2");
		static auto fm2 = frame("frame 2");
		static auto cb1 = combobox("combobox 1", &sMenu->cmb1_i, ARRAYSIZE(cmb1_sz), cmb1_sz);
		static auto cb2 = combobox("combobox 2", &sMenu->cmb2_i, ARRAYSIZE(cmb2_sz), cmb2_sz);
		static auto tab3 = tab("Tab 3");
		static auto fmb3 = frame("frame 3");
		static auto tab4 = tab("Tab 4");
		static auto fm4 = frame("frame 4");

		guiMenu->drawAll();
	}

}