#pragma once
#include "../EngineInfo.h"

class CPathManager
{
public:
	/**
	 * @brief Initializes the path manager by setting up default paths and directories.
	 *
	 * This method retrieves the executable's directory path, removes the executable's name,
	 * and sets it as the root path. Additionally, it creates predefined subdirectories such as
	 * "Asset" and "Shader" under the root path.
	 *
	 * @return true if the initialization is successful, false otherwise.
	 */
	static bool Init();
	static void Clear();

	/**
	 * @brief Creates a new path entry in the path manager.
	 *
	 * This method constructs a full path by appending the specified relative path
	 * to the base path identified by its name. The resulting path is then stored
	 * in the path manager under the given name.
	 *
	 * @param Key The unique name to associate with the new path.
	 * @param Path The relative path to append to the base path.
	 * @param BasePathName The name of the base path to which the relative path will be appended.
	 *                     Defaults to "Root" if not specified.
	 *
	 * @return true if the path was successfully created and added, false if a path
	 *         with the same name already exists or if the base path is not found.
	 */
	static bool CreatePath(const std::string& Key, const TCHAR* Path, const std::string& BasePathName = "Root");

	static const TCHAR* FindPath(const std::string& Key);

private:
	inline static std::unordered_map<std::string, TCHAR*> Paths;
};

