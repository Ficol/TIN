set(SOURCE_FILES
        server_main.cpp
        Server.cpp
        Game.cpp
        Client.cpp
	Player.cpp
	Bullet.cpp
	Encryptor.cpp
	common.h
        )
add_library(${CMAKE_PROJECT_NAME}_lib
        ${SOURCE_FILES}
        )
  
add_executable(${CMAKE_PROJECT_NAME}_run
        ${SOURCE_FILES}
        )
