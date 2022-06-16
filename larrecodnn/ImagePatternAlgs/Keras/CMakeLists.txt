
find_package(TBB REQUIRED EXPORT)
art_make(LIB_LIBRARIES TBB::tbb) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

add_subdirectory(Tools)

install_headers()
install_source()
