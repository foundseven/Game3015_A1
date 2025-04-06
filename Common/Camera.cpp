//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Camera.h"

using namespace DirectX;

//-----------------------------------------------------------------------
// Camera Class Implementation
//-----------------------------------------------------------------------

/**
 * @brief Constructs a camera with default perspective parameters.
 * @details Initializes camera with a 45-degree FOV, 1:1 aspect ratio,
 *          and 1-1000 view volume.
 */
Camera::Camera()
{
	SetLens(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

//-----------------------------------------------------------------------
// Position Accessors/Mutators
//-----------------------------------------------------------------------

/**
 * @brief Gets camera position as an XMVECTOR.
 * @return Position vector in SIMD format
 */
XMVECTOR Camera::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}

/**
 * @brief Gets camera position as an XMFLOAT3.
 * @return Position in standard float3 format
 */
XMFLOAT3 Camera::GetPosition3f()const
{
	return mPosition;
}

/**
 * @brief Sets camera position using individual coordinates.
 * @param x X-coordinate in world space
 * @param y Y-coordinate in world space
 * @param z Z-coordinate in world space
 */
void Camera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
	mViewDirty = true;
}

/**
 * @brief Sets camera position using an XMFLOAT3 vector.
 * @param v Position vector in world space
 */
void Camera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
	mViewDirty = true;
}

/**
 * @brief Sets camera position using an XMVECTOR.
 * @param v Position vector in SIMD format
 */
XMVECTOR Camera::GetRight()const
{
	return XMLoadFloat3(&mRight);
}

/**
 * @brief Gets right vector (X-axis) as XMFLOAT3.
 * @return Normalized right vector
 */
XMFLOAT3 Camera::GetRight3f()const
{
	return mRight;
}

/**
 * @brief Gets up vector (Y-axis) as XMVECTOR.
 * @return Normalized up vector
 */
XMVECTOR Camera::GetUp()const
{
	return XMLoadFloat3(&mUp);
}

/**
 * @brief Gets up vector (Y-axis) as XMFLOAT3.
 * @return Normalized up vector
 */
XMFLOAT3 Camera::GetUp3f()const
{
	return mUp;
}

/**
 * @brief Gets look vector (Z-axis) as XMVECTOR.
 * @return Normalized look vector
 */
XMVECTOR Camera::GetLook()const
{
	return XMLoadFloat3(&mLook);
}

/**
 * @brief Gets look vector (Z-axis) as XMFLOAT3.
 * @return Normalized look vector
 */
XMFLOAT3 Camera::GetLook3f()const
{
	return mLook;
}

// (Similar Doxygen comments for GetUp/GetUp3f, GetLook/GetLook3f)

//-----------------------------------------------------------------------
// Projection Parameter Accessors
//-----------------------------------------------------------------------

/**
 * @brief Gets near plane distance.
 * @return Distance to near clipping plane
 */
float Camera::GetNearZ()const
{
	return mNearZ;
}

/**
 * @brief Gets far plane distance.
 * @return Distance to far clipping plane
 */
float Camera::GetFarZ()const
{
	return mFarZ;
}

/**
 * @brief Gets aspect ratio.
 * @return Aspect ratio (width/height)
 */
float Camera::GetAspect()const
{
	return mAspect;
}

/**
 * @brief Gets field of view in radians.
 * @return Field of view angle in radians
 */
float Camera::GetFovY()const
{
	return mFovY;
}

/**
 * @brief Gets field of view in degrees.
 * @return Field of view angle in degrees
 */
float Camera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

//-----------------------------------------------------------------------
// View Volume Calculations
//-----------------------------------------------------------------------

/**
 * @brief Gets width of view window at near plane.
 * @return Width in world units at near clipping plane
 */
float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

/**
 * @brief Gets height of view window at near plane.
 * @return Height in world units at near clipping plane
 */
float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

/**
 * @brief Gets width of view window at far plane.
 * @return Width in world units at far clipping plane
 */
float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

/**
 * @brief Gets height of view window at far plane.
 * @return Height in world units at far clipping plane
 */
float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

//-----------------------------------------------------------------------
// Projection Setup
//-----------------------------------------------------------------------

/**
 * @brief Configures perspective projection parameters.
 * @param fovY Vertical field of view angle in radians
 * @param aspect Width/Height aspect ratio
 * @param zn Distance to near clipping plane
 * @param zf Distance to far clipping plane
 */
void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// Cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

//-----------------------------------------------------------------------
// View Matrix Management
//-----------------------------------------------------------------------

/**
 * @brief Orients camera to look at target from specified position.
 * @param pos Camera position in world space
 * @param target Point to look at in world space
 * @param worldUp Up vector in world space
 */
void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	mViewDirty = true;
}

/**
 * @brief Orients camera to look at target from specified position.
 * @param pos Camera position in world space
 * @param target Point to look at in world space
 * @param up Up vector in world space
 */
void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	mViewDirty = true;
}

// (Overloaded version with XMFLOAT3 parameters would have similar documentation)

/**
 * @brief Gets current view matrix.
 * @return View transformation matrix in XMMATRIX format
 * @warning Will assert if view matrix is dirty
 */
XMMATRIX Camera::GetView()const
{
	assert(!mViewDirty);
	return XMLoadFloat4x4(&mView);
}

/**
 * @brief Gets current projection matrix.
 * @return Projection transformation matrix in XMMATRIX format
 */
XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&mProj);
}

/**
 * @brief Gets current view matrix.
 * @return View transformation matrix in XMFLOAT4X4 format
 * @warning Will assert if view matrix is dirty
 */
XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!mViewDirty);
	return mView;
}

/**
 * @brief Gets current projection matrix.
 * @return Projection transformation matrix in XMFLOAT4X4 format
 */
XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return mProj;
}

//-----------------------------------------------------------------------
// Camera Movement
//-----------------------------------------------------------------------

/**
 * @brief Moves camera along its right vector.
 * @param d Distance to strafe (positive = right, negative = left)
 */
void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

	mViewDirty = true;
}

/**
 * @brief Moves camera along its look vector.
 * @param d Distance to walk (positive = forward, negative = backward)
 */
void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	mViewDirty = true;
}

/**
 * @brief Moves camera along its up vector.
 * @param d Distance to move vertically (positive = up, negative = down)
 */
void Camera::Pedestal(float d)
{
	// mPosition += d*mUp
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, u, p));

	mViewDirty = true;
}

//-----------------------------------------------------------------------
// Camera Rotation
//-----------------------------------------------------------------------

/**
 * @brief Pitches camera up/down around its right vector.
 * @param angle Angle in radians to rotate (positive = look up)
 */
void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp,   XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

/**
 * @brief Rotates camera around world Y-axis.
 * @param angle Angle in radians to rotate (positive = rotate right)
 */
void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight,   XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

/**
 * @brief Rolls camera around its look vector.
 * @param angle Angle in radians to rotate (positive = clockwise roll)
 */
void Camera::Roll(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationZ(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

/**
 * @brief Rebuilds view matrix if camera parameters have changed.
 * @details Ensures orthonormal basis vectors and reconstructs view matrix
 *          from current camera orientation and position.
 */
void Camera::UpdateViewMatrix()
{
	if(mViewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&mRight);
		XMVECTOR U = XMLoadFloat3(&mUp);
		XMVECTOR L = XMLoadFloat3(&mLook);
		XMVECTOR P = XMLoadFloat3(&mPosition);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&mRight, R);
		XMStoreFloat3(&mUp, U);
		XMStoreFloat3(&mLook, L);

		mView(0, 0) = mRight.x;
		mView(1, 0) = mRight.y;
		mView(2, 0) = mRight.z;
		mView(3, 0) = x;

		mView(0, 1) = mUp.x;
		mView(1, 1) = mUp.y;
		mView(2, 1) = mUp.z;
		mView(3, 1) = y;

		mView(0, 2) = mLook.x;
		mView(1, 2) = mLook.y;
		mView(2, 2) = mLook.z;
		mView(3, 2) = z;

		mView(0, 3) = 0.0f;
		mView(1, 3) = 0.0f;
		mView(2, 3) = 0.0f;
		mView(3, 3) = 1.0f;

		mViewDirty = false;
	}
}


