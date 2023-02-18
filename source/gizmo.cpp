#include <gizmo.h>

// empty constructor - default gizmo
Gizmo::Gizmo()
{
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location)
{
	location = _location;
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location, Visuals _visuals)
{
	location = _location;
	visuals = _visuals;
	ID = lastID;
	lastID++;
}