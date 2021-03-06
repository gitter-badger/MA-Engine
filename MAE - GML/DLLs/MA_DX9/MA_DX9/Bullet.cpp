#include "Bullet.h"

MABullet* mabullet = nullptr;

double MABullet::addShape(btCollisionShape* Shape) {
	Shapes[ShapeCount] = Shape;
	Shape->setUserIndex(ShapeCount);
	ShapeCount++;
	return ShapeCount - 1;
}

double MABullet::addConstraint(btTypedConstraint* Constraint) {
	Constraints[ConstraintCount] = Constraint;
	Constraint->setUserConstraintId(ConstraintCount);
	ConstraintCount++;
	return ConstraintCount - 1;
}

bool MABullet::destroyWorld()
{
	if (!worldExists()) return false;
	for (auto i : Constraints)
	{
		World->removeConstraint(i.second);
		delete i.second;
	}
	Constraints.clear();
	for (auto i : Bodies)
	{
		World->removeCollisionObject(i.second->Body);
		delete i.second->Body->getMotionState();
		delete i.second->Body;
		delete i.second;
	}
	Bodies.clear();
	delete World;
	delete Solver;
	delete Dispatcher;
	delete CollisionConfiguration;
	delete Broadphase;
	if (DebugDrawer) {
		delete DebugDrawer;
		DebugDrawer = nullptr;
	}
	World = nullptr;
	return true;
}

btVector3 MABullet::toEuler(btMatrix3x3 &tm)
{
	btScalar x, y, z = 0;

	if (tm.getRow(2).x() < 0.999)
	{
		if (tm.getRow(2).x() > -0.999)
			x = atan2(-tm.getRow(2).y(), tm.getRow(2).z());
		else
			x = -atan2(tm.getRow(0).y(), tm.getRow(1).y());
	}
	else
		x = atan2(tm.getRow(0).y(), tm.getRow(1).y());

	if (tm.getRow(2).x() < 0.999)
	{
		if (tm.getRow(2).x() > -0.999)
			y = asin(tm.getRow(2).x());
		else
			y = -SIMD_HALF_PI;
	}
	else
		y = SIMD_HALF_PI;

	if (tm.getRow(2).x()< +0.999 && tm.getRow(2).x() > -0.999)
		z = atan2(-tm.getRow(1).x(), tm.getRow(0).x());
	else z = 0.f;

	return btVector3(x*SIMD_DEGS_PER_RAD, y*SIMD_DEGS_PER_RAD, z*SIMD_DEGS_PER_RAD);
}

MABullet::~MABullet()
{
	for (auto i : Shapes)
	{
		btBvhTriangleMeshShape* trimesh = dynamic_cast<btBvhTriangleMeshShape*>(i.second);
		if (trimesh) delete trimesh->getMeshInterface();
		delete i.second;
	}
	destroyWorld();
}
