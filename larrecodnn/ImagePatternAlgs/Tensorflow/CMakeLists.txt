include_directories ( $ENV{TENSORFLOW_INC}/eigen ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

add_subdirectory(PointIdAlg)
add_subdirectory(Tools)
add_subdirectory(TF)
add_subdirectory(Modules)

install_headers()
install_source()

