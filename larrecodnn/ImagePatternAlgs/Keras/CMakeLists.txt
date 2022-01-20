
find_package(TBB REQUIRED PUBLIC)
art_make(LIB_LIBRARIES TBB::tbb)

add_subdirectory(Tools)

install_headers()
install_source()
