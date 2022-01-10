#ifndef __mm3d_entities_world_cheeriosTrack__
#define __mm3d_entities_world_cheeriosTrack__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/collisionResolver/cheerioCollisionResolver.h"




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
		mesh.setTextureMode(Texture::TextureMode::MODULATE_DIFFUSE);
		mesh.addTexture(Renderer::create2dTexture("src/MicroMachines3D/textures/cheerios/tire.jpg"));

		int currentId = 0;
		float drawX = 18.0f;
		float drawY = -5.0f;

		//HomeStraight
		_createStraight(scene, group, mesh, currentId, 57, 1, 0, drawX - TRACK_WIDTH, drawY);
		_createStraight(scene, group, mesh, currentId, 44, 1, 0, drawX, drawY - TRACK_WIDTH);

		//Turn1 - Uses same initial drawing point as HomeStraight
		_createStraight(scene, group, mesh, currentId, 25, 0, 1, drawX - TRACK_WIDTH, drawY);
		_createStraight(scene, group, mesh, currentId, 13, 0, 1, drawX, drawY - TRACK_WIDTH);

		//Turn2 - Only yPosition has changed 
		drawY -= 25 * CHEERIO_SPACING;

		_createStraight(scene, group, mesh, currentId, 52, 1, 0, drawX - TRACK_WIDTH, drawY);
		_createStraight(scene, group, mesh, currentId, 14, 1, 0, drawX, drawY + TRACK_WIDTH);
	

		//Turn3 - Only xPosition has changed
		drawX += 20 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 12, 0, -1, drawX, drawY + CHEERIO_SPACING);

		//TurnVIP
		drawX -= CHEERIO_SPACING;
		drawY += 12 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 12, -1, 0, drawX, drawY);
		drawX -= 12 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 1, 0, -1, drawX, drawY);

		//"Turn"5
		drawY += 1 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 22, 1, 0, drawX, drawY);

		//InteriorParabola
		drawX += 28 * CHEERIO_SPACING;
		drawY += 5 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 11, 0, 1, drawX, drawY);

		drawY -= 11 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 6, -1, 0, drawX, drawY);

		//TurnEar
		drawX -= 6 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 1, 0, 1, drawX, drawY);

		//TurnOfTheTank
		drawY -= 1 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 10, 1, 0, drawX, drawY);

		//Esses
		drawX += 10 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 12, 0, -1, drawX + TRACK_WIDTH, drawY - TRACK_WIDTH);
		_createStraight(scene, group, mesh, currentId, 4, -1, 0, drawX + TRACK_WIDTH, drawY + TRACK_WIDTH);

		//AyrtonSennaParabola
		drawX += 11 * CHEERIO_SPACING;
		drawY += 6 * CHEERIO_SPACING;
		_createStraight(scene, group, mesh, currentId, 5, -1, 0, drawX, drawY);
		_createStraight(scene, group, mesh, currentId, 13, 0, -1, drawX, drawY);

	}

	//void _createStraight(cenas, nCheerios, id, [x,y]L, [x,y]C)
	void _createStraight(Scene* scene, GroupComponent& group, const MeshComponent& mesh, int& currentId, int nCheerios, int xModifier, int yModifier, float xInitial, float yInitial) {
		for (int i = 0; i < nCheerios; i++) {
			_createCheerio(scene, group, mesh, currentId++, -TABLE_SIZE.x / 2 + xInitial + xModifier * i * CHEERIO_SPACING, TABLE_SIZE.z / 2 + yInitial - yModifier * i * CHEERIO_SPACING);
		}
	}

	void _createCheerio(Scene* scene, GroupComponent& group, const MeshComponent& mesh, unsigned int id, float xPos, float yPos)
	{
		constexpr RigidbodyComponent::RigidbodyType rigidbodyType = RigidbodyComponent::RigidbodyType::DYNAMIC;
		constexpr float mass = 1000.0f;
		constexpr float drag = 1000.0f;
		constexpr float angularDrag = 1000.0f;

		Entity cheerio = group.addNewEntity(scene, *this, "cheerio_" + std::to_string(id));
		cheerio.addComponent<MeshComponent>(mesh, cheerio);

		Coords3f cheerioSize = Coords3f({ CHEERIO_SIZE.x, CHEERIO_OUTER_RADIUS * 3.0f, CHEERIO_SIZE.z });
		RigidbodyComponent& rigidbody = cheerio.addComponent<RigidbodyComponent>(rigidbodyType, mass, drag, angularDrag);
		AABBColliderComponent& collider = cheerio.addComponent<AABBColliderComponent>(ColliderIds::CHEERIO, rigidbody, cheerioSize);
		collider.setCollisionResolver(new CheerioCollisionResolver(collider));
		collider.setFixedBoundingBox(true);
		collider.setRestitutionCocoefficient(0.1f);

		Transform::scaleTo(cheerio, CHEERIO_SIZE);
		Transform::translate(cheerio, { xPos, 0.5f, yPos });
	}

};


#endif // !__mm3d_entities_world_cheeriosTrack__