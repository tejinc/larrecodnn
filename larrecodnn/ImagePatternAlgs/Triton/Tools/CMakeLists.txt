art_make(
          TOOL_LIBRARIES
          larreco_RecoAlg_ImagePatternAlgs_DataProvider
          larcorealg_Geometry
          larrecodnn_ImagePatternAlgs_NuSonic_Triton
          ROOT::Core
          canvas::canvas
          messagefacility::MF_MessageLogger
          TRITON::grpcclient
        )

install_source()
