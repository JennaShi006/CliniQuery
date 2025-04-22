# CliniQuery

# Backend Libraries Used
# Crow library for C++ https://github.com/CrowCpp/Crow.git
## Dependencies: 
### Json library for C++ https://github.com/nlohmann/json
### Asios library for C++ https://github.com/chriskohlhoff/asio

# Before you begin
### check that you have the most updated version of MinGW, you might be required to reinstall it and set the path again
### cd into cpp_backend and run the command : git submodule add https://github.com/CrowCpp/Crow.git external/crow 
### Ensure you have node.js installed to use the npm package

# to run
### open 2 terminals, cd into frontend and cpp_backend
### to start frontend, run npm install to install dependencies, and then run : npm start
### if you don't have nodejs installed, you might need to install it and set it in your path variables 
### for backend, build your own build files with cmake -S . -B build -G "MinGW Makefiles"
### then, cd into cpp_backend/build and run : cmake --build .
### then, run ./CliniQuery to start the backend.

# While Running
### Make sure to refresh the page between searches!

# Troubleshooting
### If it is not building and it says cannot find "crow.h", try to change CMakeLists.txt from include_directories to target_include_directories.
### if attempting to rebuild, delete the build folder in between attempts, and ensure that the build folder and CMakeLists.txt is located inside cpp_backend.