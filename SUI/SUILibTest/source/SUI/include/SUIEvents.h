#ifndef SUIEvents_H
#define SUIEvents_H

namespace SUI
{
	class SUIEvents
	{
	public:
		bool ACTION_PERFORMED;

		bool MOUSE_CLICKED;
		bool MOUSE_PRESSED;
		bool MOUSE_RELEASED;

		bool FRAME_MINIMIZED;
		bool FRAME_MAXIMIZED;
		bool FRAME_CLOSE;

		SUIEvents()
		{
			ACTION_PERFORMED = false;

			MOUSE_CLICKED = false;
			MOUSE_PRESSED = false;
			MOUSE_RELEASED = false;

			FRAME_MINIMIZED = false;
			FRAME_MAXIMIZED = false;
			FRAME_CLOSE = false;
		}
	};
}

#endif