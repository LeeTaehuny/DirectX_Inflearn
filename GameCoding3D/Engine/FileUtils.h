#pragma once

// 파일의 쓰기/읽기를 구분하기 위한 열거형을 선언합니다.
enum FileMode : uint8
{
	Write,
	Read,
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	// 파일의 경로와 모드를 받아 활용하기 위한 함수를 선언합니다.
	void Open(wstring filePath, FileMode mode);

	// 파일을 쓰기 위한 함수를 정의합니다.
	// * 파일을 어떤 형태로 사용할지 현재 상황에서는 모릅니다. (int? short?.. 등 사이즈별로 넣어줘야 하기 때문)
	// * 템플릿으로 생성합니다.
	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	// * String과 다른 동적으로 할당되는 친구들은 따로 빼줘야 합니다.
	// ** 실수로 윗 버전으로 사용하기를 막기 위한 템플릿 특수화 코드 (String을 사용한 경우에만 아래 코드 실행)
	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}

	void Write(void* data, uint32 dataSize);
	void Write(const string& data);

	// 파일을 읽기 위한 함수를 정의합니다.
	// * 위와 동일한 이유로 템플릿으로 생성합니다.
	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	void Read(void** data, uint32 dataSize);
	void Read(OUT string& data);

private:
	// 파일 핸들러를 저장하기 위한 변수를 선언합니다.
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

