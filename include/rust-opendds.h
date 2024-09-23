#ifndef OPENDDS_RUST_OPENDDS_H
#define OPENDDS_RUST_OPENDDS_H

#include "rust/cxx.h"

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/TypeSupportImpl.h>

#include <memory>

namespace Rust_OpenDDS {

struct SampleInfo;
struct DomainParticipantQos;
struct SubscriberQos;
struct StatusMask;
struct ReturnCode_t;

typedef DDS::DomainParticipant_var DomainParticipantVar;
typedef DDS::Subscriber_var SubscriberVar;

void initialize(int argc, rust::Vec<rust::String> argv);
void load(rust::String lib_path);

ReturnCode_t get_default_participant_qos(DomainParticipantQos& qos);
std::unique_ptr<DDS::DomainParticipant_var> create_participant(int domain_id, const DomainParticipantQos& qos, StatusMask mask);
void delete_participant(std::unique_ptr<DDS::DomainParticipant_var> dp_ptr);

std::unique_ptr<DDS::Subscriber_var> create_subscriber(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, const SubscriberQos& qos, StatusMask mask);

void subscribe(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr, rust::String topic_name,
               rust::String type_name, rust::Fn<void(SampleInfo, rust::String)> cb_fn);

struct DataWriterInfo {
  DataWriterInfo() : dw_ptr(0), ts_ptr(0) {}

  DDS::DataWriter* dw_ptr;
  OpenDDS::DCPS::TypeSupport* ts_ptr;
};

std::unique_ptr<DataWriterInfo> create_datawriter(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
                                                    rust::String topic_name, rust::String type_name);

// Invoked by writer to wait for readers to join
void wait_for_readers(const std::unique_ptr<DataWriterInfo>& dwi_ptr);
void write(const std::unique_ptr<DataWriterInfo>& dwi_ptr, rust::String sample,
           DDS::InstanceHandle_t instance = DDS::HANDLE_NIL);
}

#endif
