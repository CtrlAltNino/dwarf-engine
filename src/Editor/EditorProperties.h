#pragma once

namespace Dwarf {

	/// @brief Class storing properties of the editor.
	class EditorProperties {
	public:
		/// @brief Rotation speed of the scene viewer camera.
		static inline float cameraRotationSpeed = 0.15f;

		/// @brief Maximum frames per second.
		static inline int FrameLimit = 144;

		/// @brief Movement speed of the scene viewer camera.
		static inline float moveSpeed = 4;
	};
}