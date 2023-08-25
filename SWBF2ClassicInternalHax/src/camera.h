#pragma once
#include "shared/structs/matrix.h"
#include "structs.h"

namespace camera {

    inline Matrix4x4 GetViewMatrix(GameCamera* camera) {
        Matrix4x4 viewMatrix = camera->matrix3;

        // The view matrix is inversed and oriented wrong, so fix that
        Matrix4x4 viewMatrixMapped;
        viewMatrixMapped.m[0] = viewMatrix.m[0];
        viewMatrixMapped.m[1] = viewMatrix.m[4];
        viewMatrixMapped.m[2] = viewMatrix.m[8];
        viewMatrixMapped.m[3] = viewMatrix.m[12];
        viewMatrixMapped.m[4] = viewMatrix.m[1];
        viewMatrixMapped.m[5] = viewMatrix.m[5];
        viewMatrixMapped.m[6] = viewMatrix.m[9];
        viewMatrixMapped.m[7] = viewMatrix.m[13];
        viewMatrixMapped.m[8] = viewMatrix.m[2] * -1;
        viewMatrixMapped.m[9] = viewMatrix.m[6] * -1;
        viewMatrixMapped.m[10] = viewMatrix.m[10] * -1;
        viewMatrixMapped.m[11] = viewMatrix.m[14] * -1;
        viewMatrixMapped.m[12] = viewMatrix.m[3] * -1;
        viewMatrixMapped.m[13] = viewMatrix.m[7] * -1;
        viewMatrixMapped.m[14] = viewMatrix.m[11] * -1;
        viewMatrixMapped.m[15] = viewMatrix.m[15] * -1;

        return viewMatrixMapped;
    }
}