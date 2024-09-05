#ifndef OPENDDS_DATAREADERLISTENERIMPL_H
#define OPENDDS_DATAREADERLISTENERIMPL_H

#include <dds/DCPS/LocalObject.h>
#include <dds/DCPS/DataReaderImpl.h>
#include <dds/DdsDcpsSubscriptionC.h>

namespace Rust_OpenDDS {

class DataReaderListenerImpl : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> {
public:
  DataReaderListenerImpl() {}
  ~DataReaderListenerImpl() {}

  void on_requested_deadline_missed(DDS::DataReader*, const DDS::RequestedDeadlineMissedStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_requested_deadline_missed\n"));
  }

  void on_requested_incompatible_qos(DDS::DataReader*, const DDS::RequestedIncompatibleQosStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_requested_incompatible_qos\n"));
  }

  void on_sample_rejected(DDS::DataReader*, const DDS::SampleRejectedStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_sample_rejected\n"));
  }

  void on_liveliness_changed(DDS::DataReader*, const DDS::LivelinessChangedStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_liveliness_changed\n"));
  }

  void on_subscription_matched(DDS::DataReader*, const DDS::SubscriptionMatchedStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_subscription_matched\n"));
  }

  void on_sample_lost(DDS::DataReader*, const DDS::SampleLostStatus&)
  {
    ACE_DEBUG((LM_DEBUG, "C++: on_sample_lost\n"));
  }

  void on_data_available(DDS::DataReader*);
};

}

#endif
