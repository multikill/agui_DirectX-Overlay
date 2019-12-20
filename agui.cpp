#include "agui.hpp"
#include "ainput.hpp"
#include "amenu.hpp"

namespace agui {

	using namespace ainput;
	using namespace adrawing;

#define MAR 10
#define BOR 3
#define PAD 20
#define INFRAMEWIDTH 160
#define INFRAMEHEIGHT 30

	guiObj::guiObj(window * winPtr, guiObj * upPtr, controlType ct, char* srcCaption, int x, int y, int w, int h) :
		win(winPtr), up(upPtr), cType(ct), xPos(x), yPos(y), width(w), height(h),
		id(gFunc->gObjCounter++), rect({ 0,0,0,0 }), mRect({ 0,0,0,0 }),
		bigFont(false), caption(srcCaption)
	{
#if _DEBUG
		int getLen = strlen(caption);
		if (getLen >= 25 || getLen < 0)
			abort();
#endif // DEBUG
	}

	void guiObj::styleUpdate(bool _bigFont)
	{
		bigFont = _bigFont;
	}

	void guiObj::drawBackground()
	{
		DrawFilledRectangle(this->rect, adrawing::DarkGrey);
	}

	void guiObj::drawBorder()
	{
		DrawFilledRectangle({ rect.x1 + BOR, rect.y1 + BOR, rect.x2 - BOR, rect.y2 - BOR }, adrawing::White);
		DrawFilledRectangle({ rect.x1 + 4, rect.y1 + 4, rect.x2 - 4, rect.y2 - 4 }, adrawing::DarkGrey);
	}

	void guiObj::drawStr()
	{
		DrawStringCenter(caption, rect, adrawing::White, bigFont);
	}

	void guiObj::drawExtra()
	{

	}

	void guiObj::draw()
	{
		updateRectPosition();
		drawBackground();
		//drawBorder();
		drawExtra();
		drawStr();
	}

	void guiObj::functionUpdate()
	{

	}


	window::window(char* Caption, bool* show, int* key, int x, int y, int w, int h) :
		guiObj(this, this, controlType::WINDOW, Caption, x, y, w, h),
		active(show), myKey(key)
	{
		win->pGuiObj.push_back(this);
		gFunc->activeWindow = this;
	}

	void window::updateRectPosition()
	{
		rect = MY_D3DRECT{ xPos, yPos, xPos + width, yPos + height };
		mRect = MY_D3DRECT{ xPos, yPos, xPos, yPos };
	}

	void window::functionUpdate()
	{
		if (isKeyClicked(*myKey)) {
			if (*active == true)
				* active = false;
			else
				*active = true;
		}
	}

	titlebar::titlebar(char* Caption, window* wPtr, int height) :
		guiObj(wPtr, wPtr, controlType::TITLE, Caption, 0, 0, 480, height),
		onMoving(false), mousePos({ 0 }), oldWinPos({0})
	{
		styleUpdate(false);
		win->pGuiObj.push_back(this);
		gFunc->activeTitlebar = this;
	}

	void titlebar::updateRectPosition()
	{
		width = win->width;
		rect = MY_D3DRECT{ win->rect.x1, win->rect.y1, win->rect.x1 + width, win->rect.y1 + height };
		mRect = rect;
		moveWindow();
	}

	void titlebar::drawBackground()
	{
		DrawFilledRectangle(this->rect, adrawing::BrightBlack);
	}

	void titlebar::moveWindow() {

		if (isMouseDown() && isMouseInRect(mRect) && (onMoving == false)) {
			mousePos = getMousePos();
			oldWinPos = win->rect;
			onMoving = true;
		}
		if (onMoving && isMouseDown()) {
			POINT newMousePos = getMousePos();
			win->xPos = oldWinPos.x1 + (newMousePos.x - mousePos.x);
			win->yPos = oldWinPos.y1 + (newMousePos.y - mousePos.y);
		}
		if (onMoving && !isMouseDown()) {
			onMoving = false;
		}

	}

	ribbon::ribbon(char* Caption, window* wPtr, int x, int y, int w, int h) :
		guiObj(wPtr, wPtr, controlType::RIBBON, Caption, x, y, w, h)
	{
		gFunc->activeRibbon = this;
		win->pGuiObj.push_back(this);
	}

	void ribbon::updateRectPosition()
	{
		guiObj* tit = nullptr;
		for (auto& a : win->pGuiObj) {
			if (a->cType == controlType::TITLE) {
				tit = a;
			}
		}
		if (tit == nullptr)
			return;

		rect = MY_D3DRECT{ tit->rect.x1, tit->rect.y2, tit->rect.x1 + width, tit->rect.y2 + height };
	}

	tab::tab(char* Caption, window* wPtr, ribbon* rPtr) :
		guiObj(wPtr, rPtr, controlType::TAB, Caption, 0, 0, 40, 10)
	{
		styleUpdate(true);
		gFunc->activeTab = this;
		win->pGuiObj.push_back(this);
		rPtr->ribbonObj.push_back(this);
		int temp = 0;
		if (gFunc->getTabCount(nullptr, temp) == 1)
			active = true;
	}

	void tab::updateRectPosition()
	{
		int pos = 0;
		int count = gFunc->getTabCount(this, pos);
		width = win->width / count;
		height = up->height;
		rect = MY_D3DRECT{ up->rect.x1 + width * pos, up->rect.y1, up->rect.x1 + width * (pos + 1), up->rect.y2 };
		mRect = { rect.x1 + 10, rect.y1 + 5, rect.x2 - 10, rect.y2 - 5 };
	}

	void tab::drawBackground()
	{
		DrawFilledRectangle(this->rect, adrawing::DarkGrey); // Background
		DrawFilledRectangle({ rect.x1, rect.y2 - 5, rect.x2, rect.y2 }, adrawing::BrightGrey); // Down line
		if (this == gFunc->activeTab)
			DrawFilledRectangle(mRect, adrawing::BrightGrey);
	}

	void tab::drawBorder()
	{
		DrawFilledRectangle({ rect.x1, rect.y1 + 5, rect.x1 + 1, rect.y2 - 5 }, adrawing::White);
		DrawFilledRectangle({ rect.x2 - 1, rect.y1 + 5, rect.x2, rect.y2 - 5 }, adrawing::White);
	}

	void tab::functionUpdate()
	{
		if (isMouseInRect(mRect) && isMouseClicked()) {
			gFunc->activeTab = this;
		}
	}

	frame::frame(char* Caption, window* wPtr, tab* tPtr) :
		guiObj(wPtr, tPtr, controlType::FRAME, Caption, 0, 0, 0, 0)
	{
		gFunc->activeFrame = this;
		win->pGuiObj.push_back(this);
	}

	void frame::updateRectPosition()
	{
		if (up == gFunc->activeTab)
			gFunc->activeFrame = this;

		guiObj* rib = nullptr;
		for (auto& a : win->pGuiObj) {
			if (a->cType == controlType::RIBBON) {
				rib = a;
			}
		}
		if (rib == nullptr)
			return;

		width = win->width;
		height = win->height - rib->height;

		rect = MY_D3DRECT{ rib->rect.x1, rib->rect.y2, rib->rect.x1 + width, rib->rect.y2 + height };

	}

	void frame::drawStr()
	{
		DrawStringCenter(caption, rect, adrawing::White, bigFont);
	}

	button::button(char* Caption, bool* btn, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::BUTTON, Caption, 0, 0, INFRAMEWIDTH, INFRAMEHEIGHT),
		btnActive(btn), lastTick(0)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void button::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void button::drawExtra()
	{
		drawButton({ rect.x1, rect.y1, rect.x1 + 2 * height, rect.y2 });
	}

	void button::drawButton(const MY_D3DRECT& btnRect) {

		mRect = { btnRect.x1 + MAR, btnRect.y1 + MAR, btnRect.x1 + INFRAMEWIDTH, btnRect.y1 + INFRAMEHEIGHT };
		DrawFilledRectangle(mRect, adrawing::BrightBlack);
		if (lastTick + 100 > GetTickCount())
			DrawFilledRectangle({ mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightBlack);
		else
			DrawFilledRectangle({ mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightGrey);

	}

	void button::drawStr()
	{
		DrawStringCenter(caption, mRect, adrawing::White, bigFont);
	}

	void button::functionUpdate()
	{
		if (isMouseInRect(mRect) && isMouseClicked()) {
			*btnActive = true;
			lastTick = GetTickCount();
		}

	}

	slider::slider(char* Caption, int* is, int max, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::SLIDER, Caption, 0, 0, INFRAMEWIDTH, INFRAMEHEIGHT),
		actual(is), maximal(max), onMoving(false), newMousePos({ 0,0 }), oldMousePos({ 0,0 })
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void slider::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void slider::drawExtra()
	{
		drawSlider({ rect.x1, rect.y1, rect.x1 + 3 * height, rect.y2 });
	}

	void slider::drawSlider(const MY_D3DRECT& sldRect) {

		int pct = 100 * (*actual) / maximal;
		mRect = { sldRect.x1 + MAR, sldRect.y1 + MAR, sldRect.x1 + 70, sldRect.y1 + INFRAMEHEIGHT };
		DrawFilledRectangle(mRect, adrawing::BrightBlack);
		if (pct) {
			MY_D3DRECT inside = { mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR };
			long full = inside.x2 - inside.x1;
			full = (full * pct) / 100;
			inside.x2 = inside.x1 + full;
			//shrinkSlider(mRect, percent);
			DrawFilledRectangle(inside, adrawing::BrightGrey);
		}
		char percentStr[20];
		sprintf_s(percentStr, sizeof(percentStr), "%i", *actual);
		DrawStringCenter(percentStr, mRect, adrawing::White, bigFont);
	}

	void slider::drawStr()
	{
		DrawStringLeft(caption, { rect.x1 + 80, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
		//DrawStringCenter(caption, { rect.x1 + 80, rect.y1 + MAR, rect.x2, rect.y2 }, *fc, myFont);
	}

	void slider::functionUpdate()
	{

		if ((onMoving == false) && isMouseDown() && isMouseInRect(mRect)) {
			oldMousePos = getMousePos();
			onMoving = true;
		}
		if (onMoving && isMouseDown()) {
			newMousePos = getMousePos();
			if (newMousePos.x > mRect.x2) {
				*actual = maximal;
			}
			else if (newMousePos.x < mRect.x1) {
				*actual = 0;
			}
			else {
				int m = mRect.x2 - mRect.x1;
				int a = newMousePos.x - mRect.x1;
				*actual = (a * maximal) / m;
			}
		}
		if (onMoving && !isMouseDown()) {
			onMoving = false;
		}
	}

	checkbox::checkbox(char* Caption, bool* isActive, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::CHECKBOX, Caption, 0, 0, INFRAMEWIDTH, INFRAMEHEIGHT),
		active(isActive)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void checkbox::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void checkbox::drawExtra()
	{
		drawCheckbox({ rect.x1, rect.y1, rect.x1 + height, rect.y2 });
	}

	void checkbox::drawCheckbox(const MY_D3DRECT& chbRect) {

		mRect = { chbRect.x1 + MAR, chbRect.y1 + MAR, chbRect.x1 + 30, chbRect.y1 + INFRAMEHEIGHT };
		DrawFilledRectangle(mRect, adrawing::BrightBlack);
		if (*active)
			DrawFilledRectangle({ mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightGrey);

	}

	void checkbox::drawStr()
	{
		DrawStringLeft(caption, { rect.x1 + 40, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
	}

	void checkbox::functionUpdate()
	{
		if (isMouseInRect(mRect) && isMouseClicked()) {
			if (*active == true)
				* active = false;
			else
				*active = true;
		}
	}

	hotkey::hotkey(char* Caption, int* key, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::HOTKEY, Caption, 0, 0, INFRAMEWIDTH, INFRAMEHEIGHT),
		myKey(key), onSearch(false), lastTick(0)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void hotkey::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void hotkey::drawExtra()
	{
		drawHotkey({ rect.x1, rect.y1, rect.x1 + 1 * height, rect.y2 });
	}

	void hotkey::drawHotkey(const MY_D3DRECT& hkyRect) {

		mRect = { hkyRect.x1 + MAR, hkyRect.y1 + MAR, hkyRect.x1 + 70, hkyRect.y1 + INFRAMEHEIGHT };
		DrawFilledRectangle(mRect, adrawing::BrightBlack);
		if (onSearch)
			DrawFilledRectangle({ mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightBlack);
		else
			DrawFilledRectangle({ mRect.x1 + BOR, mRect.y1 + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightGrey);

		char percentStr[20];
		sprintf_s(percentStr, sizeof(percentStr), "key: %i", *myKey);
		DrawStringCenter(percentStr, mRect, adrawing::White, bigFont);
	}

	void hotkey::drawStr()
	{
		DrawStringLeft(caption, { rect.x1 + 80, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
	}

	void hotkey::functionUpdate()
	{
		if (isMouseInRect(mRect) && isMouseClicked() && !onSearch) {

			lastTick = GetTickCount();
			onSearch = true;

		}
		if (onSearch) {

			int newKey = getPressedKey();
			if ((VK_LBUTTON == newKey) && (lastTick + 200 > GetTickCount()))
				return;

			if (newKey) {
				*myKey = newKey;
				onSearch = false;
			}
		}
	}

	label::label(char* Caption, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::LABEL, Caption, 0, 0, INFRAMEWIDTH, INFRAMEHEIGHT)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void label::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void label::drawStr()
	{
		DrawStringLeft(caption, { rect.x1 + PAD, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
	}

	consoleLabel::consoleLabel(char* Caption, char** ppStr, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::CONSOLELABEL, Caption, 0, 0, 480, 20),
		ptrPtrStr(ppStr)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}
	void consoleLabel::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}
	void consoleLabel::drawStr()
	{
		caption = *ptrPtrStr;
		DrawStringLeft(caption, { rect.x1 + PAD, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
	}

	textbox::textbox(char* Caption, window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::TEXTBOX, Caption, 0, 0, INFRAMEWIDTH, 30),
		onInput(false), charCounter(0), lastKey(0), newKey(0), lastTick(0)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void textbox::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void textbox::drawExtra()
	{
		mRect = { rect.x1 + MAR, rect.y1 + MAR, rect.x2, rect.y2 };
		if (onInput)
			DrawFilledRectangle(mRect, adrawing::BrightBlack);
		else
			DrawFilledRectangle(mRect, adrawing::BrightGrey);
	}

	void textbox::drawStr()
	{
		DrawStringLeft(caption, { rect.x1 + PAD, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
		//DrawStringCenter(caption, { rect.x1 + 80, rect.y1 + MAR, rect.x2, rect.y2 }, *fc, myFont);
	}

	void textbox::functionUpdate()
	{
		if (isMouseInRect(mRect) && isMouseClicked()) {
			onInput = true;
			charCounter = 0;
			caption[charCounter] = '\0';
		}
		if (!isMouseInRect(mRect)) {
			onInput = false;
		}
		if (onInput) {

			newKey = getPressedKey();

			if ((lastKey == newKey) && (lastTick + 200 > GetTickCount()))
				return;

			//while (isKeyDown(lastKey))
			//	std::this_thread::sleep_for(std::chrono::milliseconds(10));

			if (charCounter < 20 && (('0' <= newKey && newKey < '9') ||
				('A' <= newKey && newKey < 'Z') || (newKey == VK_SPACE))) {

				caption[charCounter] = newKey;
				charCounter++;
				caption[charCounter] = '\0';
				lastTick = GetTickCount();
			}
			if (newKey == VK_BACK && charCounter > 0) {
				charCounter--;
				caption[charCounter] = '\0';
				lastTick = GetTickCount();
			}
			lastKey = newKey;
		}
	}

	combobox::combobox(char* Caption, int* selected, int argc, char* argv[], window* wPtr, frame* fPtr) :
		guiObj(wPtr, fPtr, controlType::COMBOBOX, Caption, 0, 0, INFRAMEWIDTH, 30),
		actual(selected), maximal(argc), str(argv), isOpen(false)
	{
		win->pGuiObj.push_back(this);
		fPtr->frameObj.push_back(this);
	}

	void combobox::updateRectPosition()
	{
		gFunc->updateInFramePos(this);
	}

	void combobox::drawExtra()
	{
		mRect = { rect.x1 + MAR, rect.y1 + MAR, rect.x2, rect.y2 };
		if (isOpen) {
			drawMenu();
		}
		else {
			// Front
			DrawFilledRectangle(mRect, adrawing::BrightGrey);

			// String
			DrawStringLeft(str[*actual], { rect.x1 + PAD, rect.y1 + MAR, rect.x2, rect.y2 }, adrawing::White, bigFont);
		}
		// End
		//DrawFilledRectangle({mRect.x2 - PAD - BOR, mRect.y2 - PAD - BOR, mRect.x2 + BOR, mRect.y2 + BOR }, White);
		DrawFilledRectangle({ mRect.x2 - PAD, mRect.y2 - PAD, mRect.x2, mRect.y2 }, adrawing::White);
		DrawFilledRectangle({ mRect.x2 - PAD + BOR, mRect.y2 - PAD + BOR, mRect.x2 - BOR, mRect.y2 - BOR }, adrawing::BrightBlack);
		//DrawFilledRectangle({mRect.x2 - PAD, mRect.y2 - PAD, mRect.x2, mRect.y2 }, BrightBlack);
	}

	void combobox::drawMenu()
	{
		int yOffset = 0;
		int mHeight = mRect.y2 - mRect.y1;
		MY_D3DRECT tempRect = { 0,0,0,0 };
		for (int i = 0; i < maximal; i++) {
			tempRect = { mRect.x1, mRect.y1 + i * mHeight, mRect.x2, mRect.y2 + i * mHeight };
			DrawFilledRectangle(tempRect, adrawing::BrightBlack);
			if (i == *actual) {
				DrawFilledRectangle({ tempRect.x1 + 1, tempRect.y1 + 1, tempRect.x2 - 1, tempRect.y2 + 1 }, adrawing::BrightGrey);
			}
			DrawStringLeft(str[i], { tempRect.x1 + MAR, tempRect.y1, tempRect.x2, tempRect.y2 }, adrawing::White, bigFont);
		}
	}

	void combobox::drawStr()
	{
		//DrawStringLeft(caption, { rect.x1 + PAD, rect.y1 + MAR, rect.x2, rect.y2 }, White, myFont);
		//DrawStringCenter(caption, { rect.x1 + 80, rect.y1 + MAR, rect.x2, rect.y2 }, *fc, myFont);
	}

	void combobox::functionUpdate()
	{
		if (isOpen || isMouseInRect(mRect) && isMouseClicked()) {
			isOpen = true;
			if (isMouseClicked()) {
				(*actual)++;
				if (*actual >= maximal)
					* actual = 0;
			}
		}
		if (!isMouseInRect(mRect)) {
			isOpen = false;
		}
	}


	int guiFunc::getTabCount(guiObj* desTab, int& pos)
	{
		int i = 0;
		for (auto& a : gFunc->activeWindow->pGuiObj) {
			if (a->cType == controlType::TAB) {
				if (a == desTab)
					pos = i;
				i++;
			}
		}
		return i;
	}

	void guiFunc::updateInFramePos(guiObj* o)
	{
		int offsettY = 0;
		int offsettX = 0;
		guiObj* btn = nullptr;
		for (auto& a : o->win->pGuiObj) {
			if (a->cType >= controlType::BUTTON) {
				if (o->up == a->up) {
					if (offsettY + o->height > o->up->height) {
						offsettY = 0;
						offsettX = offsettX + o->width;
					}
					if (o == a)
						break;

					offsettY += a->height;
				}
			}
		}

		o->rect = MY_D3DRECT{ o->up->rect.x1 + offsettX, o->up->rect.y1 + offsettY,
			o->up->rect.x1 + offsettX + o->width, o->up->rect.y1 + o->height + offsettY };
	}

	guiFunc::guiFunc() :
		activeWindow(nullptr), activeTitlebar(nullptr), activeRibbon(nullptr), activeTab(nullptr), activeFrame(nullptr),
		gObjCounter(0), setStartTab(true)
	{
	}

	guiFunc::~guiFunc()
	{
		//// At least i tried
		//adrawing::gDraw->smallFont->Release();
		//adrawing::gDraw->middleFont->Release();
	}
	
	
	void guiFunc::init() {
		//if (isInit)
		//	return;
		//D3DXCreateFont(p_Device, 13, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &smallFont);
		//D3DXCreateFont(p_Device, 22, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &middleFont);
		//// Release!!!!
		//isInit = true;
	}

	void guiFunc::drawAll() {

		gFunc->activeWindow->functionUpdate();
		if (*(gFunc->activeWindow->active) == false) {
			return;
		}
			

		if (gFunc->setStartTab) {
			for (auto& a : gFunc->activeWindow->pGuiObj) {
				if (a->cType == controlType::TAB) {
					gFunc->activeTab = dynamic_cast<tab*>(a);
					gFunc->setStartTab = false;
					break;
				}

			}
		}

		std::sort(gFunc->activeWindow->pGuiObj.begin(), gFunc->activeWindow->pGuiObj.end(), [](guiObj* a, guiObj* b) {
			if (a->cType >= controlType::BUTTON)
				return false;
			return a->cType < b->cType;
			});

		// HAHA, need to update that shit
		for (auto& obj : gFunc->activeWindow->pGuiObj) {

			if (obj->cType <= controlType::FRAME) {
				obj->draw();
				obj->functionUpdate();
			}

			if (obj->cType == controlType::FRAME) {
				obj->draw();
				obj->functionUpdate();
				gFunc->activeFrame->draw();
				gFunc->activeFrame->functionUpdate();
			}

			if (obj->cType >= controlType::BUTTON) {
				if (obj->up == gFunc->activeFrame) {
					obj->draw();
					obj->functionUpdate();
				}
			}
		}

		// HAHA, next bullshit thing
		for (auto& obj : gFunc->activeWindow->pGuiObj) {

			if (obj->cType == controlType::COMBOBOX) {
				combobox* myPtr = dynamic_cast<combobox*>(obj);
				if (myPtr->isOpen) {
					obj->draw();
					obj->functionUpdate();
				}
			}
		}

	}
	void setGuiFunc(guiFunc* newGuiFunc)
	{
		gFunc = newGuiFunc;
	}
}