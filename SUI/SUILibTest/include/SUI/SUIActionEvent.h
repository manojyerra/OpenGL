#ifndef SUIActionEvent_H
#define SUIActionEvent_H

namespace SUI
{
	class SUIActionEvent
	{
		void* _com;

	public:
		SUIActionEvent(void* com)
		{
			_com = com;
		}

		void* GetComponent() { return _com; }
	};
}

#endif