#pragma once


#include <engine/engine.h>
#include <engine/transform.h>
#include "../../../sfml_engine/include/sfml_engine/shape.h"
#include <random>

namespace net
{
	struct ActorData
	{
		neko::Index tickIndex = neko::INVALID_INDEX;
		neko::Entity entity = neko::INVALID_ENTITY;
		neko::Vec2f position{};
		neko::Vec2f velocity{};
	};
	const size_t serverActorDataBufferSize = 4;
	const neko::EntityMask velocityMask = 1u << 15u;
	class VelocityManager : public neko::ComponentManager<neko::Vec2f, velocityMask>
	{
		
	};

	class PredSimEngine;
	enum class ServerPredictionType
	{
		None,
		Interpolation,
		Extrapolation,
		Catmull_Interpolation
	};
	class ServerSimSystem : public neko::System
	{
	public:
		explicit ServerSimSystem(PredSimEngine& engine);
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
		void PushClientData(const ActorData& data);
	private:
		PredSimEngine& engine_;
		std::vector<std::array<ActorData, serverActorDataBufferSize>> serverActorsDataBuffer_;
		std::vector<neko::Entity> serverEntities_;
		std::unordered_map<neko::Entity, neko::Entity> entitiesTranslateTable_;
		neko::Index tick_ = 0;
		size_t dataSent_ = 0;
		float currentSecondBandwidth_ = 0.0f;
        std::random_device rd{};
        std::mt19937 eng_{rd()};
		ServerPredictionType serverPredictionType_ = ServerPredictionType::Interpolation;
	};
	enum class ClientMovementType
	{
		Linear,
		Planet,
		Boids
	};
	class ClientSimSystem : public  neko::System
	{
	public:
		explicit ClientSimSystem(PredSimEngine& engine);
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		friend class ServerSimSystem;
		std::vector<neko::Index> randomDirectionChangePeriods_;
		std::vector<std::pair<ActorData, neko::Index>> dataDelayQueue_;
		std::vector<neko::Entity> entities_;
		PredSimEngine& engine_;
		neko::Index tick_ = 0;
		ClientMovementType clientMovementType_ = ClientMovementType::Boids;
        std::random_device rd{};
        std::mt19937 eng_{rd()};
	};

	
	class PredSimEngine : public neko::BasicEngine
	{
	public:
		PredSimEngine();
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		friend class ServerSimSystem;
		friend class ClientSimSystem;
		ServerSimSystem server_;
		ClientSimSystem client_;


		neko::EntityManager entityManager_;
		neko::GraphicsManager graphicsManager_;
		neko::Transform2dManager transformManager_;
		neko::ConvexShapeManager shapeManager_;
		VelocityManager velocitiesManager_;
	};

}
