#pragma once
#include "CSceneComponent.h"

#include "../UI/CWidget.h"
#include "../World/CWorld.h"

class CGameObject;

class CWidgetComponent :
    public CSceneComponent
{
    friend CGameObject;
    friend CObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& Other);
    CWidgetComponent(CWidgetComponent&& Other) noexcept;

public:
    ~CWidgetComponent() override;

protected:
    std::shared_ptr<CWidget> Widget;

public:
    void SetWidget(const std::shared_ptr<CWidget>& Widget)
    {
        this->Widget = Widget;
    }

    void Begin() override;
    bool Init() override;
    void Update(const float DeltaTime) override;
    void PostUpdate(const float DeltaTime) override;
    void Render() override;
    void PostRender() override;
    void Destroy() override;

protected:
    CWidgetComponent* Clone() const override;

public:
    template <typename T>
    std::weak_ptr<T> SetWidget(const std::string& Key)
    {
        Widget.reset(new T);

        Widget->SetWorld(World);
        Widget->SetUIManager(World.lock()->GetUIManager());
        Widget->SetKey(Key);

        if (!Widget->Init())
        {
            return {};
        }

        return std::dynamic_pointer_cast<T>(Widget);
    }
};

