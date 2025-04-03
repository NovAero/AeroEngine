#include "AEPCH.h"

namespace Win32 {

    VOID Caption::AddCaptionButton(CaptionButton* button)
    {
        if (button) {
            m_CaptionButtons.push_back(button);
        }
    }

}