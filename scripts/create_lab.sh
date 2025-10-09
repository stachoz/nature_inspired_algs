#!/bin/bash
# Skrypt do tworzenia nowego laboratorium
# Należy go uruchomić z katalogu głownego projektu, aby katalogi zostały utowrzone we właściwym miejscu
# Użycie: ./scripts/create_lab.sh lab_03

LAB_NAME=$1

if [ -z "$LAB_NAME" ]; then
  echo "Użycie: ./Scripts/create_lab.sh lab_XX"
  exit 1
fi

mkdir -p $LAB_NAME/{src,test,results}

cat > $LAB_NAME/CMakeLists.txt <<EOF
add_executable(${LAB_NAME}
    src/main.cpp
)

target_link_libraries(${LAB_NAME} PRIVATE common_libs)
EOF

cat > $LAB_NAME/src/main.cpp <<EOF
#include <iostream>

int main() {
    std::cout << "${LAB_NAME} — Nature Inspired Algorithms" << std::endl;
    return 0;
}
EOF

echo "Utworzono nowy folder: $LAB_NAME"

cat >> CMakeLists.txt << EOF
add_subdirectory($LAB_NAME)
EOF
