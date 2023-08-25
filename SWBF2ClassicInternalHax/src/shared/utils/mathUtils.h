#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "../structs/vec.h"
#include "../structs/matrix.h"

namespace mathutils {

	inline float ToDegrees(float x) {
		return x * (180.f / std::numbers::pi_v<float>);
	}

	inline float ToRadians(float x) {
		return x * (180.f / std::numbers::pi_v<float>);
	}

	// Get value between -1 to 1
	inline float normalize(float min, float max, float x) {
		return (2 * ((x - min) / (max - min))) - 1;
	}

	// Take value from -1 to 1 and change it to [min, max]
	inline float inverseNormalize(float min, float max, float value) {
		return ((value + 1) / 2) * (max - min) + min;
	}

	inline Vec3 CalcAngle(int i, Vec3 src, Vec3 dst) {
		Vec3 angles;
		Vec3 direction = src - dst;

		angles.y = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));
		angles.x = atan2(direction.x, direction.z);
		angles.z = 0.0f;

		angles = angles.ToDegrees();

		return angles;
	}

	inline float WrapAngle(float angle) {
		while (angle < 0) {
			angle += 360;
		}
		while (angle >= 360) {
			angle -= 360;
		}
		return angle;
	}

	inline glm::mat4 BuildViewMatrix(Vec3 position, float xAngle, float yAngle, float zAngle, float windowWidth, float windowHeight, Matrix4x4& out) {
		float nearDist = 0.1f;
		float farDist = 250.f;
		float aspect = windowWidth / windowHeight;

		glm::mat4 projection = glm::perspective(45.f, aspect, nearDist, farDist);
		glm::mat4 translate = glm::translate(glm::mat4(1.f), { position.x, position.y, position.z });
		glm::mat4 rotate_x = glm::rotate(glm::mat4(1.f), glm::radians(yAngle * 45.f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotate_y = glm::rotate(glm::mat4(1.f), glm::atan(-xAngle, -zAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotate_z = glm::rotate(glm::mat4(1.f), glm::radians(1.f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 model = translate * rotate_z * rotate_y * rotate_x;
		glm::mat4 view = glm::inverse(model);
		glm::mat4 viewprojection = glm::transpose(projection * view);

		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				out.m[y * 4 + x] = viewprojection[y][x];
			}
		}
		return viewprojection;
	}

	inline bool WorldToScreen(Vec3 pos, Matrix4x4 viewMatrix, float windowWidth, float windowHeight, Vec3 &screen) {
		Vec4 clipCoords;
		clipCoords.x = pos.x*viewMatrix.m[0] + pos.y*viewMatrix.m[4] + pos.z*viewMatrix.m[8] + viewMatrix.m[12];
		clipCoords.y = pos.x*viewMatrix.m[1] + pos.y*viewMatrix.m[5] + pos.z*viewMatrix.m[9] + viewMatrix.m[13];
		clipCoords.z = pos.x*viewMatrix.m[2] + pos.y*viewMatrix.m[6] + pos.z*viewMatrix.m[10] + viewMatrix.m[14];
		clipCoords.w = pos.x*viewMatrix.m[3] + pos.y*viewMatrix.m[7] + pos.z*viewMatrix.m[11] + viewMatrix.m[15];

		if (clipCoords.w < 0.1f) {
			return false;
		}

		Vec3 ndc;
		ndc.x = clipCoords.x / clipCoords.w;
		ndc.y = clipCoords.y / clipCoords.w;

		screen.x = (windowWidth / 2 * ndc.x) + (ndc.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * ndc.y) + (ndc.y + windowHeight / 2);
		
		return true;
	}


	inline bool WorldToScreen2(Vec3 pos, Matrix4x4 viewMatrix, float windowWidth, float windowHeight, Vec3 &screen) {
		Vec4 clipCoords;
		clipCoords.x = pos.x * viewMatrix.m[0] + pos.y * viewMatrix.m[1] + pos.z * viewMatrix.m[2] + viewMatrix.m[3];
		clipCoords.y = pos.x * viewMatrix.m[4] + pos.y * viewMatrix.m[5] + pos.z * viewMatrix.m[6] + viewMatrix.m[7];
		clipCoords.z = pos.x * viewMatrix.m[8] + pos.y * viewMatrix.m[9] + pos.z * viewMatrix.m[10] + viewMatrix.m[11];
		clipCoords.w = pos.x * viewMatrix.m[12] + pos.y * viewMatrix.m[13] + pos.z * viewMatrix.m[14] + viewMatrix.m[15];

		if (clipCoords.w < 0.1f) {
			return false;
		}

		Vec3 ndc;
		ndc.x = clipCoords.x / clipCoords.w;
		ndc.y = clipCoords.y / clipCoords.w;

		screen.x = (windowWidth / 2 * ndc.x) + (ndc.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * ndc.y) + (ndc.y + windowHeight / 2);
		screen.z = clipCoords.w;
		
		return true;
	}


	inline bool WorldToScreen3(Vec3 pos, Matrix4x4 viewMatrix, float windowWidth, float windowHeight, Vec3 &screen) {
		Vec4 clipCoords;
		clipCoords.x = pos.x * viewMatrix.m[3] + pos.y * viewMatrix.m[7] + pos.z * viewMatrix.m[11] + viewMatrix.m[15];
		clipCoords.y = pos.x * viewMatrix.m[2] + pos.y * viewMatrix.m[6] + pos.z * viewMatrix.m[10] + viewMatrix.m[14];
		clipCoords.z = pos.x * viewMatrix.m[1] + pos.y * viewMatrix.m[5] + pos.z * viewMatrix.m[9] + viewMatrix.m[13];
		clipCoords.w = pos.x * viewMatrix.m[0] + pos.y * viewMatrix.m[4] + pos.z * viewMatrix.m[8] + viewMatrix.m[12];

		if (clipCoords.w < 0.1f) {
			return false;
		}

		Vec3 ndc;
		ndc.x = clipCoords.x / clipCoords.w;
		ndc.y = clipCoords.y / clipCoords.w;

		screen.x = (windowWidth / 2 * ndc.x) + (ndc.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * ndc.y) + (ndc.y + windowHeight / 2);
		screen.z = clipCoords.w;
		
		return true;
	}

}