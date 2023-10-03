#pragma once

// 기하학적인 도형을 표현하는 클래스입니다.
// * 도형에 대한 Vertex 정보들과 Index 정보들을 관리하는 클래스입니다.
// * Vertex 정보는 여러 개가 존재할 수 있습니다.
//   -> 템플릿 문법을 적용시켜줍니다.

template<typename T>
class Geometry
{
public:
	Geometry() {}
	~Geometry() {}

	// 정점들의 개수를 반환하기 위한 함수를 정의합니다.
	uint32 GetVertexCount() { return static_cast<uint32>(_vertices.size()); }
	// 정점들의 데이터를 반환하기 위한 함수를 정의합니다.
	void* GetVertexData() { return _vertices.data(); }
	// 정점들을 저장하고 있는 벡터 컨테이너를 반환하기 위한 함수를 정의합니다.
	const vector<T>& GetVertices() { return _vertices; }
	
	// 인덱스들의 개수를 반환하기 위한 함수를 정의합니다.
	uint32 GetIndexCount() { return static_cast<uint32>(_indices.size()); }
	// 인덱스들의 데이터를 반환하기 위한 함수를 정의합니다.
	void* GetIndexData() { return _indices.data(); }
	// 인덱스들을 저장하고 있는 벡터 컨테이너를 반환하기 위한 함수를 정의합니다.
	const vector<uint32>& GetIndices() { return _indices; }


	// 정점을 하나씩 추가하기 위한 함수를 정의합니다.
	void AddVertex(const T& vertex) { _vertices.push_back(vertex); }
	// 정점들을 기존 컨테이너에 한 번에 추가하기 위한 함수를 정의합니다.
	void AddVertices(const vector<T>& vertices) { _vertices.insert(_vertices.end(), vertices.begin(), vertices.end()); }
	// 정점들을 한 번에 설정하기 위한 함수를 정의합니다.
	void SetVertices(const vector<T>& vertices) { _vertices = vertices; }

	// 인덱스를 추가하기 위한 함수를 정의합니다.
	void AddIndex(uint32 index) { _indices.push_back(index); }
	// 인덱스들을 기존 컨테이너에 한 번에 추가하기 위한 함수를 정의합니다.
	void AddIndices(const vector<uint32>& indices) { _indices.insert(_indices.end(), indices.begin(), indices.end()); }
	// 인덱스들을 한 번에 설정하기 위한 함수를 정의합니다.
	void SetIndices(const vector<uint32>& indices) { _indices = indices; }


private:
	// 정점들과 도형을 구성하는 인덱스들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<T> _vertices;
	vector<uint32> _indices;
};

