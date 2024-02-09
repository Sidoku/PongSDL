#ifndef CONTACT_CPP
#define CONTACT_CPP

#include "CollisionType.cpp"

struct Contact
{
	CollisionType type;
	float penetration;
};
#endif // CONTACT_CPP