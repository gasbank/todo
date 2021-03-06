TODO 클라이언트
===========================

## Windows

grpc 거대한 우주다...

1. choco 설치
2. bazel 설치
3. ```bazel build :all``` grpc 직접 빌드
4. ```error: invalid command 'bdist_wheel'``` 오류 나면 ```pip install wheel``` 필요
5. 또 실패!

```
ERROR: C:/users/gasbank/_bazel_gasbank/htdro5hd/external/com_github_cares_cares/BUILD.bazel:79:8: Executing genrule @com_github_cares_cares//:ares_build_h failed (Exit -1). Note: Remote connection/protocol failed with: execution failed
Action failed to execute: java.io.IOException: ERROR: src/main/native/windows/process.cc(202): CreateProcessW("c:\tools\msys64\usr\bin\bash.exe" -c "source external/bazel_tools/tools/genrule/genrule-setup.sh; cat third_party/cares/ares_build.h > bazel-out/x64_windows-fastbuild/bin/external/com_github_cares_cares/ares_build.h"): 지정된 파일을 찾을 수 없습니다.
```

헛헛... 그렇다면 다른 길을 찾자. ```vcpkg``` 출동! 

7. ```https://github.com/microsoft/vcpkg``` 가서 vcpkg 설치
6. ```vcpkg install grpc```

vcpkg 사용하려면 cmake 실행 시 다음 인자 추가로 붙여 줘야하는건가? ```-DCMAKE_TOOLCHAIN_FILE=C:/users/gasbank/vcpkg/scripts/buildsystems/vcpkg.cmake```

```vcpkg install grpc``` 결과 메시지. 성공인가?!

```
Total elapsed time: 16.53 min

The package grpc:x86-windows provides CMake targets:

    find_package(gRPC CONFIG REQUIRED)
    # Note: 8 target(s) were omitted.
    target_link_libraries(main PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

    find_package(modules CONFIG REQUIRED)
    target_link_libraries(main PRIVATE c-ares::cares)

```

vcpkg 기본적으로 x86 아키텍처용만 가져오는데, x64용 가져오고 싶으면 라이브러리명 뒤에 :x64-windows 붙여야 하는 것 같다.


```
Active code page: 65001
PS C:\WINDOWS\system32> cd C:\users\gasbank\
PS C:\users\gasbank> cd .\vcpkg\
PS C:\users\gasbank\vcpkg> .\vcpkg.exe integrate install
Applied user-wide integration for this vcpkg root.

All MSBuild C++ projects can now #include any installed libraries.
Linking will be handled automatically.
Installing new libraries will make them instantly available.

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/users/gasbank/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

결론 1: `vcpkg`쓰면 만사가 편하다.

결론 2: `vcpkg`관련해서 수정사항 생기면 CMake 빌드 디렉토리 완전 삭제 후 다시 시작하자. 중간에 변경된 사항을 제대로 잡지 못하는 것 같다.

## macOS

되는지 안되는지 확인이 필요

### Homebrew

설치 안되어 있다면 적절히 검색해서 설치. `brew` 커맨드 사용함.

### gRPC

```
$ brew install autoconf automake libtool pkg-config
$ git clone --recurse-submodules -b v1.30.0 https://github.com/grpc/grpc
$ cd grpc
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF ../..
$ make -j
$ make install
$ popd
```

### 클라이언트

`client` 디렉토리로 전환 후 다음 명령어 실행

```
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
```

빌드 후 `NetworkWorld.app` 디렉토리가 나오면 정상.
이는 macOS 앱 형태이다.
허나 실행하면 바로 크래시.
바로 참조하는 파일 위치와 작업 디렉토리가 다르기 떄문.
천천히 해결하자.
빌드가 된 것으로 만족.
