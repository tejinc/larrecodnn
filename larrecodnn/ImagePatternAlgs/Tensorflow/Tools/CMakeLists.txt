include(lar::PointIdAlgorithm)
cet_build_plugin(PointIdAlgTf lar::PointIdAlgorithm
  LIBRARIES PRIVATE
  larrecodnn::ImagePatternAlgs_Tensorflow_TF
  TensorFlow::framework
)

include(lar::WaveformRecognizer)

cet_build_plugin(WaveformRecogTf lar::WaveformRecognizer
  LIBRARIES PRIVATE
  larrecodnn::ImagePatternAlgs_Tensorflow_TF
  art_plugin_support::toolMaker
  messagefacility::MF_MessageLogger
  TensorFlow::framework
)  

install_source()
