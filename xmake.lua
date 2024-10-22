add_rules("mode.release", "mode.debug")

add_languages("c17")

target("tcstring")
    set_kind("headeronly")
    add_headerfiles("include/TCString.h")
target_end()

target("MainTest")
    set_kind("binary")
    add_files("src/main.c")
    add_deps("tcstring")
    add_includedirs("include")
target_end()