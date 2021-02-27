# 3D gomoku

언리얼 3D 오목 프로젝트

# 데모 동영상

> Youtube

[![이미지 텍스트](http://img.youtube.com/vi/fmlWYqndRac/0.jpg)](https://www.youtube.com/watch?v=fmlWYqndRac)

# 스크린샷
![enter image description here](https://lh3.googleusercontent.com/2Z51NysMWAHzW36N30JXdl8oTWS1n5XSSezce6lRY_w7KXmB6hvnKn8sCL6rAqmsmPOfk8DlEmUcQAmqSjDW855edl-WgQH9RtqsyuYnls8SiVK7KIhfB605Vrpz4FPgwib-xilb=w2400)

# 설명
 - 온라인, AI, 오프라인 대전이 가능한 3D 오목 게임입니다.
 - 블루프린트와 C++의 5:5의 비율로 제작되었습니다.
 - Game UI와 유저와의 상호작용 Event는 모두 블루프린트로 작성되었습니다.
 - AI는 [DahamChoi/omok: 오목 인공지능 제작 프로젝트 (github.com)](https://github.com/DahamChoi/omok)에서 활용한 탐색구조를 활용하여 모델추론을 통해 획득하는 승률을 PlayOut(재귀함수를 통한 완전탐색)으로 획득하도록 변경하여 제작
 - 온라인 파트는 [getnamo/socketio-client-ue4: socket.io client plugin for Unreal Engine 4 (github.com)](https://github.com/getnamo/socketio-client-ue4)를 활용하여 작성, 비동기식 이벤트 구조를 활용하여 Event Bind방식으로 모두 블루프린트를 사용해 작성
# 코드

### GameAPIActor
 - 블루프린트와의 상호작용을 위해 만들어진 C++ 네이티브 코드 접근 인터페이스

### Node
 - 몬테카틀로 탐색 트리 노드 클래스

### State
 - 게임 보드상황 저장
 - 게임 상황 파악 함수 제공
 - AI 추론을 도와주는 smartLegalActions 제공

### AIAsyncTask
 - AI연산 스레드 클래스
 - 핵심 AI 추론 코드, 100회 추론
 

