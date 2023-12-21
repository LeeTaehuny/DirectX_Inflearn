#include "pch.h"
#include "Viewport.h"

Viewport::Viewport()
{
	// 기본 값으로 설정합니다.
	Set(800, 600);
}

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	// 전달받은 값으로 설정합니다.
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
}

void Viewport::RSSetViewport()
{
	// viewport를 RS단계에 연결시켜줍니다.
	DC->RSSetViewports(1, &_vp);
}

void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	// 전달받은 값을 viewport에 저장합니다.
	_vp.TopLeftX = x;
	_vp.TopLeftY = y;
	_vp.Width = width;
	_vp.Height = height;
	_vp.MinDepth = minDepth;
	_vp.MaxDepth = maxDepth;
}

Vec3 Viewport::Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	// 변환 행렬을 생성합니다. (WVP)
	Matrix wvp = W * V * P;

	// 전달받은 pos는 로컬 좌표입니다. (만약 월드 좌표라면 매개변수로 W를 Identity로 전달)
	// * Transform(Coord) : 좌표를 포함한 연산
	// * TransformNormal  : 방향만 연산
	//   -> pos에 변환 행렬을 적용시켜줍니다. (projection이 된 후의 좌표로 넘어가기)
	Vec3 p = Vec3::Transform(pos, wvp);

	// projection된 좌표를 Screen Space로 변환해줍니다.
	p.x = (p.x + 1.0f) * (_vp.Width / 2) + _vp.TopLeftX;
	p.y = (-p.y + 1.0f) * (_vp.Height / 2) + _vp.TopLeftY;
	p.z = p.z * (_vp.MaxDepth - _vp.MinDepth) + _vp.MinDepth;

	return p;
}

Vec3 Viewport::UnProject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	// 2D -> 3D로 변환하는 과정입니다.
	// * Project 함수의 반대 방향으로 연산을 진행합니다.
	
	// Screen Space의 좌표
	Vec3 p = pos;

	// Projection Space 좌표로 변경
	p.x = 2.f * (p.x - _vp.TopLeftX) / _vp.Width - 1.f;
	p.y = -2.f * (p.y - _vp.TopLeftY) / _vp.Height + 1.f;
	p.z = ((p.z - _vp.MinDepth) / (_vp.MaxDepth - _vp.MinDepth));

	// WVP의 역행렬을 구해줍니다.
	Matrix wvp = W * V * P;
	Matrix wvpInv = wvp.Invert();

	// WVP의 역행렬을 통해 Projection Space 좌표를 Local Space좌표로 가져옵니다.
	p = Vec3::Transform(p, wvpInv);

	return p;
}
