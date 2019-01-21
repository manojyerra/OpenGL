#ifndef SUIScroller_H
#define SUIScroller_H

#include "SUIRect.h"

namespace SUI
{
	class SUIScroller
	{
	private:
		SUIRect _bg;
		SUIRect _bar;

		bool _enableScroller;
		float _scrollPercent;

		float _viewLength;
		float _totViewLength;

	public:
		SUIScroller(float x, float y, float w, float h);
		~SUIScroller();

		void Calc();
		bool IsScrollEnabled();
		void SetViewLength(float viewLength);
		void SetTotalViewLength(float totViewLength);

		float GetX();
		float GetY();
		float GetW();

		bool Contains(float x, float y);
		void SetScrollPercent(float scrollPercent);
		float GetScrollPercent();
		void SetBounds(float x, float y, float w, float h);
		void Move(float dx, float dy);

		void Update();
		void Draw();
	};
}

#endif