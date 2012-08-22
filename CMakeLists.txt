add_executable (mmtptest main.cpp src/CCurl.cpp src/CParser.cpp src/CCommand.cpp)
target_link_libraries(mmtptest curl pcrecpp)
