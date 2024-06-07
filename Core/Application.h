#pragma once

#include"./Types.h"
#include"./Window.h"

// using same info mode for both, contains width, height and the title of window
typedef WindowInfo ApplicationInfo;

namespace Interface
{
    class IApplication
    {
        public:
        IApplication() {}

        virtual void Initialize() = 0;
        virtual void Loop() = 0;
		inline ApplicationInfo& GetAppInfo() {   return m_window->GetWindowInfo();    }

        IWindow *m_window;
    };
}