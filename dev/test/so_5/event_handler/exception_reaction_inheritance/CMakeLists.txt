add_executable(_unit.test.event_handler.exception_reaction_inheritance main.cpp)
target_link_libraries(_unit.test.event_handler.exception_reaction_inheritance so.${SO_5_VERSION})
add_test(_unit.test.event_handler.exception_reaction_inheritance _unit.test.event_handler.exception_reaction_inheritance)
