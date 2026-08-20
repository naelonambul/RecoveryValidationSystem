# InternalAgent — Guest Validation Agent

`InternalAgent`는 각 Windows VirtualBox VM 내부에서 실행되는 **Guest-side validation worker**입니다.

Host Controller의 command를 받아 파일을 수신하고, 실행 전후의 파일 상태를 기록·비교한 뒤 결과를 다시 Host에 보고합니다.

## Responsibilities

- Host Controller에 TCP client로 연결
- Guest OS 버전을 Host에 보고
- 검증 전 파일 목록과 hash를 baseline으로 저장
- Host가 전달한 파일 metadata와 데이터를 수신
- 테스트 단계와 복구 단계를 별도 worker thread에서 실행
- 대상 디렉터리 변경을 감지
- 실행 후 파일 이름/hash map을 다시 생성
- baseline과 실행 후 상태를 비교해 결과 수치를 Host에 전달
- 종료 명령 수신 후 socket과 Winsock resource 정리

## Processing Flow

```text
Start Agent
    ↓
Collect baseline file hashes
    ↓
Connect to Host
    ↓
READY
    ↓
Receive test target
    ↓
Receive recovery tool
    ↓
Run test stage
    ↓
Collect / compare changed file state
    ↓
Report sample result
    ↓
Run recovery stage
    ↓
Compare recovered state with baseline
    ↓
Report recovery result
    ↓
STOP
```

## Key Implementation

### Command dispatch table

`InternalAgent.cpp`는 수신한 command code를 함수 포인터 테이블에 연결해 단계별 처리를 분리했습니다.

```cpp
COMD_FUNC pAgentFuc[600] = { 0 };
```

이를 통해 파일 수신, 실행, 상태 보고, health check, stop 등의 동작을 command별 handler로 나눴습니다.

### File reception

Host에서 먼저 `FILEINFO`를 받고, 이후 socket에서 파일 데이터를 반복 수신해 Win32 `WriteFile`로 저장합니다. 받은 byte 수를 metadata의 file size와 비교해 전송 완료 시점을 판단합니다.

### Process and thread lifecycle

테스트 단계와 복구 단계는 Win32 thread와 Event를 이용해 실행합니다. 실제 프로세스 실행에는 `CreateProcess`와 `WaitForSingleObject`를 사용하고, 정해진 대기 시간 이후에는 process lifecycle을 종료하도록 구성했습니다.

### File-state comparison

사용자 Documents 영역의 파일을 열거하고 각 파일의 MD5 값을 계산해 다음 세 상태를 `std::map<string, string>`에 보관합니다.

- `Sample_Before_Name_Hash`
- `Sample_After_Name_Hash`
- `Tool_After_Name_Hash`

파일 이름과 hash를 비교해 테스트 단계에서 변화한 파일 수와 복구 단계 이후 baseline과 일치하는 파일 수를 계산합니다.

> MD5는 이 prototype에서 파일 상태 식별 목적으로 사용했습니다. 현대적인 cryptographic integrity 검증을 위한 선택은 아닙니다.

### Directory change detection

`FindFirstChangeNotification`과 `WaitForSingleObject`를 이용해 대상 디렉터리의 write change를 감지하고, 작업 완료 시점에 파일 상태를 다시 수집합니다.

## Important Files

| File | Role |
| --- | --- |
| `InternalAgent/InternalAgent.cpp` | main loop, TCP 연결, command dispatch 및 workflow |
| `InternalAgent/InternalAgent.h` | 상태 객체, hash/file comparison, process 및 directory utility |
| `InternalAgent/MyAppProtocol.h` | Host와 공유하는 command 및 file metadata 구조 |

## Historical Environment

- Visual Studio 2015 / Visual Studio 14
- Win32 API
- Winsock 2
- Windows CryptoAPI
- Windows 7 / 8.1 / 10 guest VM

## Limitations

이 Agent는 격리된 실험 환경을 위한 2019년 prototype입니다.

- Host endpoint 등 일부 환경 설정이 코드에 고정되어 있습니다.
- TCP payload는 C/C++ 구조체를 직접 송수신하므로 serialization/versioning 계층이 없습니다.
- 통신 인증 및 암호화가 구현되어 있지 않습니다.
- 테스트 실행 파일은 포트폴리오 정리본에서 제거했습니다.

[← System Overview](../README.md)
