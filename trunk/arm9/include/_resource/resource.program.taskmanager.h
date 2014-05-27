#ifndef _WIN_PROG_TASKMANAGER_
#define _WIN_PROG_TASKMANAGER_

#include "_type/type.program.c.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.label.h"

class PROG_TaskManager : public _progC
{
	private:
		
		_imageGadget*		currentCpuUsage;
		_imageGadget*		currentMemoryUsage;
		_imageGadget*		histogramCpuUsage;
		_imageGadget*		histogramMemoryUsage;
		_label*				cpuLabel;
		_label*				memoryLabel;
		_u8					pixelsBeforeNextGridLine;
		_timer				updateTimer;
		static const _color	gridColor;
		_s8					lastCpuValueY;
		_s8					lastMemoryValueY;
		
		void			main( _programArgs args );
		void			destruct();
		void			timerHandler();
		static void		drawBorders( _bitmapPort& bP );
		static _bitmap	getLogo();
		
		_callbackReturn usageRefreshHandler( _event event );
		_callbackReturn histogramRefreshHandler( _event event );
		_callbackReturn histogramPrepareHandler( _event event );
		
	public:
		
		PROG_TaskManager();
};

#endif