////////////////////////////////////////////////////////////////////////
// Class:       WireClusterFinder
// Plugin Type: producer (art v3_05_00)
// File:        WireClusterFinder_module.cc
//
// Authors:     Tejin Cai (tejinc@yorku.ca)
//
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/make_tool.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larcore/Geometry/Geometry.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"
#include "lardataobj/RecoBase/Wire.h"
#include "larrecodnn/ImagePatternAlgs/ToolInterfaces/IWaveformRecog.h"

#include <memory>

namespace nnet {
  class WireClusterFinder;
  typedef std::pair<int, geo::View_t> ApaView;

  struct cluster{
    double minCentralChan;
    double maxCentralChan;
    double minCentralTick;
    double maxCentralTick;
    int nWires;
    std::vector<recob::Wire> wires;
  };
}

class nnet::WireClusterFinder : public art::EDProducer {
public:
  explicit WireClusterFinder(fhicl::ParameterSet const& pset);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  WireClusterFinder(WireClusterFinder const&) = delete;
  WireClusterFinder(WireClusterFinder&&) = delete;
  WireClusterFinder& operator=(WireClusterFinder const&) = delete;
  WireClusterFinder& operator=(WireClusterFinder&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;

private:
  art::InputTag fWireProducerLabel;

  ApaView GetApaView( art::Ptr<recob::Wire> &wire );

  void SortWirePtrByChannel( std::vector<art::Ptr<recob::Wire>> &vec, bool increasing );
  void PrintWirePtrByChannel( std::vector<art::Ptr<recob::Wire>> &vec );

  //remove single hit wires needs to set dC1 and dT1 for single hit 
  std::vector<art::Ptr<recob::Wire>> FilterWires(std::vector<art::Ptr<recob::Wire>> &vec, int dC1, int dT1, int dCn, int dTn );

  int LogLevel;

  unsigned int fWaveformSize; // Full waveform size
  int fNPlanes;
  int fNChanPerApa;
  int fChannelDistance;
  int fTickDistance;
  int fMinClusterSize;

  //config
  int fLogLevel;
  std::string fWireName;
  bool fDoAssns;


};


nnet::WireClusterFinder::WireClusterFinder(fhicl::ParameterSet const& pset)
  : EDProducer{pset}
  , fWireProducerLabel(pset.get<art::InputTag>("WireProducerLabel", ""))
  //, fWireName(pset.get<std::string>("WireLabel", ""))
{
  if (fWireProducerLabel.empty()) {
    throw cet::exception("WireClusterFinder")
      << "WireProducerLabel is empty";
  }

  const std::string myname = "WireClusterFinder::ctor: ";
  fLogLevel           = pset.get<int>("LogLevel");
  fDoAssns            = pset.get<bool>("DoAssns");
  fNChanPerApa        = pset.get<int>("ChannelPerApa", 2560);
  produces<std::vector<recob::Wire>>();

  auto const* geo = lar::providerFrom<geo::Geometry>();
  fNPlanes = geo->Nplanes();


  // Signal/Noise waveform recognition tool

}

void
nnet::WireClusterFinder::produce(art::Event& e)
{
  //Procedure:
  //1. separate wires into plane and views
  art::Handle<std::vector<recob::Wire>> wireListHandle;
  std::vector<art::Ptr<recob::Wire>> wirelist;
  if (e.getByLabel(fWireProducerLabel, wireListHandle))
    art::fill_ptr_vector(wirelist, wireListHandle);

  if (fLogLevel >= 3)
  {
    std::cout<<"=================================================="<<std::endl;
    std::cout<<"WireProducerLabel: "<<fWireProducerLabel<<std::endl;
  }

  std::unique_ptr<std::vector<recob::Wire>> outwires(new std::vector<recob::Wire>);

  //auto const* geo = lar::providerFrom<geo::Geometry>();

  //##############################
  //### Sorting wires
  //##############################
  std::map<ApaView, std::vector<art::Ptr<recob::Wire>> > apaViewWirePtrVec;

  //std::vector<art::Ptr<recob::Wire>>::iterator p
  //pre-filter wires witout ROI
  for ( auto p = wirelist.begin(); p != wirelist.end(); ++p ) {
    auto wire = *p;
    if ( wire->SignalROI().n_ranges() == 0 ) continue; //continue if no ROI on wire
    ApaView apaView=GetApaView( wire );
    apaViewWirePtrVec[ apaView ].push_back( wire );
  }

  //std::map<ApaView, std::vector<art::Ptr<recob::Wire>> >::iterator p
  for ( auto p = apaViewWirePtrVec.begin(); p != apaViewWirePtrVec.end(); ++p )
  {
    if (fLogLevel>=3 )
    {
      std::cout<<Form("Printing Channel %d, View %d",p->first.first,p->first.second)<<std::endl;
      std::cout<<"Decreasing order"<<std::endl;
      SortWirePtrByChannel( p->second , false);
      PrintWirePtrByChannel( p->second );

      std::cout<<"Increasing order"<<std::endl;
      SortWirePtrByChannel( p->second , true);
      PrintWirePtrByChannel( p->second );
    }
    //Sorting wires by channel
    SortWirePtrByChannel( p->second , true);
    //clustering wire-[p;'
  }

  ////##############################
  ////### Looping over the wires ###
  ////##############################
  //for (unsigned int ich = 0; ich <  wirelist.size(); ++ich) {

  //  std::vector<float> inputsignal(fWaveformSize);

  //  int view = -1;

  //  if (!wirelist.empty()) {
  //    const auto& wire = wirelist[ich];
  //    const auto& signal = wire->Signal();

  //    view = wire->View();

  //    for (size_t itck = 0; itck < inputsignal.size(); ++itck) {
  //      inputsignal[itck] = signal[itck];
  //    }
  //  }
  //  else if (!rawlist.empty()) {
  //    const auto& digitVec = rawlist[ich];

  //    view = geo->View(rawlist[ich]->Channel());

  //    std::vector<short> rawadc(fWaveformSize);
  //    raw::Uncompress(digitVec->ADCs(), rawadc, digitVec->GetPedestal(), digitVec->Compression());
  //    for (size_t itck = 0; itck < rawadc.size(); ++itck) {
  //      inputsignal[itck] = rawadc[itck] - digitVec->GetPedestal();
  //    }
  //  }

  //  // ... use waveform recognition CNN to perform inference on each window
  //  std::vector<bool> inroi(fWaveformSize, false);
  //  inroi = fWaveformRecogToolVec[view]->findROI(inputsignal);

  //  std::vector<float> sigs;
  //  int lastsignaltick = -1;
  //  int roistart = -1;

  //  recob::Wire::RegionsOfInterest_t rois(fWaveformSize);

  //  for (size_t i = 0; i < fWaveformSize; ++i) {
  //    if (inroi[i]) {
  //      if (sigs.empty()) {
  //        sigs.push_back(inputsignal[i]);
  //        lastsignaltick = i;
  //        roistart = i;
  //      }
  //      else {
  //        if (int(i) != lastsignaltick + 1) {
  //          rois.add_range(roistart, std::move(sigs));
  //          sigs.clear();
  //          sigs.push_back(inputsignal[i]);
  //          lastsignaltick = i;
  //          roistart = i;
  //        }
  //        else {
  //          sigs.push_back(inputsignal[i]);
  //          lastsignaltick = i;
  //        }
  //      }
  //    }
  //  }
  //  if (!sigs.empty()) { rois.add_range(roistart, std::move(sigs)); }
  //  if (!wirelist.empty()) {
  //    outwires->emplace_back(recob::Wire(rois, wirelist[ich]->Channel(), wirelist[ich]->View()));
  //  }
  //  else if (!rawlist.empty()) {
  //    outwires->emplace_back(
  //      recob::Wire(rois, rawlist[ich]->Channel(), geo->View(rawlist[ich]->Channel())));
  //  }
  //}

  e.put(std::move(outwires));
}


nnet::ApaView 
nnet::WireClusterFinder::GetApaView( art::Ptr<recob::Wire> &wire )
{
  raw::ChannelID_t chan = wire->Channel();
  ApaView ret(chan/fNChanPerApa, wire->View() );
  return ret;
}


void 
nnet::WireClusterFinder::SortWirePtrByChannel( std::vector<art::Ptr<recob::Wire>> &vec, bool increasing )
{
  if (increasing)
  {
    std::sort(vec.begin(), vec.end(), [](art::Ptr<recob::Wire> &a, art::Ptr<recob::Wire> &b) { return a->Channel() < b->Channel(); });
  }
  else
  {
    std::sort(vec.begin(), vec.end(), [](art::Ptr<recob::Wire> &a, art::Ptr<recob::Wire> &b) { return a->Channel() > b->Channel(); });
  }
}


void 
nnet::WireClusterFinder::PrintWirePtrByChannel( std::vector<art::Ptr<recob::Wire>> &vec )
{
  std::cout<<"print channels === "<<std::endl;
  for( auto p : vec ) std::cout<<p->Channel()<<", ";
  std::cout<<"  print channel ROI"<<std::endl;
  for( auto p : vec ) 
  {
    std::cout<<Form("channel %d :",p->Channel());
    for( auto roi: p->SignalROI().get_ranges() )
    {
      std::cout<<Form("(%d, %d), ",(int) roi.begin_index(), (int) roi.end_index() );
    }
  }
  std::cout<<std::endl;
}

DEFINE_ART_MODULE(nnet::WireClusterFinder)
