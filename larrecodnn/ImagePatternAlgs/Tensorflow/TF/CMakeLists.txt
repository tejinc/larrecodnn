include_directories( $ENV{TENSORFLOW_INC}/absl ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

art_make(
          LIB_LIBRARIES
			fhiclcpp::fhiclcpp
			cetlib::cetlib cetlib_except
			TENSORFLOW
			protobuf::libprotobuf
			FOR_TENSORFLOW
      #rt #workaround for larsoft issue 17960
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
