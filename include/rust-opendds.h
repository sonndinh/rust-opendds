#ifndef OPENDDS_RUST_OPENDDS_H
#define OPENDDS_RUST_OPENDDS_H

#include "rust/cxx.h"

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/TypeSupportImpl.h>

#include <memory>

namespace Rust_OpenDDS {

struct SampleInfo;
struct DomainParticipantQos;
struct TopicQos;
struct SubscriberQos;
struct DataReaderQos;
struct StatusMask;
struct ReturnCode_t;

using DomainParticipantVar = DDS::DomainParticipant_var;
using TopicVar = DDS::Topic_var;
using SubscriberVar = DDS::Subscriber_var;
using DataReaderVar = DDS::DataReader_var;

void initialize(int argc, rust::Vec<rust::String> argv);
void load(rust::String lib_path);

ReturnCode_t get_default_participant_qos(DomainParticipantQos& qos);

std::unique_ptr<DDS::DomainParticipant_var>
create_participant(int domain_id, const DomainParticipantQos& qos, StatusMask mask);

void delete_participant(std::unique_ptr<DDS::DomainParticipant_var> dp_ptr);

ReturnCode_t
get_default_topic_qos(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, TopicQos& qos);

std::unique_ptr<DDS::Topic_var>
create_topic(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, rust::String topic_name,
             rust::String type_name, const TopicQos& qos, StatusMask mask);

ReturnCode_t
get_default_subscriber_qos(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, SubscriberQos& qos);

std::unique_ptr<DDS::Subscriber_var>
create_subscriber(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, const SubscriberQos& qos, StatusMask mask);

ReturnCode_t
get_default_datareader_qos(const std::unique_ptr<DDS::Subscriber_var>& sub_ptr, DataReaderQos& qos);

std::unique_ptr<DDS::DataReader_var>
create_datareader(const std::unique_ptr<DDS::Subscriber_var>& sub_ptr,
                  const std::unique_ptr<DDS::Topic_var>& topic_ptr,
                  const DataReaderQos& qos, StatusMask mask);

ReturnCode_t
set_listener(const std::unique_ptr<DDS::DataReader_var>& dr_ptr,
             rust::Fn<void(SampleInfo, rust::String)> cb_fn, StatusMask mask,
             const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, rust::String type_name);

struct DataWriterInfo {
  DataWriterInfo() : dw_ptr(0), ts_ptr(0) {}

  DDS::DataWriter* dw_ptr;
  OpenDDS::DCPS::TypeSupport* ts_ptr;
};

std::unique_ptr<DataWriterInfo>
create_datawriter(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
                  rust::String topic_name, rust::String type_name);

void write(const std::unique_ptr<DataWriterInfo>& dwi_ptr, rust::String sample,
           DDS::InstanceHandle_t instance = DDS::HANDLE_NIL);

// Invoked by writer to wait for readers to join
void wait_for_readers(const std::unique_ptr<DataWriterInfo>& dwi_ptr);
}

#endif
