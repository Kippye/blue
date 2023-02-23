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

class Gizmo
{
	private:
		static inline unsigned int lastID = 0;
	public:
		unsigned int ID = 0;
		bool permanent = false;
		Location location;
		Visuals visuals;
		Gizmo();
		Gizmo(Location _location, bool _permanent = false);
		Gizmo(Location _location, Visuals _visuals, bool _permanent = false);
};

// operators
inline bool operator==(const Gizmo& gizmo1, const Gizmo& gizmo2){ return gizmo1.ID == gizmo2.ID; }
inline bool operator!=(const Gizmo& gizmo1, const Gizmo& gizmo2){ return !(gizmo1 == gizmo2); }
