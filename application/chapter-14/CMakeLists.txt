
add_executable(chapter-14 main.cpp SpriteAnimationState.h SpriteAnimationState.cpp SpriteForwardState.h SpriteForwardState.cpp SpriteRotateState.h SpriteErrorState.h MineSweeperScene.cpp MineSweeperScene.h)
target_link_libraries(chapter-14 framework sfml-system sfml-graphics sfml-window)
