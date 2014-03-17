#include "_type/type.dependency.h"
#include "_type/type.event.h"

bool _dependency::get( _eventType type ) const
{
	switch( type )
	{
		case onParentResize:	return this->parent.resize;
		case onParentMove:		return this->parent.move;
		case onParentRestyle:	return this->parent.restyle;
		case onParentShow:
		case onParentHide:		return this->parent.visibility;
		case onParentFocus:		return this->parent.focus;
		case onParentBlur:		return this->parent.blur;
		case onParentAdd:		return this->parent.add;
		case onParentRemove:	return this->parent.remove;
		
		case onChildResize:		return this->child.resize;
		case onChildMove:		return this->child.move;
		case onChildRestyle:	return this->child.restyle;
		case onChildShow:
		case onChildHide:		return this->child.visibility;
		case onChildFocus:		return this->child.focus;
		case onChildBlur:		return this->child.blur;
		case onChildAdd:		return this->child.add;
		case onChildRemove:		return this->child.remove;
		
		case onPreResize:		return this->pre.resize;
		case onPreMove:			return this->pre.move;
		case onPreRestyle:		return this->pre.restyle;
		case onPreShow:
		case onPreHide:			return this->pre.visibility;
		case onPreFocus:		return this->pre.focus;
		case onPreBlur:			return this->pre.blur;
		case onPreAdd:			return this->pre.add;
		case onPreRemove:		return this->pre.remove;
		
		case onPostResize:		return this->post.resize;
		case onPostMove:		return this->post.move;
		case onPostRestyle:		return this->post.restyle;
		case onPostShow:
		case onPostHide:		return this->post.visibility;
		case onPostFocus:		return this->post.focus;
		case onPostBlur:		return this->post.blur;
		case onPostAdd:			return this->post.add;
		case onPostRemove:		return this->post.remove;
		default:				return false;
	}
}

void _dependency::set( _eventType type , bool value )
{
	switch( type )
	{
		case onParentResize:	this->parent.resize = value; break;
		case onParentMove:		this->parent.move = value; break;
		case onParentRestyle:	this->parent.restyle = value; break;
		case onParentShow:
		case onParentHide:		this->parent.visibility = value; break;
		case onParentFocus:		this->parent.focus = value; break;
		case onParentBlur:		this->parent.blur = value; break;
		case onParentAdd:		this->parent.add = value; break;
		case onParentRemove:	this->parent.remove = value; break;
		
		case onChildResize:		this->child.resize = value; break;
		case onChildMove:		this->child.move = value; break;
		case onChildRestyle:	this->child.restyle = value; break;
		case onChildShow:
		case onChildHide:		this->child.visibility = value; break;
		case onChildFocus:		this->child.focus = value; break;
		case onChildBlur:		this->child.blur = value; break;
		case onChildAdd:		this->child.add = value; break;
		case onChildRemove:		this->child.remove = value; break;
		
		case onPreResize:		this->pre.resize = value; break;
		case onPreMove:			this->pre.move = value; break;
		case onPreRestyle:		this->pre.restyle = value; break;
		case onPreShow:
		case onPreHide:			this->pre.visibility = value; break;
		case onPreFocus:		this->pre.focus = value; break;
		case onPreBlur:			this->pre.blur = value; break;
		case onPreAdd:			this->pre.add = value; break;
		case onPreRemove:		this->pre.remove = value; break;
		
		case onPostResize:		this->post.resize = value; break;
		case onPostMove:		this->post.move = value; break;
		case onPostRestyle:		this->post.restyle = value; break;
		case onPostShow:
		case onPostHide:		this->post.visibility = value; break;
		case onPostFocus:		this->post.focus = value; break;
		case onPostBlur:		this->post.blur = value; break;
		case onPostAdd:			this->post.add = value; break;
		case onPostRemove:		this->post.remove = value; break;
		default: break;
	}
}