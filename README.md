# Instruction sur l'utilisation de Cmake et de tests unitaires

## Clone git repo
Pour commencer ce tutoriel, vous devez commencer par cloner le repo sur votre ordinateur.
1. Ouvrer un terminal
1. Aller dans le dossier où vous voulez cloner ce repo
1. Inscriver la commande suivante dans le terminal
 ```   
    git clone https://github.com/AER8875-Projet-integrateur-IV/Cmake_unittest_tutorial.git
```
## Écriture de tests unitaires
### Pourquoi en écrire
https://www.excella.com/insights/why-is-unit-testing-important

### Comment en écrire
Note: [Documentation](https://github.com/google/googletest)

Note: Copier l'arborescence de votre code et utiliser test_ comme suffixe
```
Euler
|
|
|
|----- src/        
|     |
|     |---- solvers/
|     |		|
|     |		|----- eulerexplicit
|     |		|	|----- code.cpp
|     |		|	|----- code.hpp
|     |		|
|     |		|----- jst       
|     |		|	|----- code.cpp
|     |		|	|----- code.hpp
|     |
|     |----- gui/
|     |        |
|     |        |----- code.cpp
|     |        |----- code.hpp
|     |
|     |----- mesh/
|     |        |
|     |        |----- code.cpp
|     |        |----- code.hpp
|
|----- tests/        
|     |
|     |---- solvers/
|     |		|
|     |		|----- eulerexplicit
|     |		|	|----- test_code.cpp
|     |		|
|     |		|----- jst       
|     |		|	|----- test_code.cpp
|     |
|     |----- gui/
|     |        |
|     |        |----- test_code.cpp
|     |
|     |----- mesh/
|     |        |
|     |        |----- test_code.cpp
```

1. Aller dans le dossier projet
2. Incluer Google test en submodule
```
git submodule add https://github.com/google/googletest.git extern/googletest
```
3. Écrire le test pour la fonction division du fichier src/InterestingDirectory/Division.cpp à tests/InterestingDirectory/test_Division.cpp. Dans notre cas, on pourrait tester une division standard et une division par zéro.
```
#include <gtest/gtest.h>    // Toujours inclu
#include <gmock/gmock.h>    // Toujours inclu
#include "../../src/InterestingDirectory/Division.h"


TEST(Test_Division, division_standard_2p4) {
    int a = 2;
    int b = 4;
    double c = Division(a,b);
    EXPECT_EQ(c, 0.5);
}

TEST(Test_Division, division_par_zer0) {
    EXPECT_EQ(Division(1,0), 0); 
}
```

## CMakeList.txt 
* Un fichier CMakeLists.txt doit être au root du projet
* Un fichier CMakeLists.txt doit être dans chaque sous dossier de src/ et tests/

### ./CMakeLists.txt 
Le fichier CMakeLists.txt au root du projet contient les informations générals du projet. 
1. Créer le fichier avec 
    touch CMakeLists.txt
1. Ouvrir le fichier et y inscrire le code suivant
```
    cmake_minimum_required(VERSION 3.10)

    project(Tuto_Cmake LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_subdirectory(src)

    # Build tests
    option(PACKAGE_TESTS "Build the tests" ON)
    if(PACKAGE_TESTS)
        # include(CTest)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(tests)
    endif()
```

### src/CMakeLists.txt
Le dossier src/. comprend le CMakeLists.txt qui défini l'exécutable, il nécessite donc les commandes suivantes:

1. une commande add_executable() pour le programme main.
1. une commande add_subdirectory() pour chaque sous dossier.
1. une commande target_link_libraries() pour chaque library défini dans un sous dossier.

exemple du fichier src/CMakeLists.txt dans notre cas:
```
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

add_subdirectory(InterestingDirectory)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC InterestingDirectory)
```

### src/*/CMakeLists.txt
Les sous dossiers de src doivent comprendre les liens vers les fichiers cpp utilisés et nécessitent donc les commandes suivantes:

1. une commande add_library()  
1. une commande target_include_directories()

exemple du fichier src/InterestingDirectory/CMakeLists.txt dans notre cas:
```
add_library(Division Division.cpp)

target_include_directories(Division PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```

### tests/CMakeLists.txt
Ce fichier devrait toujours être le même à l'exception de la liste de sous dossier.
Exemple:
```
###################################################
# This should be at ./tests/CMakeLists.txt

macro(package_add_test TESTNAME SOURCE DEPENDANCIES)
    message("adding test")
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${SOURCE})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${DEPENDANCIES})
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
                         # set a working directory so your project root so that you can find test data via paths relative to the project root
                         WORKING_DIRECTORY ${PROJECT_DIR}
                         PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()


add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# mark_as_advanced(
#     BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
#     gmock_build_tests gtest_build_samples gtest_build_tests
#     gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
# )


###################################################

# List of directories
add_subdirectory(InterestingDirectory)
```
### tests/*/CMakeLists.txt
Ces fichiers permettre de définir les exécutables de test à l'aide du macro add test. Un exemple est fourni ci-bas.
```
set(TESTNAME test_Division)                                 # Name for this test executable
set(SOURCE test_Division.cpp                                # List of source files to be included in the executable
            # other cpp file
            # and an other one! 
            )
set(DEPENDANCIES Division                                   # List of all the tests dependancies (every non standard #include )
            # Other library
            # Ouioui vous pouvez en mettre autant que necessaire
            )
package_add_test(${TESTNAME} ${SOURCE} ${DEPENDANCIES})     # Call to the macro in ./tests/CMakeLists.txt
```
## Compilation
Tout est maintenant en place pour compiler le code.
1. Aller dans le dossier build de votre projet
```
cd build
```
2. Créer vos makefile
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
``` 
3. compiler
```
make
```
4. Pour rouler le code, l'exécutable devrait se trouver à ./build/src/NomDuProjet

1. Pour rouler les tests, vous avez deux options:
    1. Rouler tous les tests
    ```
    make test
    ```
    1. Rouler seulement un groupe de tests: rouler l'exécutable se trouvant à build/tests/DossierTest/NomDuTest. exemple: build/tests/InterestingDirectory/test_Division 
