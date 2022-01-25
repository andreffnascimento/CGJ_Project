#ifndef __engine_math_l3dBillboard__
#define __engine_math_l3dBillboard__

#include "engine/utils/coords.h"

void l3dBillboardCylindricalBegin(const Coords3f& cameraPos, const Coords3f& objectPos);
void l3dBillboardSphericalBegin(const Coords3f& cameraPos, const Coords3f& objectPos);
void BillboardCheatSphericalBegin(float xScale, float yScale);
void BillboardCheatCylindricalBegin(float xScale, float yScale);

#endif