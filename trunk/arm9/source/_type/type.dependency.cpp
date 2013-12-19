#include "_type/type.dependency.h"
#include "_type/type.event.h"

bool _dependency::get( _eventType type ) const
{
	switch( type )
	{
		case onParentResize:	return this->parent.resize;
		case onParentMove:		return this->parent.move;
		case onParentRestyle:	return this->parent.restyle;
		case onParentVisibility:return this->parent.visibility;
		case onParentSet:		return this->parent.set;
		case onChildResize:		return this->child.resize;
		case onChildMove:		return this->child.move;
		case onChildRestyle:	return this->child.restyle;
		case onChildVisibility:	return this->child.visibility;
		case onChildSet:		return this->child.set;
		case onPreResize:		return this->pre.resize;
		case onPreMove:			return this->pre.move;
		case onPreRestyle:		return this->pre.restyle;
		case onPreVisibility:	return this->pre.visibility;
		case onPreSet:			return this->pre.set;
		case onPostResize:		return this->post.resize;
		case onPostMove:		return this->post.move;
		case onPostRestyle:		return this->post.restyle;
		case onPostVisibility:	return this->post.visibility;
		case onPostSet:			return this->post.set;
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
		case onParentVisibility:this->parent.visibility = value; break;
		case onParentSet:		this->parent.set = value; break;
		case onChildResize:		this->child.resize = value; break;
		case onChildMove:		this->child.move = value; break;
		case onChildRestyle:	this->child.restyle = value; break;
		case onChildVisibility:	this->child.visibility = value; break;
		case onChildSet:		this->child.set = value; break;
		case onPreResize:		this->pre.resize = value; break;
		case onPreMove:			this->pre.move = value; break;
		case onPreRestyle:		this->pre.restyle = value; break;
		case onPreVisibility:	this->pre.visibility = value; break;
		case onPreSet:			this->pre.set = value; break;
		case onPostResize:		this->post.resize = value; break;
		case onPostMove:		this->post.move = value; break;
		case onPostRestyle:		this->post.restyle = value; break;
		case onPostVisibility:	this->post.visibility = value; break;
		case onPostSet:			this->post.set = value; break;
		default: break;
	}
}