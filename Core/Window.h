#pragma once

#include"./Types.h"

struct WindowInfo
{
	int width, height;
	String title;

    WindowInfo() {};
	WindowInfo(int w, int h, String t): width(w), height(h), title(t) {}	
    WindowInfo(WindowInfo &winInfo) {   *this = winInfo;    };

	WindowInfo& operator = (const WindowInfo &winInfo)
	{
		this->width = winInfo.width;
		this->height = winInfo.height;
		this->title = winInfo.title;

		return *this;
	}
};

namespace Interface
{
    class IApplication;

    class IWindow
    {
        public:
        IWindow(IApplication &targetApp):m_targetApp(targetApp), m_winInfo(800,600,"App") { }
        virtual ~IWindow() {    }
        virtual void SetActive() = 0;
        virtual inline bool ShouldCloseWindow() = 0;
        virtual void SetColor(float r, float g, float b, float a) = 0;
        virtual void SetColor(Color4 &col) = 0;
        virtual void SwapFrameBuffer() = 0;
        inline iVec2 GetWindowSize() { return iVec2(m_winInfo.width, m_winInfo.height); }
        inline WindowInfo& GetWindowInfo() { return m_winInfo; }
        void SetWindowInfo(WindowInfo &wi) {  m_winInfo = wi; }

		IApplication &m_targetApp;
        WindowInfo m_winInfo;
    };
}