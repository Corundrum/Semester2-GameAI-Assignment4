#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>



int CollisionManager::squaredDistance(const glm::vec2 p1, const glm::vec2 p2)
{
	const int diffOfXs = p2.x - p1.x;
	const int diffOfYs = p2.y - p1.y;
	const auto result = diffOfXs * diffOfXs + diffOfYs * diffOfYs;

	return result;
}

bool CollisionManager::squaredRadiusCheck(GameObject* object1, GameObject* object2)
{
	glm::vec2 P1 = object1->getTransform()->position;
	glm::vec2 P2 = object2->getTransform()->position;
	const int halfHeights = (object1->getHeight() + object2->getHeight()) * 0.5f;

	if (CollisionManager::squaredDistance(P1, P2) < (halfHeights * halfHeights)) {
		if (!object1->getRigidBody()->isColliding) {

			object1->getRigidBody()->isColliding = true;

			switch (object2->getType()) {
			case TARGET:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
				break;
			default:

				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}
}

bool CollisionManager::AABBCheck(GameObject* object1, GameObject* object2)
{
	// prepare relevant variables
	auto p1 = object1->getTransform()->position;
	auto p2 = object2->getTransform()->position;
	const float p1Width = object1->getWidth();
	const float p1Height = object1->getHeight();
	const float p2Width = object2->getWidth();
	const float p2Height = object2->getHeight();

	if (object1->isCentered())
	{
		p1 += glm::vec2(-p1Width * 0.5f, -p1Height * 0.5f);
	}

	if (object2->isCentered())
	{
		p2 += glm::vec2(-p2Width * 0.5f, -p2Height * 0.5f);
	}

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x &&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object1->getRigidBody()->isColliding)
		{
			object1->getRigidBody()->isColliding = true;

			switch (object2->getType())
			{
			case AGENT:
				std::cout << "Collision with SpaceShip!" << std::endl;
				SoundManager::Instance().playSound("boom", 0);
				break;
			default:

				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::AABBCheck(GameObject* object1, SDL_Rect* object2)
{
	// prepare relevant variables
	auto p1 = object1->getTransform()->position;
	
	auto p2 = glm::vec2(object2->x + object2->w / 2, object2->y + object2->h / 2);
	
	const float p1Width = object1->getWidth();
	
	const float p1Height = object1->getHeight();
	
	const float p2Width = object2->w;
	
	const float p2Height = object2->h;

	if (object1->isCentered())
	{
		p1 += glm::vec2(-p1Width * 0.5f, -p1Height * 0.5f);
	}

	p2 += glm::vec2(-p2Width * 0.5f, -p2Height * 0.5f);

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x &&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object1->getRigidBody()->isColliding)
		{
			object1->getRigidBody()->isColliding = true;


			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::AABBCheckWithBuffer(GameObject* object1, GameObject* object2, int buffer)
{
	// prepare relevant variables
	auto p1 = object1->getTransform()->position;
	auto p2 = object2->getTransform()->position;
	const float p1Width = object1->getWidth();
	const float p1Height = object1->getHeight();
	float p2Width = object2->getWidth();
	float p2Height = object2->getHeight();

	if (object1->isCentered())
	{
		p1 += glm::vec2(-p1Width * 0.5f, -p1Height * 0.5f);
	}

	/*if (object2->isCentered())
	{
		p2 += glm::vec2(-p2Width * 0.5f, -p2Height * 0.5f);
	}*/

	// Add buffers.
	p2.x -= (buffer / 2);
	p2.y -= (buffer / 2);
	p2Width += buffer;
	p2Height += buffer;

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x &&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object1->getRigidBody()->isColliding)
		{
			object1->getRigidBody()->isColliding = true;

			switch (object2->getType())
			{
			case AGENT:
				std::cout << "Collision with SpaceShip!" << std::endl;
				SoundManager::Instance().playSound("boom", 0);
				break;
			default:

				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::AABBCheckWithBuffer(GameObject* object1, SDL_Rect* object2, int buffer)
{
	// prepare relevant variables
	auto p1 = object1->getTransform()->position;
	auto p2 = glm::vec2(object2->x + (object2->w / 2), object2->y + (object2->h / 2));
	const float p1Width = object1->getWidth();
	const float p1Height = object1->getHeight();
	float p2Width = object2->w;
	float p2Height = object2->h;

	if (object1->isCentered())
	{
		p1 += glm::vec2(-p1Width * 0.5f, -p1Height * 0.5f);
	}
	p2 += glm::vec2(-p2Width * 0.5f, -p2Height * 0.5f);

	// Add buffers.
	p2.x -= (buffer / 2);
	p2.y -= (buffer / 2);
	p2Width += buffer;
	p2Height += buffer;

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x &&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{
		if (!object1->getRigidBody()->isColliding)
		{
			object1->getRigidBody()->isColliding = true;

			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::lineLineCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 line2_start, const glm::vec2 line2_end)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto x3 = line2_start.x;
	const auto x4 = line2_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto y3 = line2_start.y;
	const auto y4 = line2_end.y;

	// calculate the distance to intersection point
	const auto uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const auto uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}

	return false;
}

bool CollisionManager::lineRectCheck(const glm::vec2 line_start, const glm::vec2 line_end, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const auto x1 = line_start.x;
	const auto x2 = line_end.x;
	const auto y1 = line_start.y;
	const auto y2 = line_end.y;
	const auto rx = rect_start.x;
	const auto ry = rect_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const auto left = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	const auto right = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	const auto top = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	const auto bottom = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

bool CollisionManager::lineRectEdgeCheck(const glm::vec2 line_start, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	bool state = false;

	const auto x1 = line_start.x;
	const auto y1 = line_start.y;

	const auto rx = rect_start.x;
	const auto ry = rect_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// configure the left edge
	const auto leftEdgeStart = glm::vec2(rx, ry);
	const auto leftEdgeEnd = glm::vec2(rx, ry + rh);
	const auto leftEdgeMidPoint = Util::lerp(leftEdgeStart, leftEdgeEnd, 0.5f);

	// configure the right edge
	const auto rightEdgeStart = glm::vec2(rx + rw, ry);
	const auto rightEdgeEnd = glm::vec2(rx + rw, ry + rh);
	const auto rightEdgeMidPoint = Util::lerp(rightEdgeStart, rightEdgeEnd, 0.5f);

	// configure the top edge
	const auto topEdgeStart = glm::vec2(rx, ry);
	const auto topEdgeEnd = glm::vec2(rx + rw, ry);
	const auto topEdgeMidPoint = Util::lerp(topEdgeStart, topEdgeEnd, 0.5f);

	// configure the bottom edge
	const auto bottomEdgeStart = glm::vec2(rx, ry + rh);
	const auto bottomEdgeEnd = glm::vec2(rx + rw, ry + rh);
	const auto bottomEdgeMidPoint = Util::lerp(bottomEdgeStart, bottomEdgeEnd, 0.5f);

	// line / line comparisons
	const auto left = lineLineCheck(glm::vec2(x1, y1), leftEdgeMidPoint, leftEdgeStart, leftEdgeEnd);
	const auto right = lineLineCheck(glm::vec2(x1, y1), rightEdgeMidPoint, rightEdgeStart, rightEdgeEnd);
	const auto top = lineLineCheck(glm::vec2(x1, y1), topEdgeMidPoint, topEdgeStart, topEdgeEnd);
	const auto bottom = lineLineCheck(glm::vec2(x1, y1), bottomEdgeMidPoint, bottomEdgeStart, bottomEdgeEnd);

	// return true if any line collides with the edge
	if (left || right || top || bottom)
	{
		state = true;
	}


	return state;
}

int CollisionManager::minSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
	auto u = line1_end - line1_start;
	auto v = line2_end - line2_start;
	auto w = line1_start - line2_start;
	auto a = Util::dot(u, u);         // always >= 0
	auto b = Util::dot(u, v);
	auto c = Util::dot(v, v);         // always >= 0
	auto d = Util::dot(u, w);
	auto e = Util::dot(v, w);
	auto D = a * c - b * b;					// always >= 0
	float sc, tc;

	// compute the line parameters of the two closest points
	if (D < Util::EPSILON) {					// the lines are almost parallel
		sc = 0.0;
		tc = (b > c ? d / b : e / c);			// use the largest denominator
	}
	else {
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	// get the difference of the two closest points
	auto dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	auto norm = Util::dot(dP, dP);
	return norm;
}

bool CollisionManager::lineAABBCheck(Ship* object1, GameObject* object2)
{
	const auto lineStart = object1->getTransform()->position;
	const auto lineEnd = object1->getTransform()->position + object1->getCurrentDirection() * 100.0f;
	// aabb
	const auto boxWidth = object2->getWidth();
	const int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object2->getHeight();
	const int halfBoxHeight = boxHeight * 0.5f;
	const auto boxStart = object2->getTransform()->position - glm::vec2(halfBoxWidth, halfBoxHeight);

	if (lineRectCheck(lineStart, lineEnd, boxStart, boxWidth, boxHeight))
	{
		switch (object2->getType()) {
		case TARGET:
			std::cout << "Collision with Obstacle!" << std::endl;
			SoundManager::Instance().playSound("yay", 0);

			break;
		default:

			break;
		}

		return true;
	}

	return false;
}

int CollisionManager::circleAABBsquaredDistance(const glm::vec2 circle_centre, int circle_radius, const glm::vec2 box_start, const int box_width, const int box_height)
{
	auto dx = std::max(box_start.x - circle_centre.x, 0.0f);
	dx = std::max(dx, circle_centre.x - (box_start.x + box_width));
	auto dy = std::max(box_start.y - circle_centre.y, 0.0f);
	dy = std::max(dy, circle_centre.y - (box_start.y + box_height));

	return (dx * dx) + (dy * dy);
}

bool CollisionManager::circleAABBCheck(GameObject* object1, GameObject* object2)
{
	// circle
	const auto circleCentre = object1->getTransform()->position;
	const int circleRadius = std::max(object1->getWidth() * 0.5f, object1->getHeight() * 0.5f);
	// aabb
	const auto boxWidth = object2->getWidth();
	int halfBoxWidth = boxWidth * 0.5f;
	const auto boxHeight = object2->getHeight();
	int halfBoxHeight = boxHeight * 0.5f;

	const auto boxStart = object2->getTransform()->position - glm::vec2(boxWidth * 0.5f, boxHeight * 0.5f);

	if (circleAABBsquaredDistance(circleCentre, circleRadius, boxStart, boxWidth, boxHeight) <= (circleRadius * circleRadius))
	{
		if (!object1->getRigidBody()->isColliding)
		{

			object1->getRigidBody()->isColliding = true;

			switch (object2->getType())
			{
			case AGENT:
			{
				std::cout << "Collision with SpaceShip!" << std::endl;
				SoundManager::Instance().playSound("yay", 0);
			}


			break;
			default:

				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object1->getRigidBody()->isColliding = false;
		return false;
	}

	return false;
}

bool CollisionManager::pointRectCheck(const glm::vec2 point, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const float topLeftX = rect_start.x - rect_width * 0.5;
	const float topLeftY = rect_start.y - rect_height * 0.5;
	const auto width = rect_width;
	const auto height = rect_height;

	if (point.x > topLeftX &&
		point.x < topLeftX + width &&
		point.y > topLeftY &&
		point.y < topLeftY + height)
	{
		return true;
	}
	return false;
}

// assumptions - the list of objects are stored so that they are facing the target and the target is loaded last
bool CollisionManager::LOSCheck(Agent* agent, glm::vec2 end_point, const std::vector<DisplayObject*>& objects, DisplayObject* target)
{
	const auto start_point = agent->getTransform()->position;

	// Check collision with obstacles first.
	for (auto object : objects)
	{
		auto objectOffset = glm::vec2(object->getWidth() * 0.5f, object->getHeight() * 0.5f);
		if (lineRectCheck(start_point, end_point, object->getTransform()->position - objectOffset,
			object->getWidth(), object->getHeight()))
		{
			return false;
		}
	}
	// Now check if hitting target.
	auto targetOffset = glm::vec2(target->getWidth() * 0.5f, target->getHeight() * 0.5f);
	if (lineRectCheck(start_point, end_point, target->getTransform()->position - targetOffset,
		target->getWidth(), target->getHeight()))
	{
		return true;
	}
	// Nothing hit.
	return false;
}

bool CollisionManager::LOSCheck(Agent* agent, glm::vec2 end_point, const std::vector<DisplayObject*>& objects, SDL_Rect* target)
{
	const auto start_point = agent->getTransform()->position;

	// Check collision with obstacles first.
	for (auto object : objects)
	{
		auto objectOffset = glm::vec2(object->getWidth() * 0.5f, object->getHeight() * 0.5f);
		if (lineRectCheck(start_point, end_point, object->getTransform()->position - objectOffset,
			object->getWidth(), object->getHeight()))
		{
			return false;
		}
	}
	// Now check if hitting target.
	auto targetOffset = glm::vec2(target->w * 0.5f, target->h * 0.5f);
	if (lineRectCheck(start_point, end_point, glm::vec2(target->x + target->w * 0.5, target->y + target->h * 0.5) - targetOffset,
		target->w, target->h))
	{
		return true;
	}
	// Nothing hit.
	return false;
}

void CollisionManager::rotateAABB(GameObject* object1, const float angle)
{
	// create an array of vec2s using right winding order (TL, TR, BR, BL)
	glm::vec2 points[4];

	const glm::vec2 position = object1->getTransform()->position;

	const auto width = object1->getRigidBody()->bounds.x;
	const float half_width = width * 0.5f;
	const auto height = object1->getRigidBody()->bounds.y;
	const float half_height = height * 0.5f;

	// check if object1 is centered
	if (object1->isCentered())
	{
		// compute points
		points[0] = glm::vec2(position.x - half_width, position.y - half_height);
		points[1] = glm::vec2(position.x + half_width, position.y - half_height);
		points[2] = glm::vec2(position.x + half_width, position.y + half_height);
		points[3] = glm::vec2(position.x - half_width, position.y + half_height);
	}
	else
	{
		// compute points
		points[0] = glm::vec2(position.x, position.y);
		points[1] = glm::vec2(position.x + width, position.y);
		points[2] = glm::vec2(position.x + width, position.y + height);
		points[3] = glm::vec2(position.x, position.y + height);
	}

	// rotate each point by the desired angle
	for (int i = 0; i < 4; ++i)
	{
		points[i] = Util::rotatePoint(points[i], angle, position);
	}

	// initialize extents
	auto top_left = glm::vec2(INFINITY, INFINITY);
	auto bot_right = glm::vec2(-INFINITY, -INFINITY);

	// compute extents (top_left and bot_right)
	for (const auto point : points)
	{
		if (top_left.x > point.x)
		{
			top_left.x = point.x;
		}

		if (top_left.y > point.y)
		{
			top_left.y = point.y;
		}

		if (bot_right.x < point.x)
		{
			bot_right.x = point.x;
		}

		if (bot_right.y < point.y)
		{
			bot_right.y = point.y;
		}
	}

	// compute new width and height values for object1
	object1->setWidth(static_cast<int>(bot_right.x - top_left.x));
	object1->setHeight(static_cast<int>(bot_right.y - top_left.y));
}


CollisionManager::CollisionManager()
= default;


CollisionManager::~CollisionManager()
= default;
