#include "pch.h"
#include "Animation.h"
#include "Texture.h"

Animation::Animation() : Super(ResourceType::Animation)
{
}

Animation::~Animation()
{
}

void Animation::Load(const wstring& path)
{
	// XML 문서를 생성합니다.
	tinyxml2::XMLDocument doc;

	// 입력받은 경로에서 XML 문서를 로드합니다.
	string pathStr = WstrToStr(path);
	XMLError error = doc.LoadFile(pathStr.c_str());
	assert(error == XMLError::XML_SUCCESS);

	// 첫 번째 요소를 찾아줍니다.
	XmlElement* root = doc.FirstChildElement();
	// 첫 번째 요소의 이름, 루프 여부, 파일 경로를 받아옵니다.
	string nameStr = root->Attribute("Name");
	_name = std::wstring().assign(nameStr.begin(), nameStr.end());
	_loop = root->BoolAttribute("Loop");
	_path = path;

	// Load Texture.. (현재는 TODO)

	// 자식 노드의 정보도 읽어줍니다.
	XmlElement* node = root->FirstChildElement();
	for (; node != nullptr; node = node->NextSiblingElement())
	{
		Keyframe keyframe;

		keyframe.offset.x = node->FloatAttribute("OffsetX");
		keyframe.offset.y = node->FloatAttribute("Offsety");
		keyframe.size.x = node->FloatAttribute("SizeX");
		keyframe.size.y = node->FloatAttribute("SizeY");
		keyframe.time = node->FloatAttribute("Time");

		// 받아온 정보를 키 프레임에 추가합니다.
		AddKeyframe(keyframe);
	}
}

void Animation::Save(const wstring& path)
{
	// XML 문서를 생성합니다.
	tinyxml2::XMLDocument doc;

	// 새로운 요소(element)를 생성합니다.
	XMLElement* root = doc.NewElement("Animation");
	// 만들어진 요소를 XML 문서에 추가합니다.
	doc.LinkEndChild(root);

	// wstring을 string 형식으로 변경해서 저장해야 합니다.
	// * 애니메이션의 이름을 string 형식으로 변경합니다.
	string nameStr = WstrToStr(GetName());

	// 키 - 값 형태로 정보를 저장할 것입니다.
	// * Attribute라고 합니다.
	// * 루트 요소에 Name - nameStr.c_str()의 속성을 추가합니다.
	root->SetAttribute("Name", nameStr.c_str());
	// * 루프 여부도 추가합니다.
	root->SetAttribute("Loop", _loop);
	// * 경로도 추가합니다.
	root->SetAttribute("TexturePath", "TODO");

	for (const auto& Keyframe : _keyframes)
	{
		// 새로운 노드를 생성합니다.
		XMLElement* node = doc.NewElement("Keyframe");
		// 루트 노드에 연결합니다.
		root->LinkEndChild(node);

		// 연결한 노드에 들어갈 정보들을 추가합니다.
		node->SetAttribute("OffsetX", Keyframe.offset.x);
		node->SetAttribute("OffsetY", Keyframe.offset.y);
		node->SetAttribute("SizeX", Keyframe.size.x);
		node->SetAttribute("SizeY", Keyframe.size.y);
		node->SetAttribute("Time", Keyframe.time);
	}

	// XML 문서를 만들었으므로 원하는 경로에 저장합니다.
	string pathStr = WstrToStr(path);
	auto result = doc.SaveFile(pathStr.c_str());

	// result가 XML_SUCCESS라면? -> 성공
	assert(result == XMLError::XML_SUCCESS);
}

Vec2 Animation::GetTextureSize()
{
	return _texture->GetSize();
}

const Keyframe& Animation::GetKeyframe(int32 index)
{
	return _keyframes[index];
}

int32 Animation::GetKeyframeCount()
{
	return static_cast<int32>(_keyframes.size());
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}
