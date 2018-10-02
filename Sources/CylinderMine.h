#ifndef MINE_CYLINDER_H
#define MINE_CYLINDER_H

#include "Globals.h"
#include <vector>
#include "Primitive.h"

class NOTlinder : public NOTprimitive
{

public:

	NOTlinder(float radius, unsigned int sector_count,unsigned int stack_count, unsigned int height);

};
#endif