// Only for testing
#include "idl/NodeJSTestTypeSupportImpl.h"

#include "rust-opendds/include/rust-opendds.h"
#include "rust-opendds/include/DataReaderListenerImpl.h"

#include <dds/DCPS/Registered_Data_Types.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/JsonValueReader.h>

#include <ace/DLL_Manager.h>

#include <stdexcept>
#include <iostream>

namespace Rust_OpenDDS {

DDS::DomainParticipantFactory_var dpf_;

void initialize(int argc, rust::Vec<rust::String> argv)
{
  char* argv_str[argv.size()];
  for (size_t i = 0; i < argv.size(); ++i) {
    argv_str[i] = const_cast<char*>(argv[i].c_str());
  }
  dpf_ = TheParticipantFactoryWithArgs(argc, argv_str);
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::initialize\n"));
}

void load(rust::String lib_path)
{
  auto ret = ACE_DLL_Manager::instance()->open_dll(ACE_TEXT_CHAR_TO_TCHAR(lib_path.c_str()),
                                                   ACE_DEFAULT_SHLIB_MODE, 0);
  if (!ret) {
    throw std::runtime_error("load: open_dll failed for library " + (std::string)lib_path);
  }
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::load\n"));
}

// TODO: Support custom QoS
std::unique_ptr<DDS::DomainParticipant_var> create_participant(int domain_id)
{
  DDS::DomainParticipantQos qos;
  dpf_->get_default_participant_qos(qos);
  DDS::DomainParticipant_var dp = dpf_->create_participant(domain_id, qos, 0, 0);
  if (!dp) {
    throw std::runtime_error("create_participant: create_participant failed");
  }

  std::unique_ptr<DDS::DomainParticipant_var> ret(new DDS::DomainParticipant_var);
  *ret = dp._retn();
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::create_participant\n"));
  return ret;
}

void delete_participant(std::unique_ptr<DDS::DomainParticipant_var> dp_ptr)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  dp->delete_contained_entities();
  dpf_->delete_participant(dp);
  dp_ptr.reset(0);
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::delete_participant\n"));
}

// TODO: Support custom QoS and call back
void subscribe(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
               rust::String topic_name, rust::String type_name)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, type_name.c_str());
  if (!ts) {
    ts = Registered_Data_Types->lookup(0, type_name.c_str());
    if (!ts) {
      throw std::runtime_error(std::string("subscribe: type ") + type_name.c_str() + " is not registered");
    }
    Registered_Data_Types->register_type(dp, type_name.c_str(), ts);
  }

  DDS::Topic_var topic_var = dp->create_topic(topic_name.c_str(), type_name.c_str(),
                                              TOPIC_QOS_DEFAULT, 0, 0);
  DDS::TopicDescription_var topic = DDS::TopicDescription::_duplicate(topic_var);
  if (!topic) {
    throw std::runtime_error("subscribe: create topic failed");
  }

  const DDS::Subscriber_var sub = dp->create_subscriber(SUBSCRIBER_QOS_DEFAULT, 0, 0);
  if (!sub) {
    throw std::runtime_error("subscribe: create_subscriber failed");
  }

  DDS::DataReaderQos dr_qos;
  sub->get_default_datareader_qos(dr_qos);
  DataReaderListenerImpl* const drli = new DataReaderListenerImpl;
  const DDS::DataReaderListener_var listen(drli);

  DDS::DataReader_var dr = sub->create_datareader(topic, dr_qos, listen, DDS::DATA_AVAILABLE_STATUS);
  if (!dr) {
    throw std::runtime_error("subscribe: create_datareader failed");
  }
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::subscribe: subscribe topic '%C' with type '%C'\n",
             topic_name.c_str(), type_name.c_str()));
}

void unsubscribe()
{
}

std::unique_ptr<DataWriterInfo> create_datawriter(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
                                                  rust::String topic_name, rust::String type_name)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, type_name.c_str());
  if (!ts) {
    ts = Registered_Data_Types->lookup(0, type_name.c_str());
    if (!ts) {
      throw std::runtime_error(std::string("create_datawriter: type ") + type_name.c_str() + " is not registered");
    }
    Registered_Data_Types->register_type(dp, type_name.c_str(), ts);
  }

  DDS::Topic_var topic = dp->create_topic(topic_name.c_str(), type_name.c_str(),
                                          TOPIC_QOS_DEFAULT, 0, 0);
  DDS::PublisherQos pub_qos;
  dp->get_default_publisher_qos(pub_qos);

  DDS::Publisher_var pub = dp->create_publisher(pub_qos, 0, 0);
  if (!pub) {
    throw std::runtime_error("create_datawriter: Failed to create publisher");
  }

  DDS::DataWriterQos dw_qos;
  pub->get_default_datawriter_qos(dw_qos);

  DDS::DataWriter_var dw = pub->create_datawriter(topic, dw_qos, 0, 0);
  if (!dw) {
    throw std::runtime_error("create_datawriter: Failed to create data writer");
  }

  std::unique_ptr<DataWriterInfo> ret(new DataWriterInfo);
  ret->dw_ptr = dw._retn();
  ret->ts_ptr = ts;

  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::create_datawriter for topic '%C' with type '%C'\n",
             topic_name.c_str(), type_name.c_str()));
  return ret;
}

void write(const std::unique_ptr<DataWriterInfo>& dwi_ptr, rust::String sample, DDS::InstanceHandle_t instance)
{
  DDS::DataWriter* const dw = dwi_ptr->dw_ptr;
  OpenDDS::DCPS::TypeSupport* const ts = dwi_ptr->ts_ptr;
  const OpenDDS::DCPS::ValueDispatcher* const vd = dynamic_cast<const OpenDDS::DCPS::ValueDispatcher* const>(ts);
  if (!vd) {
    throw std::runtime_error("C++: Rust_OpenDDS::write: Failed to get ValueDispatcher");
  }

  CORBA::String_var type_name = ts->get_type_name();

  void* sample_obj = vd->new_value();
  rapidjson::StringStream buffer(sample.c_str());
  OpenDDS::DCPS::JsonValueReader<> jvr(buffer);
  if (!vd->read(jvr, sample_obj)) {
    vd->delete_value(sample_obj);
    throw std::runtime_error(std::string("C++: Rust_OpenDDS::write: Failed to read JSON sample with type ") + type_name.in());
  }

  const DDS::ReturnCode_t rc = vd->write_helper(dw, sample_obj, instance);
  vd->delete_value(sample_obj);
  if (rc != DDS::RETCODE_OK) {
    throw std::runtime_error(std::string("C++: Rust_OpenDDS::write: Failed to write sample with type ") + type_name.in());
  }

  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::write sample %C\n", type_name.in()));
}

}
