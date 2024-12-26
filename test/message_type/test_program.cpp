#include "message_formats.h"
#include <iostream>
#include <string>
#include <google/protobuf/empty.pb.h>  // Empty 메시지를 포함한 헤더 파일

// 1. rapidjson 테스트
void testRapidJSON() {
    const char* json = R"({"name": "John", "age": 30})";
    rapidjson::Document document;
    document.Parse(json);

    if (document.HasParseError()) {
        std::cerr << "RapidJSON 파싱 오류!" << std::endl;
        return;
    }

    if (document.HasMember("name") && document["name"].IsString()) {
        std::cout << "RapidJSON: name = " << document["name"].GetString() << std::endl;
    }
    if (document.HasMember("age") && document["age"].IsInt()) {
        std::cout << "RapidJSON: age = " << document["age"].GetInt() << std::endl;
    }
}

// 2. tinyxml2 테스트
void testTinyXML2() {
    const char* xml = R"(<person><name>John</name><age>30</age></person>)";
    tinyxml2::XMLDocument doc;
    doc.Parse(xml);

    if (doc.Error()) {
        std::cerr << "TinyXML2 파싱 오류!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* person = doc.FirstChildElement("person");
    if (person) {
        tinyxml2::XMLElement* nameElement = person->FirstChildElement("name");
        if (nameElement) {
            std::cout << "TinyXML2: name = " << nameElement->GetText() << std::endl;
        }

        tinyxml2::XMLElement* ageElement = person->FirstChildElement("age");
        if (ageElement) {
            std::cout << "TinyXML2: age = " << ageElement->GetText() << std::endl;
        }
    }
}

// 3. protobuf 메시지 테스트
void testProtobuf() {
    // google::protobuf::Empty는 빈 메시지 타입입니다.
    google::protobuf::Empty emptyMsg;  // 프로토콜 버퍼의 Empty 메시지 생성

    std::cout << "Protobuf: Empty 메시지 생성 성공!" << std::endl;

    // 실제로 protobuf 메시지를 직렬화하거나 역직렬화 할 수 있습니다.
    // 예시로는 아래와 같은 방식으로 메시지를 직렬화 할 수 있습니다:
    std::string serialized;
    if (emptyMsg.SerializeToString(&serialized)) {
        std::cout << "Protobuf: 직렬화 성공, 크기 = " << serialized.size() << " bytes" << std::endl;
    } else {
        std::cerr << "Protobuf: 직렬화 실패!" << std::endl;
    }
}

int main() {
    std::cout << "RapidJSON 테스트:" << std::endl;
    testRapidJSON();
    
    std::cout << "\nTinyXML2 테스트:" << std::endl;
    testTinyXML2();

    std::cout << "\nProtobuf 테스트:" << std::endl;
    testProtobuf();

    return 0;
}
