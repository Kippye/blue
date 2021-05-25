#pragma once

#include <glm.hpp>

// shits weird
//~ template <class T>
//~ class Range
//~ {
	//~ public:
		//~ T minimum, maximum;

		//~ Range (T min, T max)
		//~ {
			//~ if (is_valid())
			//~ {
				//~ minimum = min;
				//~ maximum = max;
			//~ }
		//~ }

		//~ bool is_valid()
		//~ {
			//~ return minimum - maximum <= 0;
		//~ }

		//~ bool contains_value(T value)
		//~ {
			//~ return minimum - value <= 0 && value - maximum <= 0;
		//~ }

		//~ bool is_inside_range(Range range)
		//~ {
			//~ return is_valid() && range.is_valid() && range.contains_value(minimum) && range.contains_value(maximum)
		//~ }

		//~ bool contains_range(Range range)
		//~ {
			//~ return is_valid() && range.is_valid() && contains_value(range.minimum) && contains_value(range.maximum);
		//~ }
//~ };

class Bounding_Box
{
	glm::vec4 minimum, maximum;

	public:
		Bounding_Box()
		{
			minimum = glm::vec4(1.0f);
			maximum = glm::vec4(1.0f);
		}

		Bounding_Box(glm::vec4 min, glm::vec4 max)
		{
			minimum = min;
			maximum = max;
		}

		bool contains_position(glm::vec4 pos)
		{
			return pos.x >= minimum.x && pos.y >= minimum.y
			&& pos.x <= maximum.x && pos.y <= maximum.y;
		}
};
