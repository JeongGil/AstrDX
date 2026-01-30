#pragma once
#include <UI/CWidgetContainer.h>

class CImage;
class CWorldUIManager;
class CInventory;

class CSlotIcon :
    public CWidgetContainer
{
    friend CWorldUIManager;
    friend CInventory;
    friend CWidgetContainer;

protected:
    CSlotIcon();
    CSlotIcon(const CSlotIcon& other);

public:
    ~CSlotIcon() override;

private:
    std::weak_ptr<CImage> Image;

public:
    void SetIconImage(const std::string& Key);
    void SetIconImage(const std::string& Key, const TCHAR* FileName, const std::string& PathName = "Texture");

public:
    void MouseHovered() override;
    void MouseUnHovered() override;
    bool MouseDragStart(const FVector2& MousePos, std::shared_ptr<CWidget>& DragOperator) override;
    bool MouseDrag(const FVector2& MousePos, const FVector2& MouseMove) override;
    bool MouseDragEnd(const FVector2& MousePos) override;
    CSlotIcon* Clone() const override;
    bool Init() override;
    void Update(const float DeltaTime) override;
    void Render() override;
};

