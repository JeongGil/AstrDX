#pragma once
#include <UI/CWidgetContainer.h>

class CProgressBar;
class CTextBlock;
class CWorldUIManager;

class CPlayerStateWidget :
    public CWidgetContainer
{
    friend CWorldUIManager;
    friend CWidgetContainer;


protected:
    CPlayerStateWidget();

public:
    ~CPlayerStateWidget() override;

private:
    std::weak_ptr<CTextBlock> NameText;
    std::weak_ptr<CProgressBar> HPBar;
    std::weak_ptr<CProgressBar> MPBar;

public:
    void SetPlayerName(const TCHAR* Name);
    void SetPlayerHP(float HP, float MaxHP);
    void SetPlayerMP(float MP, float MaxMP);

    bool Init() override;
    void Update(const float DeltaTime) override;
    void Render() override;
};

