
add_executable(chapter-35 main.cpp
    SpriteIdleState.h
    SpriteIdleState.cpp
    SpriteForwardState.h
    SpriteForwardState.cpp
    SpriteRotateState.h
    SpriteRotateState.cpp
    SpriteErrorState.h
    SpriteErrorState.cpp
    WayFindingScene.cpp
    WayFindingScene.h
    WayFindingTank.cpp
    WayFindingTank.h
    WayFindingTileVisitor.cpp
    WayFindingTileVisitor.h
    WayFindingState.cpp
    WayFindingState.h
    )

target_link_libraries(chapter-35 framework sfml-system sfml-graphics sfml-window)
