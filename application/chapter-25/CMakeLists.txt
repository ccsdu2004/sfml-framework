
add_executable(chapter-25 main.cpp
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

target_link_libraries(chapter-25 framework sfml-system sfml-graphics sfml-window)
