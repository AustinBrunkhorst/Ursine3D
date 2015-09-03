/* --h-------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** SMat4.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SVec4.h"
#include "SVec3.h"
#include "SQuat.h"

namespace ursine
{
	// Forward Declaration
	class SMat3;
	class Mat3;
	class Mat4;

    ALIGNED16(class) SMat4
    {
        friend class SMat3;
		friend class Mat3;
		friend class Mat4;

        // Row Major
        // http://en.wikipedia.org/wiki/Row-major_order
        // Each vector represents one row of the 4x4 matrix
        SVec4 m[4];

        // helpers for column dot row
        float dotCol0(const SVec4 &row) const;
        float dotCol1(const SVec4 &row) const;
        float dotCol2(const SVec4 &row) const;
        float dotCol3(const SVec4 &row) const;

        // helper for inverse (eleminating duplicate code)
        static void invert(const float *src, float *dest);
        // TODO: static void invertFast(const float *src, float *dest);

		void setRotation(const SQuat &quat);
		void setTRS(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale);

    public:
        // Generic Constructors
        SMat4(void);
        SMat4(const SMat4 &other);
		explicit SMat4(const Mat4 &other);
        explicit SMat4(const SMat3 &mat);
		explicit SMat4(const Mat3 &mat);
		explicit SMat4(const SQuat &quat);
		SMat4(const SQuat &quat, const SVec3 &trans);
        SMat4(const SVec4 &r0, const SVec4 &r1, const SVec4 &r2, const SVec4 &r3);
        SMat4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33);

        // 3D Homogenous Constructors
        SMat4(float x_scalar, float y_scalar, float z_scalar); // Scalar
        explicit SMat4(const SVec3 &translation); // Translation
		SMat4(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale);

        // Properties
        static const SMat4 &Identity(void);

        // Public Methods
        void Clean(void); // Sets all near zero elements to zero

        void Set(float M00, float M01, float M02, float M03,
                 float M10, float M11, float M12, float M13,
                 float M20, float M21, float M22, float M23,
                 float M30, float M31, float M32, float M33);

        void Translate(const SVec3 &translation);
        static void Translate(SMat4 &mat, const SVec3 &translation);

		void Rotation(const SQuat &quat);

		void RotationZXY(float z_degrees, float x_degrees, float y_degrees);
		static void RotationZXY(SMat4 &mat, float z_degrees, float x_degrees, float y_degrees);

        void Scale(const SVec3 &scale);
        static void Scale(SMat4 &mat, const SVec3 &scale);

        void Scale(const SVec4 &scale);
        static void Scale(SMat4 &mat, const SVec4 &scale);

		void TRS(const SVec3 &translation, const SQuat &rotation, const SVec3 &scale);

        void Transpose(void);
        static void Transpose(SMat4 &mat);

        SMat4 Transpose(void) const;
        static SMat4 Transpose(const SMat4 &mat);

        void Inverse(void);
        static void Inverse(SMat4 &mat);

        SMat4 Inverse(void) const;
        static SMat4 Inverse(const SMat4 &mat);

        // Homogeneous Matrices Only!
        // TODO: void InverseFast(void);
        // TODO: static void InverseFast(SMat4 &mat);

        // Homogeneous Matrices Only!
        // TODO: SMat4 InverseFast(void) const;
        // TODO: static SMat4 InverseFast(const SMat4 &mat);

        float Determinant(void) const;
        static float Determinant(const SMat4 &mat);

        void SetRows(const SVec4 &r0, const SVec4 &r1, const SVec4 &r2, const SVec4 &r3);
        void GetRows(SVec4 &r0, SVec4 &r1, SVec4 &r2, SVec4 &r3) const;

        // i must be between 0 and 3 inclusively
        void SetRow(uint i, const SVec4 &row);
        SVec4 GetRow(uint i) const;

        void SetColumns(const SVec4 &c0, const SVec4 &c1, const SVec4 &c2, const SVec4 &c3);
        void GetColumns(SVec4 &c0, SVec4 &c1, SVec4 &c2, SVec4 &c3) const;

        // i must be between 0 and 3 inclusively
        void SetColumn(uint i, const SVec4 &column);
        SVec4 GetColumn(uint i) const;

        // (fast) Homogeneous Transformations Only! Bottom row neglected.
        SVec3 TransformVector(const SVec3 &vector) const;
        void TransformVectorInplace(SVec3 &vector) const;
        SVec3 TransformPoint(const SVec3 &point) const;
        void TransformPointInplace(SVec3 &point) const;

        // Transforms a point and accounts for perspective divide
        SVec3 TransformPointAndDiv(const SVec3 &point) const;
        void TransformPointAndDivInplace(SVec3 &point) const;

		// Identical to D3DXMatrixOrthoLH, except transposed to account for Matrix * vector convention
		// see http://msdn.microsoft.com/en-us/library/windows/desktop/bb205346(v=vs.85).aspx
		void D3DOrthoProjLH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);
		
		// Identical to D3DXMatrixOrthoRH, except transposed to account for Matrix * vector convention
		// see http://msdn.microsoft.com/en-us/library/windows/desktop/bb205349(v=vs.85).aspx
		void D3DOrthoProjRH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Identical to D3DXMatrixPerspectiveLH, except transposed to account for Matrix * vector convention
		// see http://msdn.microsoft.com/en-us/library/windows/desktop/bb205352(v=vs.85).aspx
		void D3DPerspProjLH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Identical to D3DXMatrixPerspectiveRH, except transposed to account for Matrix * vector convention
		// see http://msdn.microsoft.com/en-us/library/windows/desktop/bb205355(v=vs.85).aspx
		void D3DPerspProjRH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Computes a left-handed orthographic projection matrix for OpenGL
		void OGLOrthoProjLH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Computes a right-handed orthographic projection matrix for OpenGL
		void OGLOrthoProjRH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Computes a left-handed perspective projection matrix for OpenGL
		void OGLPerspProjLH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);

		// Computes a right-handed perspective projection matrix for OpenGL
		void OGLPerspProjRH(float nearDist, float farDist, float horizontalViewSize, float verticalViewSize);


        std::string ToString(void) const;

        // Accessors
        float &operator()(uint row, uint column);
        float operator()(uint row, uint column) const;
        const float *GetFloatPtr(void) const;

        // Operators
        SMat4 operator*(const SMat4 &rhs) const;
        SMat4 operator*(float rhs) const;
        SVec4 operator*(const SVec4 &rhs) const;
        SMat4 operator+(const SMat4 &rhs) const;
        SMat4 operator-(const SMat4 &rhs) const;
        const SMat4 &operator=(const SMat4 &rhs);
        const SMat4 &operator*=(const SMat4 &rhs); // this = this * rhs
        const SMat4 &operator*=(float rhs);
        const SMat4 &operator+=(const SMat4 &rhs);
        const SMat4 &operator-=(const SMat4 &rhs);
        bool operator==(const SMat4 &rhs) const;
        bool operator!=(const SMat4 &rhs) const;

		ALLOW_ALIGNED_ALLOC(16)
    };
}

#include "SMat4.hpp"
