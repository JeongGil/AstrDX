#pragma once
#include <UI/CWidgetContainer.h>

class CPlayerStateWidget;
class CWorldUIManager;

class CMainWidget :
    public CWidgetContainer
{
	friend CWorldUIManager;

protected:
	CMainWidget();

public:
	~CMainWidget() override;

private:
	std::shared_ptr<CPlayerStateWidget> PlayerStateWidget;

public:
	[[nodiscard]] std::shared_ptr<CPlayerStateWidget> GetPlayerStateWidget() const
	{
		return PlayerStateWidget;
	}

public:
	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
};

