#ifndef _WIN_G_TREEVIEW_
#define _WIN_G_TREEVIEW_

#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollarea.h"

class _treeViewNode
{
	private:
		
		friend class _treeView;
		
		//! Node Value
		_gadget*				value;
		
		//! Child Nodes...
		_vector<_treeViewNode>	children;
		
		//! Constructs tree structure below inside the supplied _treeView gadget
		void addTo( _gadget* dest , _u32 curLevel = 1 );
	
	public:
		
		//! Ctor
		_treeViewNode( _gadget*&& value , _initializerList<_treeViewNode> children ) :
			value( value )
			, children( move(children) )
		{}
		
		//! Default Ctor
		_treeViewNode( _gadget*&& value = nullptr ) :
			value( value )
		{}
		
		//! Copy Ctor
		_treeViewNode( const _treeViewNode& node ) :
			_treeViewNode( move(const_cast<_treeViewNode&>(node)) )
		{}
		
		//! Move Ctor
		_treeViewNode( _treeViewNode&& node ) :
			value( node.value )
			, children( move(node.children) )
		{
			node.value = nullptr;
		}
		
		//! Set Children
		void setChildren( _vector<_treeViewNode> children ){
			this->children = move(children);
		}
		
		//! Set Value
		void setValue( _gadget*&& value ){
			if( this->value )
				delete this->value;
			this->value = value;
		}
		
		//! Dtor
		~_treeViewNode(){
			if( this->value ) delete this->value;
		}
};

class _treeView : public _scrollArea
{
	private:
		
		friend class _treeViewNode;
		
		//! Methods that handle changes in the dom tree
		static _callbackReturn nodePreVisibilityHandler( _event event );
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn nodeClickHandler( _event event );
		static _callbackReturn nodeAddRemoveHandler( _event event );
		
		//! Get cooresponding X-Coordinate to tree level
		static _coord getLevelX( _int level ){
			return abs( level ) * 8 - 7;
		}
	
	public:
		
		//! Gets the root gadget of the supplied gaget
		static _gadget*		getRootOf( _gadget* gadget );
		static _gadget*		getNextVisibleOrHigherLevel( _gadget* current );
		static bool			hasChildren( _gadget* root );
		
		//! Ctor
		_treeView(
			_optValue<_coord> x , _optValue<_coord> y
			, _optValue<_length> width , _optValue<_length> height
			, _vector<_treeViewNode> treeStructure
			, _scrollType scrollTypeX = _scrollType::meta, _scrollType scrollTypeY = _scrollType::meta
			, _style&& style = _style()
		);
		
		//! replace the tree structure; All previously allocated nodes get destroyed
		void setTree( _vector<_treeViewNode> treeStructure );
		
		//! Dtor
		virtual ~_treeView(){
			this->removeChildren( true );
		}
};

#endif