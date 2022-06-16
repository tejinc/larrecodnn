art_make(
          TOOL_LIBRARIES
          larreco_RecoAlg_ImagePatternAlgs_DataProvider
          larcorealg_Geometry
          larrecodnn_ImagePatternAlgs_NuSonic_Triton
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
          TRITON::grpcclient
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_source()
