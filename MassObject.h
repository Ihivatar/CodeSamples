#pragma once

#include <SFML/Graphics.hpp>

//#define REALISTIC
const float mass_scale = 25.0f;

#ifdef REALISTIC
const double G = 6.67300E-11;
//const double G = 6.67300E-3;
#else
const double G = 0.06673f;
#endif

sf::Vector2f operator *(sf::Vector2f A, sf::Vector2f B)
{
	sf::Vector2f Result;
	Result.x = A.x*B.x;
	Result.y = A.y*B.y;

	return Result;
}

float Dot(sf::Vector2f A, sf::Vector2f B)
{
	float Result = A.x*B.x + A.y*B.y;

	return Result;
}

void Clamp(sf::Vector2f& A, float min, float max)
{
	if (A.x > max)
		A.x = max;
	else if (A.x < min)
		A.x = min;
	if (A.y > max)
		A.y = max;
	else if (A.y < min)
		A.y = min;
}

float Length(sf::Vector2f& value)
{
	float Result = sqrtf((value.x*value.x) + (value.y*value.y));

	return Result;
}

sf::Vector2f Normalize(sf::Vector2f& direction)
{
	sf::Vector2f Result = direction;
	float distance = Length(Result);
	Result.x /= distance;
	Result.y /= distance;

	return Result;
}

struct MassObject
{
	MassObject(float x, float y, float mass)
	{
		pos.x = x;
		pos.y = y;
		this->mass = mass;
#ifdef REALISTIC
		obj.setRadius(sqrtf(mass));
#else
		obj.setRadius(mass);
#endif
		this->mass *= mass_scale;
		obj.setOrigin(obj.getLocalBounds().width/2.0f, obj.getLocalBounds().height/2.0f);
		obj.setPosition(pos);
		obj.setFillColor(sf::Color::Red);
		vel = sf::Vector2f(0.0f, 0.0f);
		acc = sf::Vector2f(0.0f, 0.0f);
		prev_vel = sf::Vector2f(0.0f, 0.0f);
		dir = sf::Vector2f(0.0f, 0.0f);
	}

	float GetDistance(MassObject otherObject)
	{
		sf::Vector2f dist;
		dist.x = pos.x - otherObject.pos.x;
		dist.y = pos.y - otherObject.pos.y;
		float distance = sqrtf((dist.x*dist.x) + (dist.y*dist.y));

		return distance;
	}

	double GravitationalForce(MassObject otherObject)
	{
		float r_squared = GetDistance(otherObject);
		float Result = 0.0f;
		if (r_squared > 0.01f)
			Result = ((G * mass * otherObject.mass) / r_squared);
		else
			((G * mass * otherObject.mass) / 1.0f);

		return Result;
	}

	void CalculateDirectionalForce(MassObject other)
	{
		double force = GravitationalForce(other);
		float length = GetDistance(other);
		dir = other.pos - pos;
		sf::Vector2f dir_normalized = Normalize(dir);
		dir_normalized.x *= force;
		dir_normalized.y *= force;
	}

	void AddForce(float x, float y, float delta)
	{
		acc.x = (x / mass) * delta;
		acc.y = (y / mass) * delta;
		vel += acc;
		sf::Vector2f new_pos = (acc*acc)/2.0f + prev_vel + pos;	// 1/2 a^2 + old vel + pos
		prev_vel = vel;
		obj.setPosition(new_pos);
		pos = new_pos;
	}

	bool IsColliding(MassObject other)
	{
		float radiusAB = obj.getRadius() + other.obj.getRadius();

		if (radiusAB >= GetDistance(other))
			return true;
		return false;
	}

	sf::Vector2f GetCollisionNormal(MassObject other)
	{
		sf::Vector2f Result = pos - other.pos;

		return Result;
	}

	void ResolveCollision(MassObject& other)
	{
		sf::Vector2f dist_vec = other.pos - pos;
		sf::Vector2f dist_dir = Normalize(dist_vec);
		float distance = Length(dist_vec);
		float total_radius = obj.getRadius() + other.obj.getRadius();

		float collision_depth = total_radius - distance;

		if (collision_depth > 0)
		{
			if (mass < other.mass)
				pos -= dist_dir * collision_depth;
			else
				other.pos += dist_dir * collision_depth;

			float aci = Dot(vel, dist_dir);
			float bci = Dot(other.vel, dist_dir);

			float acf = (aci * (mass - other.mass) + 2 * other.mass * bci) / (mass * other.mass);
			float bcf = (bci * (other.mass - mass) + 2 * mass * aci) / (mass + other.mass);

			vel += (acf - aci) * dist_dir;
			other.vel += (bcf - bci) * dist_dir;
		}
	}

	sf::Vector2f pos;
	sf::Vector2f prev_vel;
	sf::Vector2f vel;
	sf::Vector2f acc;
	sf::Vector2f dir;

	float mass;

	sf::CircleShape obj;
};
