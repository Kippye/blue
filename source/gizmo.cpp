#include <gizmo.h>

// empty constructor - default gizmo
Gizmo::Gizmo()
{
	ID = lastID;
	lastID++;
}

// default gizmo with type
Gizmo::Gizmo(GizmoType _type)
{
	type = _type;
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location, GizmoType _type, bool _permanent)
{
	location = _location;
	type = _type;
	permanent = _permanent;
	ID = lastID;
	lastID++;
}

Gizmo::Gizmo(Location _location, Visuals _visuals, GizmoType _type, bool _permanent)
{
	location = _location;
	visuals = _visuals;
	type = _type;
	permanent = _permanent;
	ID = lastID;
	lastID++;
}