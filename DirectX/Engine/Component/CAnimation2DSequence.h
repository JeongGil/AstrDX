#pragma once

#include "../Asset/Animation2D/CAnimation2D.h"

struct FAnimation2DNotify;

class CAnimation2DSequence
{
	friend class CAnimation2DComponent;

private:
	CAnimation2DSequence() = default;

	CAnimation2DSequence(const CAnimation2DSequence& other);
	CAnimation2DSequence(CAnimation2DSequence&& other) noexcept;
	CAnimation2DSequence& operator=(const CAnimation2DSequence& other);
	CAnimation2DSequence& operator=(CAnimation2DSequence&& other) noexcept;

public:
	~CAnimation2DSequence() = default;

	void Update(float DeltaTime);

	CAnimation2DSequence* Clone() const;

	void Clear();

	template <typename T>
	void AddNotify(const std::string& Key, int Frame, T* Obj, void(T::* Func)())
	{
		Notifies.emplace_back(Key, Frame, false, std::bind(Func, Obj));
	}

	[[nodiscard]] int GetFrame() const
	{
		return Frame;
	}

	[[nodiscard]] float GetFrameTime() const
	{
		return FrameTime;
	}

	[[nodiscard]] const std::string& GetKey() const;

	[[nodiscard]] float GetElapsedTime() const
	{
		return ElapsedTime;
	}

	const std::weak_ptr<CAnimation2D>& GetAsset() const
	{
		return Animation;
	}

	void SetAnimation2D(const std::weak_ptr<CAnimation2D>& Animation)
	{
		this->Animation = Animation;

		CalcFrameTime();
	}

	void SetPlayTime(float PlayTime)
	{
		this->PlayTime = PlayTime;

		CalcFrameTime();
	}

	void SetPlayRate(float PlayRate)
	{
		this->PlayRate = PlayRate;
	}

	void SetLoop(bool bLoop)
	{
		this->bLoop = bLoop;
	}

	void SetReverse(bool bReverse)
	{
		this->bReverse = bReverse;

		if (auto Animation = this->Animation.lock())
		{
			if (bReverse)
			{
				Frame = Animation->GetFrameCount() - 1;
			}
			else
			{
				Frame = 0;
			}
		}
	}

	void SetSymmetry(bool bSymmetry)
	{
		this->bSymmetry = bSymmetry;
	}

private:
	void CalcFrameTime();

	void CallNotify();

private:
	std::weak_ptr<CAnimation2D> Animation;

	int Frame = 0;
	float FrameTime = 0.f;
	float PlayTime = 1.f;
	float PlayRate = 1.f;
	bool bLoop = false;
	bool bReverse = false;
	bool bSymmetry = false;

	float ElapsedTime = 0.f;

	std::vector<FAnimation2DNotify> Notifies;
};

struct FAnimation2DNotify
{
	std::string Name;
	int Frame = 0;
	bool bCalled = false;
	std::function<void()> Function;
};
