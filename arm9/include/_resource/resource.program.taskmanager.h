#ifndef _WIN_PROG_TASKMANAGER_
#define _WIN_PROG_TASKMANAGER_

#include <_type/type.program.c.h>
#include <_type/type.timer.h>
#include <_gadget/gadget.image.h>
#include <_gadget/gadget.label.h>

class PROG_TaskManager : public _cProgram
{
	private:
		
		_imageGadget*		currentCpuUsage;
		_imageGadget*		currentMemoryUsage;
		_imageGadget*		histogramCpuUsage;
		_imageGadget*		histogramCpuUsageSub;
		_imageGadget*		histogramMemoryUsage;
		_label*				cpuLabel;
		_label*				memoryLabel;
		_timer				updateTimer;
		static const _color	gridColor;
		_s8					lastCpuValueY;
		_s8					lastCpuSubValueY;
		_s8					lastMemoryValueY;
		_u8					pixelsBeforeNextGridLine;
		
		void			main( _args args );
		void			cleanUp();
		
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