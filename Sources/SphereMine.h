#ifndef MINE_SPHERE_H
#define MINE_SPHERE_H

#include "Globals.h"
#include <vector>
#include "Primitive.h"

class NOTphere : public NOTprimitive
{
public:

	NOTphere(float radius, unsigned int sector_count, unsigned int stack_count);
};
#endif