#pragma once

#include "../../CObject.h"
#include "CBufferContainer.h"

class CConstantBuffer :
    public CObject
{
	friend class CShaderManager;

public:
	/**
	 * @brief Initializes the constant buffer with the specified size, register, and shader buffer type.
	 *
	 * This method sets up a Direct3D constant buffer with the given parameters. It creates a buffer
	 * description with dynamic usage and binds it as a constant buffer. The buffer is designed to
	 * be frequently updated with new data.
	 *
	 * @param Size The size of the constant buffer in bytes.
	 * @param Register The register number where the constant buffer will be bound.
	 * @param ShaderBuffer The shader buffer type that determines which shaders will use this constant buffer.
	 * @return true if the constant buffer is successfully created and initialized; false otherwise.
	 */
	bool Init(int Size, int Register, int ShaderBuffer);
	void Update(const void* Data) const;

private:
	ID3D11Buffer* Buffer = nullptr;

	/**
	 * @brief Represents the size of the constant buffer in bytes.
	 *
	 * This member variable stores the size of the Direct3D constant buffer.
	 * It is used during initialization and updates to allocate and manage
	 * the buffer's memory. The size determines how much data can be stored
	 * in the buffer for use in shader operations.
	 */
	int Size = 0;
	
	/**
	 * @brief Specifies the register number where the constant buffer is bound.
	 *
	 * This member variable determines the binding point of the constant buffer
	 * in the shader pipeline. It is used during initialization and when setting
	 * the constant buffer for various shader stages (e.g., vertex, pixel, etc.).
	 * The register number must match the corresponding binding point defined
	 * in the shader code.
	 */
	int Register = 0;

	/**
	 * @brief Specifies the type of shader buffer that utilizes this constant buffer.
	 *
	 * This member variable determines which shader stages (e.g., Vertex, Pixel, Domain, etc.)
	 * will use this constant buffer. It is a bitmask value based on the `EShaderBufferType` enum,
	 * allowing multiple shader stages to share the same buffer. The value is set during
	 * initialization and used during updates to bind the buffer to the appropriate shader stages.
	 *
	 * @see EShaderBufferType
	 */
	int ShaderBuffer = 0;

private:
	CConstantBuffer() = default;

public:
	~CConstantBuffer() override;
};

