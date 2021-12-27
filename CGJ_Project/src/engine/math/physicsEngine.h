#ifndef __engine_math_physicsEngine__
#define __engine_math_physicsEngine__


class Scene;




class PhysicsEngine
{

public:
	static constexpr float GRAVITY = 9.8f;
	



private:
	




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	void run(const Scene& scene) const;

};


#endif // !__engine_math_physicsEngine__
