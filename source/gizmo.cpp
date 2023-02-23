#include <gizmo.h>

// empty constructor - default gizmo
Gizmo::Gizmo()
{
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location, bool _permanent)
{
	location = _location;
	permanent = _permanent;
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location, Visuals _visuals, bool _permanent)
{
	location = _location;
	visuals = _visuals;
	permanent = _permanent;
	ID = lastID;
	lastID++;
}