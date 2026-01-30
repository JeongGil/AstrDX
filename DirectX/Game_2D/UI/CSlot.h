#pragma once
#include <UI\CWidgetContainer.h>

class CSlotIcon;
class CWorldUIManager;
class CInventory;

class CSlot :
    public CWidgetContainer
{
    friend CWorldUIManager;
    friend CInventory;
    friend CWidgetContainer;

protected:
    CSlot();

public:
    ~CSlot() override;

private:
    std::shared_ptr<CSlotIcon> Icon;

public:
    void EnableIcon(bool bEnable);
    void SetIconImage(const std::string& Key);
    void SetIconImage(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");

    bool Init() override;
    void Update(const float DeltaTime) override;
    void Render() override;
};

