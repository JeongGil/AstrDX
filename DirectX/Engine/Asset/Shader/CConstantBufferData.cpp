#include "CConstantBufferData.h"

#include "CShaderManager.h"
#include "../CAssetManager.h"

void CConstantBufferData::SetConstantBuffer(const std::string& Key)
{
	const auto WeakShaderMgr = CAssetManager::GetInst()->GetShaderManager();
	if (const auto ShaderManager = WeakShaderMgr.lock())
	{
		Buffer = ShaderManager->FindCBuffer(Key);
	}
}
