include_directories( $ENV{TENSORFLOW_INC}/absl ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

art_make(
          TOOL_LIBRARIES
          larreco_RecoAlg_ImagePatternAlgs_DataProvider
          larrecodnn_ImagePatternAlgs_Tensorflow_TF
          larcorealg_Geometry
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_source()
