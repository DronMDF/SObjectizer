add_executable(_unit.test.coop.parent_child_4 main.cpp)
target_link_libraries(_unit.test.coop.parent_child_4 so.${SO_5_VERSION})
add_test(_unit.test.coop.parent_child_4 _unit.test.coop.parent_child_4)
