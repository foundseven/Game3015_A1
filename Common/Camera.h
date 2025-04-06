//***************************************************************************************
// Camera.h 
//   
//! @file Camera.h
//! @brief Simple first-person camera class for 3D scene navigation
//! 
//! Maintains camera coordinate system and viewing frustum to generate view/projection matrices.
//! Provides movement controls (strafe/walk/pedestal) and rotation controls (pitch/yaw/roll).
//***************************************************************************************

#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

//! @brief First-person camera implementation for 3D scene viewing
//!
//! Manages camera space orientation and perspective projection parameters.
//! Automatically handles view matrix updates when camera parameters change.
class Camera
{
public:

	Camera();
	~Camera();

	//--------------------------------------------------------------
	// Position Management
	//--------------------------------------------------------------
	
	//! @brief Gets camera position in SIMD vector format
	DirectX::XMVECTOR GetPosition()const;

	//! @brief Gets camera position in XMFLOAT3 format
	DirectX::XMFLOAT3 GetPosition3f()const;

	//! @brief Sets camera position using individual coordinates
	//! @param x World space X-coordinate
	//! @param y World space Y-coordinate
	//! @param z World space Z-coordinate
	void SetPosition(float x, float y, float z);

	//! @brief Sets camera position using float3 vector
	//! @param v Position vector in world space
	void SetPosition(const DirectX::XMFLOAT3& v);
	
	//--------------------------------------------------------------
	// Basis Vectors
	//--------------------------------------------------------------

	//! @brief Gets right vector (X-axis) in SIMD format
	DirectX::XMVECTOR GetRight()const;

	//! @brief Gets right vector (X-axis) in XMFLOAT3 format
	DirectX::XMFLOAT3 GetRight3f()const;

	//! @brief Gets up vector (Y-axis) in SIMD format
	DirectX::XMVECTOR GetUp()const;

	//! @brief Gets up vector (Y-axis) in XMFLOAT3 format
	DirectX::XMFLOAT3 GetUp3f()const;

	//! @brief Gets look vector (Z-axis) in SIMD format
	DirectX::XMVECTOR GetLook()const;

	//! @brief Gets look vector (Z-axis) in XMFLOAT3 format
	DirectX::XMFLOAT3 GetLook3f()const;

	//--------------------------------------------------------------
	// Frustum Properties
	//--------------------------------------------------------------

	//! @brief Gets near clipping plane distance
	float GetNearZ()const;

	//! @brief Gets far clipping plane distance
	float GetFarZ()const;

	//! @brief Gets aspect ratio (width/height)
	float GetAspect()const;

	//! @brief Gets vertical field of view in radians
	float GetFovY()const;

	//! @brief Calculates horizontal field of view in radians
	float GetFovX()const;

	//--------------------------------------------------------------
	// View Volume Dimensions
	//--------------------------------------------------------------

	//! @brief Gets width of near plane view window
	float GetNearWindowWidth()const;

	//! @brief Gets height of near plane view window
	float GetNearWindowHeight()const;

	//! @brief Gets width of far plane view window
	float GetFarWindowWidth()const;

	//! @brief Gets height of far plane view window
	float GetFarWindowHeight()const;
	
	//--------------------------------------------------------------
	// Projection Configuration
	//--------------------------------------------------------------

	//! @brief Configures perspective projection parameters
	//! @param fovY Vertical field of view in radians
	//! @param aspect Width/height ratio
	//! @param zn Near clipping plane distance
	//! @param zf Far clipping plane distance
	void SetLens(float fovY, float aspect, float zn, float zf);

	//--------------------------------------------------------------
	// View Orientation
	//--------------------------------------------------------------

	//! @brief Orients camera to look at target from specified position
	//! @param pos Camera position in world space
	//! @param target Point to focus on in world space
	//! @param worldUp Up direction in world space
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);

	//! @brief Orients camera using XMFLOAT3 parameters
	//! @param pos Camera position in world space
	//! @param target Point to focus on in world space
	//! @param up Up direction in world space
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	//--------------------------------------------------------------
	// Matrix Accessors
	//--------------------------------------------------------------
	
	//! @brief Gets current view matrix
	//! @warning Asserts if view matrix needs updating (check mViewDirty)
	DirectX::XMMATRIX GetView()const;

	//! @brief Gets current projection matrix
	DirectX::XMMATRIX GetProj()const;

	//! @brief Gets view matrix as 4x4 float array
	//! @warning Asserts if view matrix needs updating (check mViewDirty)
	DirectX::XMFLOAT4X4 GetView4x4f()const;

	//! @brief Gets projection matrix as 4x4 float array
	DirectX::XMFLOAT4X4 GetProj4x4f()const;

	//--------------------------------------------------------------
	// Camera Movement
	//--------------------------------------------------------------
	
	//! @brief Moves camera along its right vector
	//! @param d Distance to strafe (positive = right, negative = left)
	void Strafe(float d);

	//! @brief Moves camera along its look vector
	//! @param d Distance to walk (positive = forward, negative = backward)
	void Walk(float d);

	//! @brief Moves camera along its up vector
	//! @param d Distance to move vertically (positive = up, negative = down)
	void Pedestal(float d);

	//--------------------------------------------------------------
	// Camera Rotation
	//--------------------------------------------------------------
	
	//! @brief Pitches camera around its right vector
	//! @param angle Rotation angle in radians (positive = upward pitch)
	void Pitch(float angle);

	//! @brief Rotates camera around world Y-axis
	//! @param angle Rotation angle in radians (positive = clockwise)
	void RotateY(float angle);

	//! @brief Rolls camera around its look vector
	//! @param angle Rotation angle in radians (positive = clockwise)
	void Roll(float angle);

	//--------------------------------------------------------------
	// View Matrix Management
	//--------------------------------------------------------------

	//! @brief Updates view matrix if camera parameters have changed
	//! @details Call after modifying position/orientation to refresh view matrix
	void UpdateViewMatrix();

private:
	//! @brief Camera position in world space
	DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };

	//! @brief Right vector (X-axis) of camera space
	DirectX::XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };

	//! @brief Up vector (Y-axis) of camera space
	DirectX::XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };

	//! @brief Look vector (Z-axis) of camera space
	DirectX::XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	//--------------------------------------------------------------
	// Frustum Properties
	//--------------------------------------------------------------

	//! @brief Near clipping plane distance
	float mNearZ = 0.0f;

	//! @brief Far clipping plane distance
	float mFarZ = 0.0f;

	//! @brief Aspect ratio (width/height)
	float mAspect = 0.0f;

	//! @brief Vertical field of view in radians
	float mFovY = 0.0f;

	//! @brief Height of near plane view window
	float mNearWindowHeight = 0.0f;

	//! @brief Height of far plane view window
	float mFarWindowHeight = 0.0f;

	//! @brief View matrix dirty flag
	bool mViewDirty = true;

	//--------------------------------------------------------------
	// Cached Matrices
	//--------------------------------------------------------------

	//! @brief Cached view matrix
	DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();

	//! @brief Cached projection matrix
	DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};

#endif // CAMERA_H
