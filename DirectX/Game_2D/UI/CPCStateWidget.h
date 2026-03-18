#pragma once
#include <UI/CWidgetContainer.h>

class CBrotatoProgressBar;
class CPlayerCharacter;
class CWorldUIManager;

class CPCStateWidget :
    public CWidgetContainer
{
    friend CWorldUIManager;
    friend CWidgetContainer;

protected:
    CPCStateWidget();

public:
    ~CPCStateWidget() override;

private:
    std::weak_ptr<CBrotatoProgressBar> HPBar;
    std::weak_ptr<CPlayerCharacter> PlayerCharacter;

private:
    void OnPlayerHPChanged(float CurrentHP, float MaxHP);

public:
    void SetPlayerCharacter(const std::weak_ptr<CPlayerCharacter>& InPlayerCharacter);

    bool Init() override;
    void Update(const float DeltaTime) override;
    void Render() override;
};

