include_directories( $ENV{TENSORFLOW_INC}/absl )

art_make(
          TOOL_LIBRARIES
          larreco_RecoAlg_ImagePatternAlgs_DataProvider
          larrecodnn_ImagePatternAlgs_Tensorflow_TF
          larcorealg_Geometry
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
        )

install_source()
