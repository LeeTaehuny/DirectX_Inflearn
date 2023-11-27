#pragma once

// Assimp를 가져와 로드하는 기능의 클래스입니다.
class Converter
{
public:
	Converter();
	~Converter();

private:
	shared_ptr<Assimp::Importer> _importer;
};

