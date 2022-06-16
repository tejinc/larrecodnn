art_make( MODULE_LIBRARIES
	lardata_ArtDataHelper
        lardataobj_RawData
	larrecodnn_ImagePatternAlgs_Tensorflow_PointIdAlg
        art_Framework_Services_System_TriggerNamesService_service
        larsim_MCCheater_ParticleInventoryService_service
        nurandom_RandomUtils_NuRandomService_service
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()
