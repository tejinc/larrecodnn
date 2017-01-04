//////////////////////////////////////////////////////////////////////////////
// \version 
//
// \brief Wrapper for saving MVA results into art::Event
//
// \author robert.sulej@cern.ch
//
//////////////////////////////////////////////////////////////////////////////
#ifndef ANAB_MVAWRITER_H
#define ANAB_MVAWRITER_H

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Utilities/InputTag.h"

#include "larreco/RecoAlg/ImagePatternAlgs/PointIdAlg/MVAWrapperBase.h"

namespace anab {

/// Index to the MVA output collection, used when result vectors are added or set.
typedef size_t MVAOutput_ID;

/// Helper for registering in the art::EDProducer all data products needed for
/// N-output MVA results: keep MVADescriptions<N> for all types T in one collection
/// while separate instance names are used for the MVA output value collections for
/// each type T.
/// Use one instance of this class per one MVA model, applied to one or more types.
template <size_t N>
class MVAWriter : public MVAWrapperBase {
public:

    /// Name provided to the constructor is used as an instance name for MVADescription<N>
    /// and MVAOutput<N> (for which it is combined with the processed data product names);
    /// good idea is to use the name as an indication of what MVA model was used on the data
    /// (like eg. "emtrack" for outputs from a model distinguishing EM from track-like hits
    /// and clusters).
    MVAWriter(art::EDProducer* module, const char* name = "") :
        fProducer(module), fInstanceName(name),
        fIsDescriptionRegistered(false),
        fDescriptions(nullptr)
    { }

    /// Register the collection of metadata type MVADescription<N> (once for all data types
    /// for which MVA is saved) and the collection of MVAOutputs<N> (using data type name
    /// added to fInstanceName as instance name of the collection made for the type T).
    template <class T>
    void produces_using();

    /// Initialize container for MVA outputs and, if not yet done, the container for
    /// metadata, then creates metadata for data products of type T. MVA output container
    /// is initialized to hold dataSize vectors (if dataSize > 0): use setOutput() to
    /// store values.
    /// Returns index of outputs which should be used when saving actual output values.
    template <class T>
    MVAOutput_ID initOutputs(art::InputTag const & dataTag, size_t dataSize,
        std::vector< std::string > const & names = std::vector< std::string >(N, ""));

    void setOutput(MVAOutput_ID id, size_t key, std::array<float, N> const & values) { (*(fOutputs[id]))[key] = values; }
    void setOutput(MVAOutput_ID id, size_t key, std::array<double, N> const & values) { (*(fOutputs[id]))[key] = values; }
    void setOutput(MVAOutput_ID id, size_t key, std::vector<float> const & values) { (*(fOutputs[id]))[key] = values; }
    void setOutput(MVAOutput_ID id, size_t key, std::vector<double> const & values) { (*(fOutputs[id]))[key] = values; }


    /// Initialize container for MVA outputs and, if not yet done, the container for
    /// metadata, then creates metadata for data products of type T. MVA output container
    /// is initialized as EMPTY and MVA vectors should be added with addOutput() function.
    /// Returns index of outputs which should be used when adding actual output values.
    template <class T>
    MVAOutput_ID initOutputs(art::InputTag const & dataTag,
        std::vector< std::string > const & names = std::vector< std::string >(N, ""))
    { return initOutputs<T>(dataTag, 0, names); }

    void addOutput(MVAOutput_ID id, std::array<float, N> const & values) { fOutputs[id]->emplace_back(values); }
    void addOutput(MVAOutput_ID id, std::array<double, N> const & values) { fOutputs[id]->emplace_back(values); }
    void addOutput(MVAOutput_ID id, std::vector<float> const & values) { fOutputs[id]->emplace_back(values); }
    void addOutput(MVAOutput_ID id, std::vector<double> const & values) { fOutputs[id]->emplace_back(values); }


    /// Check consistency and save all the results in the event.
    void saveOutputs(art::Event & evt);

    /// Get MVA results accumulated over the vector of items (eg. over hits associated to a cluster).
    /// NOTE: MVA outputs for these items has to be added to the MVAWriter first!
    template <class T>
    std::array<float, N> getOutput(std::vector< art::Ptr<T> > const & items) const
    { return pAccumulate<T, N>(items, *(fOutputs[getProductID<T>()])); }

    /// Get MVA results accumulated with provided weights over the vector of items
    /// (eg. over clusters associated to a track, weighted by the cluster size; or
    /// over hits associated to a cluster, weighted by the hit area).
    /// NOTE: MVA outputs for these items has to be added to the MVAWriter first!
    template <class T>
    std::array<float, N> getOutput(std::vector< art::Ptr<T> > const & items,
        std::vector<float> const & weights) const
    { return pAccumulate<T, N>(items, weights, *(fOutputs[getProductID<T>()])); }

    /// Get MVA results accumulated with provided weighting function over the vector
    /// of items (eg. over clusters associated to a track, weighted by the cluster size;
    /// or over hits associated to a cluster, weighted by the hit area).
    /// NOTE: MVA outputs for these items has to be added to the MVAWriter first!
    template <class T>
    std::array<float, N> getOutput(std::vector< art::Ptr<T> > const & items,
        std::function<float (T const &)> fweight) const
    { return pAccumulate<T, N>(items, fweight, *(fOutputs[getProductID<T>()])); }

    template <class T>
    std::array<float, N> getOutput(std::vector< art::Ptr<T> > const & items,
        std::function<float (art::Ptr<T> const &)> fweight) const
    { return pAccumulate<T, N>(items, fweight, *(fOutputs[getProductID<T>()])); }

    /// Get copy of the MVA output vector for the type T, at index "key".
    template <class T>
    std::array<float, N> getOutput(size_t key) const
    {
        std::array<float, N> vout;
        auto const & src = ( *(fOutputs[getProductID<T>()]) )[key];
        for (size_t i = 0; i < N; ++i) vout[i] = src[i];
        return vout;
    }

    /// Get copy of the MVA output vector for the type T, idicated with art::Ptr::key().
    template <class T>
    std::array<float, N> getOutput(art::Ptr<T> const & item) const
    {
        std::array<float, N> vout;
        auto const & src = ( *(fOutputs[getProductID<T>()]) )[item.key()];
        for (size_t i = 0; i < N; ++i) vout[i] = src[i];
        return vout;
    }

    friend std::ostream& operator<< (std::ostream &o, MVAWriter const& a)
    {
        o << "MVAWriter for " << a.fInstanceName << ", " << N << " outputs";
        if (!a.fRegisteredDataTypes.empty())
        {
            o << ", ready to write results made for:" << std::endl;
            for (auto const & n : a.fRegisteredDataTypes) { o << "\t" << n << std::endl; }
        }
        else { o << ", nothing registered for writing to the events" << std::endl; }
        return o;
    }

private:

    // Data initialized for the module life:
    art::EDProducer* fProducer;
    std::string fInstanceName;

    std::vector< std::string > fRegisteredDataTypes;
    bool fIsDescriptionRegistered;

    // Data collected for each event:
    std::unordered_map< size_t, MVAOutput_ID > fTypeHashToID;
    template <class T> MVAOutput_ID getProductID() const;

    std::vector< std::unique_ptr< std::vector< anab::MVAOutput<N> > > > fOutputs;
    std::unique_ptr< std::vector< anab::MVADescription<N> > > fDescriptions;
    void clearEventData()
    {
        fTypeHashToID.clear(); fOutputs.clear();
        fDescriptions.reset(nullptr);
    }

    /// Check if the containers for results prepared for "tname" data type are ready.
    bool descriptionExists(std::string tname);
};

} // namespace anab

//----------------------------------------------------------------------------
// MVAWriter functions.
//
template <size_t N>
template <class T>
anab::MVAOutput_ID anab::MVAWriter<N>::getProductID() const
{
    auto const & ti = typeid(T);
    auto search = fTypeHashToID.find(getProductHash(ti));
    if (search != fTypeHashToID.end()) { return search->second; }
    else
    {
        throw cet::exception("MVAWriter") << "MVA not initialized for product " << getProductName(ti);
    }
}
//----------------------------------------------------------------------------

template <size_t N>
template <class T>
void anab::MVAWriter<N>::produces_using()
{
    if (!fIsDescriptionRegistered)
    {
        fProducer->produces< std::vector< anab::MVADescription<N> > >(fInstanceName);
        fIsDescriptionRegistered = true;
    }

    std::string dataName = getProductName(typeid(T));
    fProducer->produces< std::vector< anab::MVAOutput<N> > >(fInstanceName + dataName);
    fRegisteredDataTypes.push_back(dataName);
}
//----------------------------------------------------------------------------

template <size_t N>
bool anab::MVAWriter<N>::descriptionExists(std::string tname)
{
    if (!fDescriptions) return false;

    std::string n = fInstanceName + tname;
    for (auto const & d : *fDescriptions)
    {
        if (d.outputInstance() == n) { return true; }
    }
    return false;
}
//----------------------------------------------------------------------------

template <size_t N>
template <class T>
anab::MVAOutput_ID anab::MVAWriter<N>::initOutputs(
    art::InputTag const & dataTag, size_t dataSize,
    std::vector< std::string > const & names)
{
    size_t dataHash = getProductHash(typeid(T));
    std::string dataName = getProductName(typeid(T));
    if (!fDescriptions)
    {
        fDescriptions = std::make_unique< std::vector< anab::MVADescription<N> > >();
    }
    else if (descriptionExists(dataName))
    {
        throw cet::exception("MVAWriter") << "MVADescription<N> already initialized for " << dataName;
    }
    fDescriptions->emplace_back(dataTag.encode(), fInstanceName + dataName);

    fOutputs.push_back( std::make_unique< std::vector< anab::MVAOutput<N> > >() );
    anab::MVAOutput_ID id = fOutputs.size() - 1;
    fTypeHashToID[dataHash] = id;

    if (dataSize) { fOutputs[id]->resize(dataSize, anab::MVAOutput<N>(0.0F)); }

    return id;
}
//----------------------------------------------------------------------------

template <size_t N>
void anab::MVAWriter<N>::saveOutputs(art::Event & evt)
{
    for (auto const & n : fRegisteredDataTypes)
    {
        if (!descriptionExists(n))
        {
            throw cet::exception("MVAWriter") << "No MVADescription<N> prepared for type " << n;
        }
    }

    if (fOutputs.size() != fDescriptions->size())
    {
        throw cet::exception("MVAWriter") << "MVADescription<N> vector length not equal to the number of MVAOutput<N> vectors";
    }

    std::cout << "put..." << std::endl;
    for (size_t i = 0; i < fOutputs.size(); ++i)
    {
        evt.put(std::move(fOutputs[i]), (*fDescriptions)[i].outputInstance());
    }
    evt.put(std::move(fDescriptions), fInstanceName);
    std::cout << "...done" << std::endl;

    clearEventData();
    std::cout << "reset ok" << std::endl;
}
//----------------------------------------------------------------------------

#endif //ANAB_MVAREADER

