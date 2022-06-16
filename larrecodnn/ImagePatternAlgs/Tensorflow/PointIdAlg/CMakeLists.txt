include_directories($ENV{TENSORFLOW_INC}/absl) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

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
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()
