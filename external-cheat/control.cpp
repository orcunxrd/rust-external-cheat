#include "control.h"

namespace Cheat {


	void Update() {
		while (true) {

			if (Globals::tWnd == GetForegroundWindow()) {

			}

			if (GetAsyncKeyState(0x2D)) {
				if (!Globals::bShowMenu) {
					long winlong = GetWindowLong(Globals::hWnd, GWL_EXSTYLE);

					if (winlong != WS_EX_LAYERED | WS_EX_TOPMOST)
						SetWindowLong(Globals::hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}

				if (Globals::bShowMenu) {
					long winlong = GetWindowLong(Globals::hWnd, GWL_EXSTYLE);

					if (winlong != WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT)
						SetWindowLong(Globals::hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
				}
				Globals::bShowMenu = !Globals::bShowMenu;

				while (GetAsyncKeyState(0x2D)) {}
			}

			//CHANGEDTIME
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	}
}


