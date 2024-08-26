#include "rust-opendds/include/rust-opendds.h"

#include <dds/DCPS/Registered_Data_Types.h>
#include <dds/DCPS/Marked_Default_Qos.h>

#include <ace/DLL_Manager.h>

//#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace OpenDDS {

DDS::DomainParticipantFactory_var dpf_;

//std::vector<DDS::DomainParticipant_var> participants_;
std::unordered_map<DomainParticipantHandle, DDS::DomainParticipant_var> participants_;

void initialize(int argc, rust::Vec<rust::String> argv)
{
  char* argv_str[argv.size()];
  for (size_t i = 0; i < argv.size(); ++i) {
    argv_str[i] = const_cast<char*>(argv[i].c_str());
  }
  dpf_ = TheParticipantFactoryWithArgs(argc, argv_str);
  ACE_DEBUG((LM_DEBUG, "C++: OpenDDS::initialize\n"));
}

void load(rust::String lib_path)
{
  auto ret = ACE_DLL_Manager::instance()->open_dll(ACE_TEXT_CHAR_TO_TCHAR(lib_path.c_str()),
                                                   ACE_DEFAULT_SHLIB_MODE, 0);
  if (!ret) {
    throw runtime_error("load: open_dll failed for library " + (std::string)lib_path);
  }
  ACE_DEBUG((LM_DEBUG, "C++: OpenDDS::load\n"));
}

// TODO: Support custom QoS
//DDS::DomainParticipant_var& create_participant(int domain_id)
std::unique_ptr<DomainParticipantHandle> create_participant(int domain_id)
{
  DDS::DomainParticipantQos qos;
  dpf_->get_default_participant_qos(qos);
  DDS::DomainParticipant_var dp = dpf_->create_participant(domain_id, qos, 0, 0);
  if (!dp) {
    throw std::runtime_error("create_participant: create_participant failed");
  }
  //  auto ret = participants_.insert(participants_.end(), dp);
  auto handle = participants_.size();
  participants_.insert(std::make_pair(handle, dp));
  ACE_DEBUG((LM_DEBUG, "C++: OpenDDS::create_participant\n"));
  //  return *ret;
  return std::unique_ptr<DomainParticipantHandle>(new DomainParticipantHandle(handle));
}

//void delete_participant(DDS::DomainParticipant_var& dp)
void delete_participant(std::unique_ptr<DomainParticipantHandle> dp_handle)
{
  //  std::vector<DDS::DomainParticipant_var>::iterator iter =
  //    std::find(participants_.begin(), participants_.end(), dp);
  //  if (iter != participants_.end()) {
  //    participants_.erase(iter);
  //  }
  const DomainParticipantHandle handle = *dp_handle;
  if (participants_.count(handle)) {
    DDS::DomainParticipant_var& dp = participants_[handle];
    dp->delete_contained_entities();
    dpf_->delete_participant(dp);
    participants_.erase(handle);
    ACE_DEBUG((LM_DEBUG, "C++: OpenDDS::delete_participant with handle %u\n", handle));
  }
  dp_handle.reset(0);
}

// TODO: Support custom QoS and call back
void subscribe(const std::unique_ptr<DomainParticipantHandle>& dp_handle,
               rust::String topic_name, rust::String topic_type)
{
  DDS::DomainParticipant_var& dp = participants_[*dp_handle];
  OpenDDS::DCPS::TypeSupport* ts = Registered_Data_Types->lookup(dp, topic_type.c_str());
  if (!ts) {
    ts = Registered_Data_Types->lookup(0, topic_type.c_str());
    if (!ts) {
      throw std::runtime_error(std::string("subscribe: type ") + topic_type.c_str() + " is not registered");
    }
    Registered_Data_Types->register_type(dp, topic_type.c_str(), ts);
  }

  DDS::Topic_var topic_var = dp->create_topic(topic_name.c_str(), topic_type.c_str(),
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

  DDS::DataReader_var dr = sub->create_datareader(topic, dr_qos, 0, DDS::DATA_AVAILABLE_STATUS);
  if (!dr) {
    throw std::runtime_error("subscribe: create_datareader failed");
  }
  ACE_DEBUG((LM_DEBUG, "C++: OpenDDS::subscribe: subscribe topic '%C' with type '%C'\n",
             topic_name.c_str(), topic_type.c_str()));
}

void unsubscribe()
{
}

void create_datawriter()
{
}

void write()
{
}

}
