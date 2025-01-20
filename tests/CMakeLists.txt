find_package(Qt6 COMPONENTS Test REQUIRED)

function (add_standard_test test_name test_src)
    add_executable(${test_name} ${test_src} ${testing_additional})
    target_link_libraries(${test_name} Qt6::Test)
    add_test(${test_name} ${test_name})
    set_property(TARGET ${test_name} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
endfunction ()

add_subdirectory(tst-pdf-model)

