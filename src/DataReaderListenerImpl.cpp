#include "rust-opendds/include/DataReaderListenerImpl.h"
#include "rust-opendds/src/lib.rs.h"

#include <dds/DCPS/JsonValueWriter.h>

#include <stdexcept>

namespace Rust_OpenDDS {

void DataReaderListenerImpl::on_data_available(DDS::DataReader* dr)
{
  ACE_DEBUG((LM_DEBUG, "C++: DataReaderListenerImpl::on_data_available: sample received\n"));
  OpenDDS::DCPS::DataReaderImpl* const dri = dynamic_cast<OpenDDS::DCPS::DataReaderImpl*>(dr);
  OpenDDS::DCPS::DataReaderImpl::GenericBundle bundle;
  const DDS::ReturnCode_t rc = dri->take(*this, DDS::NOT_READ_SAMPLE_STATE,
                                         DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);
  if (rc != DDS::RETCODE_OK) {
    throw std::runtime_error("C++: DataReaderListenerImpl::on_data_available: take failed\n");
  }
}

void DataReaderListenerImpl::push_back(const DDS::SampleInfo& si, const void* sample)
{
  const OpenDDS::DCPS::ValueDispatcher* const vd = dynamic_cast<const OpenDDS::DCPS::ValueDispatcher* const>(ts_);
  if (!vd) {
    throw std::runtime_error("C++: DataReaderListenerImpl::push_back: Failed to get ValueDispatcher");
  }

  CORBA::String_var type_name = ts_->get_type_name();

  const OpenDDS::DCPS::Sample::Extent ext =
    si.valid_data ? OpenDDS::DCPS::Sample::Full : OpenDDS::DCPS::Sample::KeyOnly;

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  OpenDDS::DCPS::JsonValueWriter<rapidjson::Writer<rapidjson::StringBuffer>> jvw(writer);
  if (!vd->write(jvw, sample, ext)) {
    throw std::runtime_error(std::string("C++: DataReaderListenerImpl::push_back: Failed to write JSON sample with type") + type_name.in());
  }

  // Invoke the Rust application's callback
  SampleInfo sample_info;
  sample_info.sample_state.value = si.sample_state;
  sample_info.view_state.value = si.view_state;
  sample_info.instance_state.value = si.instance_state;
  sample_info.source_timestamp.sec = si.source_timestamp.sec;
  sample_info.source_timestamp.nanosec = si.source_timestamp.nanosec;
  sample_info.instance_handle.value = si.instance_handle;
  sample_info.publication_handle.value = si.publication_handle;
  sample_info.disposed_generation_count = si.disposed_generation_count;
  sample_info.no_writers_generation_count = si.no_writers_generation_count;
  sample_info.sample_rank = si.sample_rank;
  sample_info.generation_rank = si.generation_rank;
  sample_info.absolute_generation_rank = si.absolute_generation_rank;
  sample_info.valid_data = si.valid_data;
  sample_info.opendds_reserved_publication_seq = si.opendds_reserved_publication_seq;
  (*cb_fn_)(sample_info, buffer.GetString());
}

}
