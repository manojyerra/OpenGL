#ifndef SUIActionListener_H
#define SUIActionListener_H

#include "SUIActionEvent.h"

namespace SUI
{
	class SUIActionListener
	{
	public:
		virtual void actionPerformed(SUIActionEvent SUIActionEvent) = 0;
	};
}

#endif