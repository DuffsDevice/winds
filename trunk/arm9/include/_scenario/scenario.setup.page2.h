#ifndef _WIN_SC_SETUP_PAGE2_
#define _WIN_SC_SETUP_PAGE2_

#include "_type/type.h"
#include "_type/type.view.h"
#include "_type/type.system.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.select.h"

class _scSetupPage2 : public _view
{
	private:
	
		_label*		title;
		_label*		line1;
		_label*		line2;
	
	public:
		
		void create( _gadget* viewParent )
		{
			// Welcome label
			this->title = new _label( 13 , 28 , ignore , ignore , _system::getLocalizedString("lbl_welcome_to_winds") );
			this->title->setColor( RGB( 30 , 30 , 30 ) );
			this->title->setFont( _system::getFont( "ArialBlack13" ) );
			
			// Few steps to set up label
			this->line1 = new _label( 20 , 60 , ignore , ignore , _system::getLocalizedString("txt_few_step_setup") );
			this->line1->setColor( RGB( 30 , 30 , 30 ) );
			
			// Few steps to set up label (line 2)
			this->line2 = new _label( 20 , 70 , ignore , ignore , _system::getLocalizedString("txt_few_step_setup_2") );
			this->line2->setColor( RGB( 30 , 30 , 30 ) );			
			
			viewParent->addChild( this->title );
			viewParent->addChild( this->line1 );
			viewParent->addChild( this->line2 );
		}
		
		bool destroy( _gadget* viewParent )
		{
			delete this->title;
			delete this->line1;
			delete this->line2;
			
			// Switching the view is allowed
			return true;
		}
};

#endif