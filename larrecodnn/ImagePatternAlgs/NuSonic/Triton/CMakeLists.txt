art_make(
          LIB_LIBRARIES
          fhiclcpp::fhiclcpp
          cetlib::cetlib cetlib_except
          art::Framework_Services_Registry
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
          TRITON::grpcclient
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
