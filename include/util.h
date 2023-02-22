#pragma once

#include <glm.hpp>
#include <stdio.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#define GLM_ENABLE_EXPERIMENTAL

// constants
const int MAX_TAGS = 5;

class mymath
{
	public:
		static glm::vec2 floor_to_grid(glm::vec2 pos)
		{
			pos.x = floor(pos.x);
			pos.y = floor(pos.y);
			return pos;
		}
		static glm::vec4 floor_to_grid(glm::vec4 pos)
		{
			pos.x = floor(pos.x);
			pos.y = floor(pos.y);
			return pos;
		}
		static glm::vec2 round_to_grid(glm::vec2 pos)
		{
			pos.x = round(pos.x);
			pos.y = round(pos.y);
			return pos;
		}
		static glm::vec4 round_to_grid(glm::vec4 pos)
		{
			pos.x = round(pos.x);
			pos.y = round(pos.y);
			return pos;
		}
		static double rad(double deg)
		{
			if (deg == 0 || deg == 360)
				return 0;
			
			return deg * (glm::pi<double>() / 180);
		}
		static double deg(double rad)
		{
			if (rad == 0)
				return 0;

			return rad / glm::pi<double>() * 180;
		}
};

class Bounding_Box
{
	public:
		glm::vec2 minimum, maximum;
		glm::vec2 size;

	public:
		Bounding_Box()
		{
			size = glm::vec2(1.0f);
		}

		Bounding_Box(glm::vec2 size)
		{
			update_size(size);
		}

		void update_size(glm::vec2 size)
		{
			this->size = size;
			// handle negative sizes correctly as well
			minimum.x = size.x > 0 ? 0.0f : size.x;
			minimum.y = size.y > 0 ? 0.0f : size.y;
			maximum.x = size.x > 0 ? size.x : 0.0f;
			maximum.y = size.y > 0 ? size.y : 0.0f;
		}

		bool contains_position(glm::vec4 &boxPos, glm::vec4 &sourcePos)
		{
			return sourcePos.x >= (boxPos.x) && sourcePos.y >= (boxPos.y)
			&& sourcePos.x <= (maximum.x + boxPos.x) && sourcePos.y <= (maximum.y + boxPos.y);
		}

		bool overlaps(glm::vec4 &boxPos, glm::vec4 &sourcePos, Bounding_Box &sourceBox)
		{
			return
			(
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum + glm::vec2(0.01f), 0.0f, 1.0f)) || // contains BL
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum.x + sourceBox.size.x - 0.01f, sourceBox.minimum.y + 0.01f, 0.0f, 1.0f)) || // BR
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum.x + 0.01f, sourceBox.minimum.y + sourceBox.size.y - 0.01f, 0.0f, 1.0f)) || // TL
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.maximum - glm::vec2(0.01f), 0.0f, 1.0f)) // TR
			);
		}

		bool contains(glm::vec4 &boxPos, glm::vec4 &sourcePos, Bounding_Box &sourceBox)
		{
			return
			(
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum + glm::vec2(0.01f), 0.0f, 1.0f)) && // contains BL
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum.x + sourceBox.size.x - 0.01f, sourceBox.minimum.y + 0.01f, 0.0f, 1.0f)) && // contains BR
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.minimum.x + 0.01f, sourceBox.minimum.y + sourceBox.size.y - 0.01f, 0.0f, 1.0f)) && // contains TL
				contains_position(boxPos, sourcePos + glm::vec4(sourceBox.maximum - glm::vec2(0.01f), 0.0f, 1.0f)) // containts TR
			);
		}
};
