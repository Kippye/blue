#pragma once

#include <util.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <editor_tile.h>

class Location;
class Visuals;

typedef int GizmoType;

enum GizmoType_
{
	GizmoType_None = 0,
	GizmoType_Indicator = 1 << 0,
	GizmoType_ResizeDragger = 1 << 1,
	GizmoType_MoveDragger = 1 << 2
};

class Gizmo
{
	private:
		static inline unsigned int lastID = 0;
	public:
		unsigned int ID = 0;
		GizmoType type = GizmoType_Indicator;
		bool permanent = false;
		Location location;
		Visuals visuals;
		Gizmo();
		Gizmo(GizmoType _type);
		Gizmo(Location _location, GizmoType _type = GizmoType_Indicator, bool _permanent = false);
		Gizmo(Location _location, Visuals _visuals, GizmoType _type = GizmoType_Indicator, bool _permanent = false);
};

// operators
inline bool operator==(const Gizmo& gizmo1, const Gizmo& gizmo2){ return gizmo1.ID == gizmo2.ID; }
inline bool operator!=(const Gizmo& gizmo1, const Gizmo& gizmo2){ return !(gizmo1 == gizmo2); }
