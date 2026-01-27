#pragma once

#include "CBufferContainer.h"
#include "../../EngineInfo.h"
#include "CShader.h"

class CConstantBuffer;

class CShaderManager
{
	friend class CAssetManager;

public:
	/**
	 * @brief Initializes the shader manager by setting up constant buffers and shaders.
	 *
	 * This method creates and initializes a constant buffer named "Transform" with
	 * predefined size and shader buffer type. Additionally, it creates and initializes
	 * a shader of type `CShaderColor2D` with the key "Color2D".
	 *
	 * @return true if all constant buffers and shaders are successfully created and initialized,
	 *         false otherwise.
	 */
	bool Init();

	void ReleaseShader(const std::string& Key);
	void ReleaseCBuffer(const std::string& Key);

	std::weak_ptr<CShader> FindShader(const std::string& Key);
	std::weak_ptr<CConstantBuffer> FindCBuffer(const std::string& Key);

	/**
	 * @brief Creates and initializes a shader of the specified type.
	 *
	 * This templated method allows the creation of a shader instance of type `T` and associates it
	 * with a unique key. If a shader with the given key already exists, the method returns false.
	 * Otherwise, it initializes the new shader and adds it to the internal shader collection.
	 *
	 * @tparam T The type of the shader to be created. Must be derived from `CShader`.
	 * @param Key The unique identifier for the shader.
	 * @param PathName = "Shader"
	 * @return true if the shader is successfully created and initialized,
	 *         false if a shader with the same key already exists or initialization fails.
	 */
	template <typename T>
	bool CreateShader(const std::string& Key, const std::string& PathName)
	{
		if (Shaders.contains(Key))
		{
			return true;
		}

		auto ShaderInstance = new T;
		std::shared_ptr<CShader> NewShader;
		NewShader.reset(ShaderInstance);
		if (!NewShader->Init(PathName))
		{
			return false;
		}

		NewShader->SetKey(Key);

		Shaders.emplace(Key, NewShader);

		return true;
	}

	/**
	 * @brief Creates and initializes a constant buffer for shaders.
	 *
	 * This method creates a constant buffer with the specified key, size, register,
	 * and shader buffer type. If a constant buffer with the given key already exists,
	 * the method returns false. Otherwise, it initializes a new constant buffer and
	 * adds it to the internal collection.
	 *
	 * @param Key The unique identifier for the constant buffer.
	 * @param Size The size of the constant buffer in bytes.
	 * @param Register The register number where the constant buffer will be bound.
	 * @param ShaderBuffer The shader buffer type that determines which shaders will use this constant buffer.
	 *                     Defaults to `EShaderBufferType::VP` (Vertex and Pixel shaders).
	 *
	 * @return true if the constant buffer is successfully created and initialized,
	 *         false if a buffer with the same key already exists or initialization fails.
	 */
	bool CreateCBuffer(const std::string& Key, int Size, int Register, int ShaderBuffer = EShaderBufferType::VP);

private:
	std::unordered_map<std::string, std::shared_ptr<CShader>> Shaders;
	std::unordered_map<std::string, std::shared_ptr<CConstantBuffer>> CBuffers;

private:
	CShaderManager() = default;

public:
	~CShaderManager() = default;
};

