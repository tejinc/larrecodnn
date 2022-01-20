art_make(
          LIB_LIBRARIES
          fhiclcpp::fhiclcpp
          cetlib::cetlib cetlib_except
          art::Framework_Services_Registry
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
          TRITON::grpcclient
        )

install_headers()
install_source()
