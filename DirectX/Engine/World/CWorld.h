#pragma once

#include "../EngineInfo.h"
#include "../Object/CGameObject.h"

class CWorld : public std::enable_shared_from_this<CWorld>
{	
public:
	template <typename T>
	std::weak_ptr<CGameObject> CreateGameObject(const std::string& Name)
	{
		std::shared_ptr<CGameObject> NewObject(new T);

		NewObject->SetWorld(weak_from_this());
		NewObject->SetName(Name);

		if (!NewObject->Init())
		{
			return std::weak_ptr<CGameObject>();
		}

		Objects.push_back(NewObject);

		return NewObject;
	}

protected:
	std::list<std::shared_ptr<CGameObject>> Objects;

public:
	bool Init();
	void Update(float DeltaTime);
	void Render();

public:
	CWorld();
	~CWorld();
};

