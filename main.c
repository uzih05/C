#include <stdio.h>
#include <stdlib.h>
#include <process.h> // _spawn, _exec, _cwait 함수 포함.
#include <string.h>
#include <windows.h> // Windows API - GetCurrentProcessId(): 현재 프로세스의 ID를 반환하는 Windows API 함수.

// 이 프로그램은 Windows에서 fork()와 유사한 기능을 _spawnl() 함수를 이용해 구현한 예제입니다.
// Unix에서는 fork() 호출로 부모와 자식 프로세스가 동일한 메모리 공간을 복사하지만,
// Windows에서는 fork()가 없기때문에, 부모 프로세스가 자기 자신을 재실행하여 자식 프로세스를 생성합니다.
// _spawnl() 로 새 프로세스를 생성
// _cwait() 함수로 프로세스가 종료될 때까지 기다림

int main(int argc, char *argv[]) {
    // 명령줄 인수에 "child"를 인식해 자식 프로세스로 간주하고 프로세스를 실행.
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        // 자식 프로세스: 자신의 프로세스 ID를 출력하고 종료.
        printf("자식 프로세스 실행 중 - PID: %d\n", GetCurrentProcessId());
        return 0;
    }

    // 부모 프로세스 코드: "child" 인수가 없으므로 부모로 판단.
    printf("부모 프로세스 시작 - PID: %d\n", GetCurrentProcessId());

    // _spawnl() 함수는 새로운 프로세스를 생성합니다.
    // _P_NOWAIT 옵션을 사용하면 자식 프로세스가 생성된 후 즉시 제어(호출한 쪽으로 실행 제어권)를 반환하여,
    // 부모 프로세스는 자식 프로세스와 병렬로 실행 가능(Block되지 않고 즉각적으로 반환).
    // argv[0]는 실행 파일의 경로이며, "child" 인수를 추가하여 자식 프로세스임을 알린다.
    int childPID = _spawnl(_P_NOWAIT, argv[0], argv[0], "child", NULL);

    // 프로세스 생성에 실패했는지 확인합니다.
    if (childPID == -1) {
        perror("자식 프로세스 생성 실패");
        return 1;
    }

    // 자식 프로세스가 정상적으로 생성되었음을 출력합니다.
    printf("자식 프로세스 생성됨 - 자식 PID: %d\n", childPID);

    // 부모 프로세스는 _cwait() 함수를 사용하여 자식 프로세스가 종료될 때까지 대기합니다.
    _cwait(NULL, childPID, 0);

    // 자식 프로세스가 종료된 후, 부모 프로세스의 종료를 알리는 메시지를 출력합니다.
    printf("부모 프로세스 종료 - PID: %d\n", GetCurrentProcessId());

    return 0;
}
