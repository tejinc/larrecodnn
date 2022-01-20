include_directories($ENV{TENSORFLOW_INC}/absl)

art_make(LIB_LIBRARIES larreco_RecoAlg_ImagePatternAlgs_DataProvider
                       larrecodnn_ImagePatternAlgs_Keras
                       larrecodnn_ImagePatternAlgs_Tensorflow_TF
                       larcore_Geometry_Geometry_service
                       larcorealg_Geometry
                       lardataalg_DetectorInfo
                       lardataobj_RecoBase
                       larreco_Calorimetry
                       nusimdata::SimulationBase
                       art::Framework_Core
                       art::Framework_Principal
                       art::Framework_Services_Registry
                       art_root_io::tfile_support
                       art_root_io::TFileService_service
                       art::Framework_Services_Optional_RandomNumberGenerator_service
                       art::Persistency_Common
                       art::Persistency_Provenance
                       art::Utilities
                       canvas::canvas
                       messagefacility::MF_MessageLogger
                       fhiclcpp::fhiclcpp
                       cetlib::cetlib cetlib_except
                       ROOT::Core
                       ROOT::Minuit
                       ROOT::Minuit2
        )

install_headers()
install_fhicl()
install_source()
