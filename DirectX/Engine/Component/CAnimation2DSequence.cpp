#include "CAnimation2DSequence.h"

CAnimation2DSequence::CAnimation2DSequence(const CAnimation2DSequence& other): Animation(other.Animation),
                                                                               CurrFrame(other.CurrFrame),
                                                                               FrameTime(other.FrameTime),
                                                                               PlayTime(other.PlayTime),
                                                                               PlayRate(other.PlayRate),
                                                                               bLoop(other.bLoop),
                                                                               bReverse(other.bReverse),
                                                                               bSymmetry(other.bSymmetry),
                                                                               ElapsedTime(other.ElapsedTime),
                                                                               Notifies(other.Notifies)
{
}

CAnimation2DSequence::CAnimation2DSequence(CAnimation2DSequence&& other) noexcept: Animation(std::move(other.Animation)),
	CurrFrame(other.CurrFrame),
	FrameTime(other.FrameTime),
	PlayTime(other.PlayTime),
	PlayRate(other.PlayRate),
	bLoop(other.bLoop),
	bReverse(other.bReverse),
	bSymmetry(other.bSymmetry),
	ElapsedTime(other.ElapsedTime),
	Notifies(std::move(other.Notifies))
{
}

CAnimation2DSequence& CAnimation2DSequence::operator=(const CAnimation2DSequence& other)
{
	if (this == &other)
		return *this;
	Animation = other.Animation;
	CurrFrame = other.CurrFrame;
	FrameTime = other.FrameTime;
	PlayTime = other.PlayTime;
	PlayRate = other.PlayRate;
	bLoop = other.bLoop;
	bReverse = other.bReverse;
	bSymmetry = other.bSymmetry;
	ElapsedTime = other.ElapsedTime;
	Notifies = other.Notifies;
	return *this;
}

CAnimation2DSequence& CAnimation2DSequence::operator=(CAnimation2DSequence&& other) noexcept
{
	if (this == &other)
		return *this;
	Animation = std::move(other.Animation);
	CurrFrame = other.CurrFrame;
	FrameTime = other.FrameTime;
	PlayTime = other.PlayTime;
	PlayRate = other.PlayRate;
	bLoop = other.bLoop;
	bReverse = other.bReverse;
	bSymmetry = other.bSymmetry;
	ElapsedTime = other.ElapsedTime;
	Notifies = std::move(other.Notifies);
	return *this;
}

void CAnimation2DSequence::Update(const float DeltaTime)
{
	auto Animation = this->Animation.lock();
	if (!Animation)
	{
		return;
	}

	ElapsedTime += DeltaTime * PlayRate;
	if (ElapsedTime >= FrameTime)
	{
		ElapsedTime -= FrameTime;

		if (bReverse)
		{
			--CurrFrame;
			if (CurrFrame < 0)
			{
				if (FinishNotify)
				{
					FinishNotify();
				}

				if (bLoop)
				{
					CurrFrame = Animation->GetFrameCount() - 1;

					for (auto& Notify : Notifies)
					{
						Notify.bCalled = false;
					}
				}
				else
				{
					CurrFrame = 0;
				}
			}
		}
		else
		{
			++CurrFrame;
			if (CurrFrame == Animation->GetFrameCount())
			{
				if (FinishNotify)
				{
					FinishNotify();
				}

				if (bLoop)
				{
					CurrFrame = 0;

					for (auto& Notify : Notifies)
					{
						Notify.bCalled = false;
					}
				}
				else
				{
					CurrFrame = Animation->GetFrameCount() - 1;
				}
			}
		}
	}

	CallNotify();
}

CAnimation2DSequence* CAnimation2DSequence::Clone() const
{
	return new CAnimation2DSequence(*this);
}

void CAnimation2DSequence::Clear()
{
	for (auto& Notify : Notifies)
	{
		Notify.bCalled = false;
	}

	if (auto Animation = this->Animation.lock())
	{
		if (bReverse)
		{
			CurrFrame = Animation->GetFrameCount() - 1;
		}
		else
		{
			CurrFrame = 0;
		}

		ElapsedTime = 0.f;
	}
}

const std::string& CAnimation2DSequence::GetKey() const
{
	if (auto Animation = this->Animation.lock())
	{
		return Animation->GetKey();
	}

	static const std::string Empty = "";
	return Empty;
}

void CAnimation2DSequence::CalcFrameTime()
{
	if (auto Animation = this->Animation.lock())
	{
		FrameTime = PlayTime / Animation->GetFrameCount();
	}
}

void CAnimation2DSequence::CallNotify()
{
	for (auto& Notify : Notifies)
	{
		if (Notify.Frame == CurrFrame && !Notify.bCalled)
		{
			Notify.bCalled = true;

			if (Notify.Function)
			{
				Notify.Function();
			}
		}
	}
}
