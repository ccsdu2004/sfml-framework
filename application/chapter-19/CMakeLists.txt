
add_executable(chapter-19 main.cpp
    InputMessageListener.cpp
    InputMessageListener.h
    SpriteIdleState.h
    SpriteIdleState.cpp
    SpriteForwardState.h
    SpriteForwardState.cpp
    SpriteRotateState.h
    SpriteErrorState.h
    SpriteErrorState.cpp
    MineSweeperScene.cpp
    MineSweeperScene.h
    )

target_link_libraries(chapter-19 framework sfml-system sfml-graphics sfml-window)
