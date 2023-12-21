#pragma once

class Viewport
{
public:
	Viewport();
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	// Viewport를 세팅하기 위한 함수를 선언합니다.
	void RSSetViewport();

	// 값을 세팅하기 위한 함수를 선언합니다. (생성과 동시 X -> 추후 값 세팅이 필요한 경우)
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return _vp.Width; }
	float GetHeight() { return _vp.Height; }

	// 2D -> 3D or 3D -> 2D로 좌표를 변환해주기 위한 함수들을 선언합니다.
	Vec3 Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);
	Vec3 UnProject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);

private:
	D3D11_VIEWPORT _vp;
};

