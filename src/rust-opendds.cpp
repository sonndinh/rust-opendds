#include "rust-opendds/include/rust-opendds.h"
#include "rust-opendds/include/DataReaderListenerImpl.h"
#include "rust-opendds/include/QosHelper.h"

#include <dds/DCPS/Registered_Data_Types.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/JsonValueReader.h>

#include <ace/DLL_Manager.h>

#include <stdexcept>
#include <iostream>

// TODO:
// 1. Handle termination, e.g., delete entities
// 2. Support DDS security
// 3. (Optional) Simple IDL mapping to Rust:
//    IDL struct -> Rust struct
//    IDL union -> Rust enum
// 4. Rework test: separate processes for pub and sub, etc.

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

ReturnCode_t get_default_participant_qos(DomainParticipantQos& qos)
{
  if (!dpf_) {
    throw std::runtime_error("get_default_participant_qos: initialize domain participant factory first!");
  }

  DDS::DomainParticipantQos dds_qos;
  const DDS::ReturnCode_t rc = dpf_->get_default_participant_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::DomainParticipant_var> create_participant(int domain_id, const DomainParticipantQos& qos, StatusMask mask)
{
  DDS::DomainParticipantQos dds_qos;
  to_dds_qos(dds_qos, qos);

  DDS::DomainParticipant_var dp = dpf_->create_participant(domain_id, dds_qos, 0, mask.value);
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

ReturnCode_t
get_default_topic_qos(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, TopicQos& qos)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  if (!dp) {
    throw std::runtime_error("get_default_topic_qos: domain participant is nil!");
  }

  DDS::TopicQos dds_qos;
  const DDS::ReturnCode_t rc = dp->get_default_topic_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::Topic_var>
create_topic(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, rust::String topic_name,
             rust::String type_name, const TopicQos& qos, StatusMask mask)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, type_name.c_str());
  if (!ts) {
    ts = Registered_Data_Types->lookup(0, type_name.c_str());
    if (!ts) {
      throw std::runtime_error(std::string("create_topic: type ") + type_name.c_str() + " is not registered");
    }
    Registered_Data_Types->register_type(dp, type_name.c_str(), ts);
  }

  DDS::TopicQos topic_qos;
  to_dds_qos(topic_qos, qos);
  DDS::Topic_var topic = dp->create_topic(topic_name.c_str(), type_name.c_str(),
                                          topic_qos, 0, mask.value);
  if (!topic) {
    throw std::runtime_error(std::string("create_topic: create topic ") + topic_name.c_str() + " failed");
  }

  // TODO: free the _var object
  std::unique_ptr<DDS::Topic_var> ret(new DDS::Topic_var);
  *ret = topic._retn();
  return ret;
}

ReturnCode_t get_default_subscriber_qos(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, SubscriberQos& qos)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  if (!dp) {
    throw std::runtime_error("get_default_subscriber_qos: domain participant is nil!");
  }

  DDS::SubscriberQos dds_qos;
  const DDS::ReturnCode_t rc = dp->get_default_subscriber_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::Subscriber_var>
create_subscriber(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, const SubscriberQos& qos, StatusMask mask)
{
  DDS::SubscriberQos dds_qos;
  to_dds_qos(dds_qos, qos);
  DDS::Subscriber_var sub = (*dp_ptr)->create_subscriber(dds_qos, 0, mask.value);
  if (!sub) {
    throw std::runtime_error("create_subscriber: create subscriber failed");
  }

  // TODO: free the _var object
  std::unique_ptr<DDS::Subscriber_var> ret(new DDS::Subscriber_var);
  *ret = sub._retn();
  return ret;
}

ReturnCode_t get_default_datareader_qos(const std::unique_ptr<DDS::Subscriber_var>& sub_ptr, DataReaderQos& qos)
{
  DDS::Subscriber_var& sub = *sub_ptr;
  if (!sub) {
    throw std::runtime_error("get_default_datareader_qos: subscriber is nil!");
  }

  DDS::DataReaderQos dds_qos;
  const DDS::ReturnCode_t rc = sub->get_default_datareader_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::DataReader_var>
create_datareader(const std::unique_ptr<DDS::Subscriber_var>& sub_ptr,
                  const std::unique_ptr<DDS::Topic_var>& topic_ptr,
                  const DataReaderQos& qos, StatusMask mask)
{
  DDS::DataReaderQos dds_qos;
  to_dds_qos(dds_qos, qos);
  DDS::Subscriber_var& sub = *sub_ptr;
  DDS::Topic_var& topic = *topic_ptr;

  DDS::DataReader_var dr = sub->create_datareader(topic, dds_qos, 0, mask.value);
  if (!dr) {
    throw std::runtime_error("create_datareader: create data reader failed!");
  }

  // TODO: free the _var object
  std::unique_ptr<DDS::DataReader_var> ret(new DDS::DataReader_var);
  *ret = dr._retn();
  return ret;
}

ReturnCode_t
set_listener(const std::unique_ptr<DDS::DataReader_var>& dr_ptr,
             rust::Fn<void(SampleInfo, rust::String)> cb_fn, StatusMask mask,
             const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, rust::String type_name)
{
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(*dp_ptr, type_name.c_str());
  DDS::DataReaderListener_var listener(new DataReaderListenerImpl(ts, cb_fn));

  DDS::DataReader_var& dr = *dr_ptr;
  const DDS::ReturnCode_t rc = dr->set_listener(listener, mask.value);
  ReturnCode_t ret = { .value = rc};
  return ret;
}

ReturnCode_t get_default_publisher_qos(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, PublisherQos& qos)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  if (!dp) {
    throw std::runtime_error("get_default_publisher_qos: domain participant is nil!");
  }

  DDS::PublisherQos dds_qos;
  const DDS::ReturnCode_t rc = dp->get_default_publisher_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::Publisher_var>
create_publisher(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, const PublisherQos& qos, StatusMask mask)
{
  DDS::PublisherQos dds_qos;
  to_dds_qos(dds_qos, qos);
  DDS::Publisher_var pub = (*dp_ptr)->create_publisher(dds_qos, 0, mask.value);
  if (!pub) {
    throw std::runtime_error("create_publisher: create publisher failed");
  }

  // TODO: free the _var object
  std::unique_ptr<DDS::Publisher_var> ret(new DDS::Publisher_var);
  *ret = pub._retn();
  return ret;
}

ReturnCode_t get_default_datawriter_qos(const std::unique_ptr<DDS::Publisher_var>& pub_ptr, DataWriterQos& qos)
{
  DDS::Publisher_var& pub = *pub_ptr;
  if (!pub) {
    throw std::runtime_error("get_default_datawriter_qos: publisher is nil!");
  }

  DDS::DataWriterQos dds_qos;
  const DDS::ReturnCode_t rc = pub->get_default_datawriter_qos(dds_qos);
  ReturnCode_t ret;
  ret.value = rc;
  if (rc == DDS::RETCODE_OK) {
    to_cxx_qos(qos, dds_qos);
  }
  return ret;
}

std::unique_ptr<DDS::DataWriter_var>
create_datawriter(const std::unique_ptr<DDS::Publisher_var>& pub_ptr,
                  const std::unique_ptr<DDS::Topic_var>& topic_ptr,
                  const DataWriterQos& qos, StatusMask mask)
{
  DDS::DataWriterQos dds_qos;
  to_dds_qos(dds_qos, qos);
  DDS::Publisher_var& pub = *pub_ptr;
  DDS::Topic_var& topic = *topic_ptr;

  DDS::DataWriter_var dw = pub->create_datawriter(topic, dds_qos, 0, mask.value);
  if (!dw) {
    throw std::runtime_error("create_datawriter: create data writer failed!");
  }

  // TODO: free the _var object
  std::unique_ptr<DDS::DataWriter_var> ret(new DDS::DataWriter_var);
  *ret = dw._retn();
  return ret;
}

void wait_for_readers(const std::unique_ptr<DDS::DataWriter_var>& dw_ptr)
{
  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::wait_for_readers: Waiting for matched readers...\n"));
  DDS::InstanceHandleSeq handles;
  while (true) {
    (*dw_ptr)->get_matched_subscriptions(handles);
    if (handles.length() > 0) {
      ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::wait_for_readers: %d matched reader(s)\n", handles.length()));
      break;
    }
    ACE_OS::sleep(ACE_Time_Value(0, 500000));
  }
}

InstanceHandle_t
register_instance(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
                  const std::unique_ptr<DDS::DataWriter_var>& dw_ptr,
                  rust::String type_name, rust::String instance)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, type_name.c_str());
  if (!ts) {
    throw std::runtime_error(std::string("register_instance: type support for type \"") + type_name.c_str() + "\" not found");
  }

  const OpenDDS::DCPS::ValueDispatcher* const vd = dynamic_cast<const OpenDDS::DCPS::ValueDispatcher* const>(ts);
  if (!vd) {
    throw std::runtime_error("C++: Rust_OpenDDS:register_instance: Failed to get ValueDispatcher");
  }

  void* instance_obj = vd->new_value();
  rapidjson::StringStream buffer(instance.c_str());
  OpenDDS::DCPS::JsonValueReader<> jvr(buffer);
  if (!vd->read(jvr, instance_obj)) {
    vd->delete_value(instance_obj);
    throw std::runtime_error(std::string("C++: Rust_OpenDDS::register_instance: Failed to read JSON sample with type ") + type_name.c_str());
  }

  DDS::DataWriter_var& dw = *dw_ptr;
  const DDS::InstanceHandle_t ih = vd->register_instance_helper(dw, instance_obj);
  vd->delete_value(instance_obj);

  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::register_instance for type %C\n", type_name.c_str()));
  return InstanceHandle_t{ih};
}

// TODO: Right now, every write would need to look up the TypeSupport for the given type.
// A better solution would be a class that cache and encapsulate the TypeSupport object.
// Or it could factor out the type support lookup into a separate function and the user
// is responsible for passing that object to the subsequent functions, such as write, register_instance.
ReturnCode_t
write(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
      const std::unique_ptr<DDS::DataWriter_var>& dw_ptr,
      rust::String type_name, rust::String sample, InstanceHandle_t instance_handle)
{
  DDS::DomainParticipant_var& dp = *dp_ptr;
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, type_name.c_str());
  if (!ts) {
    throw std::runtime_error(std::string("write: type support for type \"") + type_name.c_str() + "\" not found");
  }

  const OpenDDS::DCPS::ValueDispatcher* const vd = dynamic_cast<const OpenDDS::DCPS::ValueDispatcher* const>(ts);
  if (!vd) {
    throw std::runtime_error("C++: Rust_OpenDDS::write: Failed to get ValueDispatcher");
  }

  void* sample_obj = vd->new_value();
  rapidjson::StringStream buffer(sample.c_str());
  OpenDDS::DCPS::JsonValueReader<> jvr(buffer);
  if (!vd->read(jvr, sample_obj)) {
    vd->delete_value(sample_obj);
    throw std::runtime_error(std::string("C++: Rust_OpenDDS::write: Failed to read JSON sample with type ") + type_name.c_str());
  }

  DDS::DataWriter_var& dw = *dw_ptr;
  const DDS::ReturnCode_t rc = vd->write_helper(dw, sample_obj, instance_handle.value);
  vd->delete_value(sample_obj);

  ACE_DEBUG((LM_DEBUG, "C++: Rust_OpenDDS::write sample %C\n", type_name.c_str()));
  return ReturnCode_t{rc};
}

}
