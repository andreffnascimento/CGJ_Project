#ifndef __mm3d_entities_world_cheeriosTrack__
#define __mm3d_entities_world_cheeriosTrack__


#include "MicroMachines3D/common/include.h"




class CheeriosTrack : public SceneEntity
{

public:
	CheeriosTrack(Scene* scene)
		: SceneEntity(scene->createEntity("CheeriosTrack"))
	{
		GroupComponent& group = addComponent<GroupComponent>();
		_createTrack(scene, group);
	}




private:
	void _createTrack(Scene* scene, GroupComponent& group)
	{
		MeshComponent mesh = MeshComponent(createTorus(CHEERIO_INNER_RADIUS, CHEERIO_OUTER_RADIUS, CHEERIO_RINGS, CHEERIO_SIDES), CHEERIO_MATERIAL);
		_createCheerio(scene, group, mesh, 0, 0.0f, 0.0f);
	}


	void _createCheerio(Scene* scene, GroupComponent& group, const MeshComponent& mesh, unsigned int id, float xPos, float yPos)
	{
		constexpr RigidbodyComponent::RigidbodyType rigidbodyType = RigidbodyComponent::RigidbodyType::DYNAMIC;
		constexpr float mass = 2000.0f;
		constexpr float drag = 1000.0f;
		constexpr float angularDrag = 1000.0f;

		Entity cheerio = group.addNewEntity(scene, *this, "cheerio_" + std::to_string(id));
		cheerio.addComponent<MeshComponent>(mesh);

		RigidbodyComponent& rigidbody = cheerio.addComponent<RigidbodyComponent>(rigidbodyType, mass, drag, angularDrag);
		AABBColliderComponent& collider = cheerio.addComponent<AABBColliderComponent>(rigidbody, Coords3f({ CHEERIO_SIZE.x, CHEERIO_OUTER_RADIUS * 3.0f, CHEERIO_SIZE.z }));
		collider.setFixedBoundingBox(true);
		collider.setRestitutionCocoefficient(0.1f);

		Transform::scaleTo(cheerio, CHEERIO_SIZE);
		Transform::translate(cheerio, { xPos, CHEERIO_OUTER_RADIUS, yPos });
	}

};


#endif // !__mm3d_entities_world_cheeriosTrack__